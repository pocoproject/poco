//
// PropertyFileConfiguration.cpp
//
// Library: Util
// Package: Configuration
// Module:  PropertyFileConfiguration
//
// Copyright (c) 2004-2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Util/PropertyFileConfiguration.h"
#include "Poco/Exception.h"
#include "Poco/String.h"
#include "Poco/Path.h"
#include "Poco/File.h"
#include "Poco/FileStream.h"
#include "Poco/LineEndingConverter.h"
#include "Poco/Ascii.h"
#include <set>
#include <sstream>
#include <string_view>

using Poco::trim;
using Poco::Path;


namespace Poco {
namespace Util {


PropertyFileConfiguration::PropertyFileConfiguration(AbstractConfiguration::Ptr pParentConfig):
	_pParentConfig(std::move(pParentConfig))
{
}


PropertyFileConfiguration::PropertyFileConfiguration(std::istream& istr, AbstractConfiguration::Ptr pParentConfig):
	_pParentConfig(std::move(pParentConfig))
{
	load(istr);
}


PropertyFileConfiguration::PropertyFileConfiguration(const std::string& path, AbstractConfiguration::Ptr pParentConfig):
	_pParentConfig(std::move(pParentConfig))
{
	load(path);
}


void PropertyFileConfiguration::load(std::istream& istr)
{
	AbstractConfiguration::ScopedLock lock(*this);

	clear();
	_sourceMap.clear();
	_rootFile.clear();
	std::set<std::string> includeStack;
	loadStream(istr, "", "", includeStack);
}


void PropertyFileConfiguration::load(const std::string& path)
{
	Poco::Path p(path);
	p.makeAbsolute();
	std::string absPath = p.toString();
	std::string basePath = p.parent().toString();
	Poco::FileInputStream istr(path);
	if (!istr.good())
		throw Poco::OpenFileException(path);
	AbstractConfiguration::ScopedLock lock(*this);
	clear();
	_sourceMap.clear();
	_rootFile = absPath;
	std::set<std::string> includeStack;
	includeStack.insert(absPath);
	loadStream(istr, basePath, absPath, includeStack);
}


void PropertyFileConfiguration::loadStream(std::istream& istr, const std::string& basePath, const std::string& currentFile, std::set<std::string>& includeStack)
{
	for (;;)
	{
		if (!istr.good())
		{
			if (istr.eof())
				break;
			throw Poco::IOException("Broken input stream");
		}
		parseLine(istr, basePath, currentFile, includeStack);
	}
}


void PropertyFileConfiguration::save(std::ostream& ostr) const
{
	AbstractConfiguration::ScopedLock lock(*this);

	MapConfiguration::iterator it = begin();
	MapConfiguration::iterator ed = end();
	while (it != ed)
	{
		ostr << it->first << ": ";
		for (auto ch: it->second)
		{
			switch (ch)
			{
			case '\t':
				ostr << "\\t";
				break;
			case '\r':
				ostr << "\\r";
				break;
			case '\n':
				ostr << "\\n";
				break;
			case '\f':
				ostr << "\\f";
				break;
			case '\\':
				ostr << "\\\\";
				break;
			default:
				ostr << ch;
				break;
			}
		}
		ostr << "\n";
		++it;
	}
}


void PropertyFileConfiguration::save(const std::string& path) const
{
	AbstractConfiguration::ScopedLock lock(*this);

	if (_sourceMap.empty())
	{
		// No provenance — write flat key-value pairs
		std::set<std::string> allKeys;
		MapConfiguration::iterator it = begin();
		MapConfiguration::iterator ed = end();
		while (it != ed) { allKeys.insert(it->first); ++it; }
		saveToFile(path, allKeys);
		return;
	}

	// Group keys by source file
	std::map<std::string, std::set<std::string>> fileKeys;
	for (const auto& [key, file] : _sourceMap)
	{
		fileKeys[file].insert(key);
	}

	// Find keys with no provenance (newly added) — assign to root file
	Poco::Path rootPath(path);
	rootPath.makeAbsolute();
	std::string rootFile = _rootFile.empty() ? rootPath.toString() : _rootFile;

	MapConfiguration::iterator it = begin();
	MapConfiguration::iterator ed = end();
	while (it != ed)
	{
		if (_sourceMap.find(it->first) == _sourceMap.end())
		{
			fileKeys[rootFile].insert(it->first);
		}
		++it;
	}

	// Save each file
	for (const auto& [file, keys] : fileKeys)
	{
		saveToFile(file, keys);
	}
}


void PropertyFileConfiguration::saveToFile(const std::string& path, const std::set<std::string>& keys) const
{
	std::set<std::string> written;
	std::ostringstream out;

	Poco::File f(path);
	if (f.exists())
	{
		Poco::FileInputStream istr(path);
		std::string line;
		while (std::getline(istr, line))
		{
			// Remove trailing \r if present
			if (!line.empty() && line.back() == '\r')
				line.pop_back();

			// Blank line
			if (line.empty())
			{
				out << "\n";
				continue;
			}

			// Skip leading whitespace for detection
			std::string::size_type pos = 0;
			while (pos < line.size() && Poco::Ascii::isSpace(line[pos])) ++pos;

			// Comment or !include
			if (pos < line.size() && (line[pos] == '#' || line[pos] == '!'))
			{
				out << line << "\n";
				continue;
			}

			// Key-value line: extract key
			std::string::size_type sep = line.find('=');
			std::string::size_type sep2 = line.find(':');
			if (sep2 != std::string::npos && (sep == std::string::npos || sep2 < sep))
				sep = sep2;

			if (sep != std::string::npos)
			{
				std::string key = Poco::trim(line.substr(0, sep));
				if (keys.count(key) && has(key))
				{
					std::string rawValue;
					getRaw(key, rawValue);
					char sepChar = line[sep];
					out << key << " " << sepChar << " " << escapeValue(rawValue) << "\n";
					written.insert(key);
				}
				// Key not in config (removed) — skip line
			}
			else
			{
				// Line with no separator — preserve as-is
				out << line << "\n";
			}
		}
	}

	// Append new keys not yet written
	for (const auto& key : keys)
	{
		if (!written.count(key) && has(key))
		{
			std::string rawValue;
			getRaw(key, rawValue);
			out << key << ": " << escapeValue(rawValue) << "\n";
		}
	}

	Poco::FileOutputStream ostr(path);
	if (!ostr.good()) throw Poco::CreateFileException(path);
	Poco::OutputLineEndingConverter lec(ostr);
	lec << out.str();
	lec.flush();
	ostr.flush();
	if (!ostr.good()) throw Poco::WriteFileException(path);
}


std::string PropertyFileConfiguration::escapeValue(const std::string& value)
{
	std::string result;
	for (auto ch : value)
	{
		switch (ch)
		{
		case '\t': result += "\\t"; break;
		case '\r': result += "\\r"; break;
		case '\n': result += "\\n"; break;
		case '\f': result += "\\f"; break;
		case '\\': result += "\\\\"; break;
		default: result += ch; break;
		}
	}
	return result;
}


std::string PropertyFileConfiguration::getSourceFile(const std::string& key) const
{
	auto it = _sourceMap.find(key);
	if (it != _sourceMap.end())
		return it->second;
	return "";
}


void PropertyFileConfiguration::parseLine(std::istream& istr, const std::string& basePath, const std::string& currentFile, std::set<std::string>& includeStack)
{
	constexpr int eof = std::char_traits<char>::eof();

	int c = istr.get();
	while (c != eof && Poco::Ascii::isSpace(c)) c = istr.get();
	if (c != eof)
	{
		if (c == '#')
		{
			while (c != eof && c != '\n' && c != '\r') c = istr.get();
		}
		else if (c == '!')
		{
			std::string line;
			line += static_cast<char>(c);
			while (c != eof && c != '\n' && c != '\r')
			{
				c = istr.get();
				if (c != eof && c != '\n' && c != '\r')
					line += static_cast<char>(c);
			}

			constexpr std::string_view includeDirective = "!include";
			if (line.size() > includeDirective.size() &&
				line.compare(0, includeDirective.size(), includeDirective) == 0 &&
				Poco::Ascii::isSpace(line[includeDirective.size()]))
			{
				std::string includePath = Poco::trim(line.substr(includeDirective.size()));
				if (includePath.empty())
					throw Poco::SyntaxException("Missing path in !include directive");
				includePath = expand(includePath);
				if (_pParentConfig)
					includePath = _pParentConfig->expand(includePath);
				Poco::Path p(includePath);
				if (p.isRelative() && !basePath.empty())
					p = Poco::Path(basePath).resolve(p);
				p.makeAbsolute();
				const std::string absPathStr = p.toString();

				if (includeStack.find(absPathStr) != includeStack.end())
				{
					throw Poco::FileException("Cyclic property file include detected", absPathStr);
				}

				struct StackGuard
				{
					std::set<std::string>& stack;
					const std::string& key;
					StackGuard(std::set<std::string>& s, const std::string& k): stack(s), key(k) { stack.insert(k); }
					~StackGuard() { stack.erase(key); }
				} guard(includeStack, absPathStr);

				Poco::FileInputStream includeIstr(p.toString());
				if (!includeIstr.good())
					throw Poco::OpenFileException(p.toString());
				loadStream(includeIstr, p.parent().toString(), absPathStr, includeStack);
			}
		}
		else
		{
			std::string key;
			while (c != eof && c != '=' && c != ':' && c != '\r' && c != '\n') { key += (char) c; c = istr.get(); }
			std::string value;
			if (c == '=' || c == ':')
			{
				c = readChar(istr);
				while (c != eof && c) { value += (char) c; c = readChar(istr); }
			}
				std::string trimmedKey = trim(key);
			setRaw(trimmedKey, trim(value));
			if (!currentFile.empty())
				_sourceMap[trimmedKey] = currentFile;
		}
	}
}


int PropertyFileConfiguration::readChar(std::istream& istr)
{
	for (;;)
	{
		int c = istr.get();
		if (c == '\\')
		{
			c = istr.get();
			switch (c)
			{
			case 't':
				return '\t';
			case 'r':
				return '\r';
			case 'n':
				return '\n';
			case 'f':
				return '\f';
			case '\r':
				if (istr.peek() == '\n')
					istr.get();
				continue;
			case '\n':
				continue;
			default:
				return c;
			}
		}
		else if (c == '\n' || c == '\r')
			return 0;
		else
			return c;
	}
}


} } // namespace Poco::Util
