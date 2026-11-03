//
// Utility.cpp
//
// Library: CppParser
// Package: CppParser
// Module:  Utility
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/CppParser/Utility.h"
#include "Poco/CppParser/Parser.h"
#include "Poco/CppParser/Struct.h"
#include "Poco/StringTokenizer.h"
#include "Poco/Glob.h"
#include "Poco/Path.h"
#include "Poco/File.h"
#include "Poco/Process.h"
#include "Poco/Environment.h"
#include "Poco/NumberFormatter.h"
#include "Poco/Exception.h"
#include "Poco/Config.h"
#include <fstream>
#include <cstddef>
#include <filesystem>
#include <set>
#include <string>
#include <system_error>
#include <utility>
#include <vector>


using Poco::StringTokenizer;
using Poco::Glob;
using Poco::Path;
using Poco::File;
using Poco::Process;
using Poco::ProcessHandle;
using Poco::Environment;
using Poco::NumberFormatter;
using Poco::Exception;


namespace Poco::CppParser {


namespace {
#if defined(_WIN32)
	constexpr bool CASELESS_SEARCH = true;
#else
	constexpr bool CASELESS_SEARCH = false;
#endif
	constexpr int GLOB_OPTIONS = CASELESS_SEARCH ? Glob::GLOB_CASELESS : Glob::GLOB_DEFAULT;

	// Longer than any chain of links that a supported system resolves; also ends a loop of links.
	constexpr int MAX_LINK_CHAIN = 64;


	// Poco paths are UTF-8, which std::filesystem::path takes as char8_t since C++20.
	[[nodiscard]] std::filesystem::path toFilesystemPath(const std::string& path)
	{
#if (POCO_HAVE_CPP20_COMPILER)
		return {std::u8string(path.begin(), path.end())};
#else
		return std::filesystem::u8path(path);
#endif
	}


	[[nodiscard]] bool isLink(const std::filesystem::path& path)
	{
		const auto type = std::filesystem::symlink_status(path).type();
#if defined(_MSVC_STL_VERSION)
		// The MSVC standard library gives a junction a file type of its own.
		if (type == std::filesystem::file_type::junction) return true;
#endif
		return type == std::filesystem::file_type::symlink;
	}


	// A change of the entries of the deepest existing directory on the target path can make the link resolve.
	// The components before a ".." are resolved, as the system resolves them.
	void collectLinkTarget(const std::filesystem::path& link, int chainLength, std::set<std::string>& directories)
	{
		if (chainLength > MAX_LINK_CHAIN) return;
		std::filesystem::path next = link.parent_path() / std::filesystem::read_symlink(link);
		std::filesystem::path dir = next.parent_path();
		std::error_code error;
		while (dir.has_relative_path() && !std::filesystem::is_directory(dir, error))
		{
			next = dir;
			dir = dir.parent_path();
		}
		std::filesystem::path deepest;
		for (const auto& component: dir)
		{
			deepest /= component;
			if (component == "..") deepest = std::filesystem::canonical(deepest);
		}
		const auto deepestName = deepest.u8string();
		Path deepestPath(std::string(deepestName.begin(), deepestName.end()));
		deepestPath.makeDirectory();
		directories.insert(deepestPath.toString());
		if (isLink(next))
		{
			collectLinkTarget(next, chainLength + 1, directories);
		}
	}


	void searchDirectory(const Path& dir, const std::string& component, bool isFileName,
		std::vector<Path>& found, std::set<std::string>& directories)
	{
		// Entries of every kind match, so that links that do not resolve to a directory are seen.
		Path entryPattern(dir);
		entryPattern.setFileName(component);
		std::set<std::string> matches;
		Glob::glob(entryPattern, dir, matches, GLOB_OPTIONS);
		bool matched = false;
		std::error_code error;
		for (const auto& match: matches)
		{
			Path entry(match);
			entry.makeFile();
			const std::filesystem::path entryPath = toFilesystemPath(entry.toString());
			// On Windows, Glob takes a directory link for a directory even where it does not resolve to one.
			try
			{
				if (isFileName)
				{
					matched = true;
				}
				else if (std::filesystem::is_directory(entryPath, error))
				{
					found.push_back(entry.makeDirectory());
					matched = true;
				}
				else if (isLink(entryPath))
				{
					collectLinkTarget(entryPath, 1, directories);
				}
			}
			catch (const std::filesystem::filesystem_error&)
			{
				// An entry that cannot be followed does not match, so its directory is listed below.
			}
		}
		// A caseless search also finds an entry added or renamed in another letter case.
		const bool hasWildcard = component.find_first_of("*?[") != std::string::npos;
		if (hasWildcard || !matched || CASELESS_SEARCH)
			directories.insert(dir.toString());
	}


	void collectSearchedDirectories(const std::string& includePattern, std::set<std::string>& directories)
	{
		const Path pattern(Path::expand(includePattern), Path::PATH_GUESS);
		Path dirPattern(pattern);
		dirPattern.makeDirectory();
		// Glob::glob() opens the leading ".." components (or the share of a UNC path) without searching them.
		Path base(dirPattern);
		const int minDepth = base.getNode().empty() ? 0 : 1;
		while (base.depth() > minDepth && base[base.depth() - 1] != "..")
		{
			base.popDirectory();
		}
		// An absolute start gives absolute Glob results, which Path does not read as the home directory.
		std::vector<Path> searched{base.absolute()};
		for (int depth = base.depth(); depth < dirPattern.depth() && !searched.empty(); ++depth)
		{
			const bool isFileName = depth + 1 == dirPattern.depth() && pattern.isFile();
			std::vector<Path> found;
			for (const auto& dir: searched)
			{
				searchDirectory(dir, dirPattern.directory(depth), isFileName, found, directories);
			}
			searched = std::move(found);
		}
	}
}


void Utility::parseDir(const std::vector<std::string>& includePattern, const std::vector<std::string>& excludePattern, NameSpace::SymbolTable& st, const std::string& exec, const std::string& options, const std::string& path)
{
	std::set<std::string> files;
	Utility::buildFileList(files, includePattern, excludePattern);
	for (const auto& file : files)
	{
		Utility::parse(file, st, exec, options, path);
	}
	Utility::fixup(st);
}


void Utility::parse(const std::string& file, NameSpace::SymbolTable& st, const std::string& exec, const std::string& options, const std::string& path)
{
	std::string prepFile = Utility::preprocessFile(file, exec, options, path);
	Utility::parseOnly(file, st, prepFile, true);
}


void Utility::collectDependencies(const std::vector<std::string>& includePattern,
	const std::vector<std::string>& excludePattern,
	std::set<std::string>& files,
	std::set<std::string>& directories)
{
	std::set<std::string> found;
	buildFileList(found, includePattern, excludePattern);
	const std::string current = Path::current();
	for (const auto& file: found)
	{
		// On POSIX, Path reads a relative path whose first directory is "~" or ends with ":" as the home directory
		// or a device.
		files.insert(Path(toFilesystemPath(file).has_root_path() ? file : current + file).toString());
	}
	for (const auto& pattern: includePattern)
	{
		collectSearchedDirectories(pattern, directories);
	}
}


void Utility::fixup(NameSpace::SymbolTable& st)
{
	for (auto& [name, pSym] : st)
	{
		if (pSym->kind() == Symbol::SYM_STRUCT)
		{
			static_cast<Struct*>(pSym)->fixupBases();
		}
	}
}


void Utility::detectPrefixAndIncludes(const std::string& origHFile, std::vector<std::string>& lines, std::string& prefix)
{
	std::ifstream istr(origHFile.c_str());
	try
	{
		if (istr.good())
		{
			std::string x;
			istr >> x;
			while (x.find("#ifndef") == std::string::npos)
				istr >> x;
			StringTokenizer tokenizer(x, " \t", StringTokenizer::TOK_IGNORE_EMPTY | StringTokenizer::TOK_TRIM);
			poco_assert (tokenizer.count() == 2);
			StringTokenizer::Iterator itTmp = tokenizer.begin();
			++itTmp;
			std::string defValue = *itTmp;
			istr >> x;
			// now find the corresponding #define
			while (x.find(defValue) == std::string::npos)
				istr >> x;
			 //now parse until a class def is found without a ; at the end
			bool stop = false;
			std::string prefixHint;
			do
			{
				istr >> x;
				// we add EVERYTHING to lines: reason: used macros/preprocessor defines, conditional includes should all be present in the generated code
				// just think about fwd declarations inside a NS_BEGIN ... NS_END block
				if (x.find("class") != std::string::npos && x.find(";") == std::string::npos)
				{
					StringTokenizer tok(x, " \t", StringTokenizer::TOK_IGNORE_EMPTY | StringTokenizer::TOK_TRIM);
					StringTokenizer::Iterator it = tok.begin();
					while (*it != "class" && it != tok.end())
						++it;
					// the next after class must be *_API or in case of a template it must be the class name
					++it;
					std::size_t apiPos = it->find("_API");
					if (apiPos != std::string::npos)
					{
						prefixHint = it->substr(0, apiPos);
					}
					stop = true;
				}
				else
				{
					lines.push_back(x);
				}
			}
			while (!stop && !istr.eof());
			if (!stop)
			{
				lines.clear();
				prefix.clear();
				//throw Poco::SyntaxException("Failed to parse file " + origHFile + ".No class declared?");
				return;
			}

			// now search the prefix
			if (lines.empty())
			{
				prefix.clear();
				return;
			}
			// the prefix for that file
			std::vector<std::string>::const_iterator it = lines.end();
			--it;
			std::vector<std::string>::const_iterator itBegin = lines.begin();
			for (; it != itBegin; --it)
			{
				std::size_t prefixPos = it->find("_BEGIN");
				if (prefixPos != std::string::npos)
				{
					prefix = it->substr(0, prefixPos);
					if (prefix != prefixHint && !prefixHint.empty())
					{
						throw Poco::SyntaxException("Conflicting prefixes detected: " + prefixHint + "<->" + prefix);
					}
				}
			}
		}
		else throw Poco::OpenFileException(origHFile);
	}
	catch (Exception&)
	{
		istr.close();
		throw;
	}
	istr.close();
}


std::string Utility::preprocessFile(const std::string& file, const std::string& exec, const std::string& options, const std::string& path)
{
	Path pp(file);
	pp.setExtension("i");

	std::string popts;
	for (const auto ch : options)
	{
		if (ch == '%')
			popts += pp.getBaseName();
		else
			popts += ch;
	}
	StringTokenizer tokenizer(popts, ",;\n", StringTokenizer::TOK_IGNORE_EMPTY | StringTokenizer::TOK_TRIM);
	std::vector<std::string> args(tokenizer.begin(), tokenizer.end());
#ifdef _WIN32
	std::string quotedFile("\"");
	quotedFile += file;
	quotedFile += "\"";
	args.push_back(quotedFile);
#else
	args.push_back(file);
#endif
	if (!path.empty())
	{
		std::string newPath(Environment::get("PATH"));
		newPath += Path::pathSeparator();
		newPath += path;
		Environment::set("PATH", path);
	}

	auto proc = Process::launch(exec, args);
	const int rc = Process::wait(proc);
	if (rc != 0)
	{
		throw Poco::RuntimeException("Failed to process file");
	}

	return pp.getFileName();
}


void Utility::parseOnly(const std::string& file, NameSpace::SymbolTable& st, const std::string& preprocessedFile, bool removePreprocessedFile)
{
	std::ifstream istr(preprocessedFile.c_str());
	try
	{
		if (istr.good())
		{
			Parser parser(st, file, istr);
			parser.parse();
		}
		else throw Poco::OpenFileException(preprocessedFile);
	}
	catch (Exception&)
	{
		istr.close();
		if (removePreprocessedFile)
			removeFile(preprocessedFile);
		throw;
	}
	istr.close();
	if (removePreprocessedFile)
		removeFile(preprocessedFile);
}


void Utility::removeFile(const std::string& preprocessedfile)
{
	try
	{
		File f(preprocessedfile);
		f.remove();
	}
	catch (Exception&)
	{
	}
}


void Utility::buildFileList(std::set<std::string>& files, const std::vector<std::string>& includePattern, const std::vector<std::string>& excludePattern)
{
	std::set<std::string> temp;

	for (const auto& pattern : includePattern)
	{
		Glob::glob(pattern, temp, GLOB_OPTIONS);
	}

	for (const auto& file : temp)
	{
		const Path p(file);
		bool include = true;
		for (const auto& excPattern : excludePattern)
		{
			Glob glob(excPattern, GLOB_OPTIONS);
			if (glob.match(p.toString()))
				include = false;
		}
		if (include)
			files.insert(file);
	}
}


std::string replace(const std::string& input, const std::string& oldToken, const std::string& newToken)
{
	std::string result;
	std::size_t start = 0;
	std::size_t pos = 0;
	do
	{
		pos = input.find(oldToken, start);
		result.append(input.substr(start, pos-start));
		if (pos != std::string::npos)
			result.append(newToken);
		start = pos + oldToken.length();
	}
	while (pos != std::string::npos);

	return result;
}

} // namespace Poco::CppParser
