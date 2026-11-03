//
// Utility.h
//
// Library: CppParser
// Package: CppParser
// Module:  Utility
//
// Definition of the Utility class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef CppParser_Utility_INCLUDED
#define CppParser_Utility_INCLUDED


#include "Poco/CppParser/CppParser.h"
#include "Poco/CppParser/NameSpace.h"
#include <vector>
#include <set>


namespace Poco::CppParser {


class CppParser_API Utility
	/// Various helpers for parsing and analyzing C++ header files.
{
public:
	class CppParser_API FwdDeclBlock
	{
	public:
		std::string beginNameSpaceDecl;      // contains either $(NS)_BEGIN or the namespace x { decl
		std::string endNameSpaceDecl;        // contains either $(NS)_END or the closing brackets }
		std::vector<std::string> classDecls; // contains strings of the form "class X;"
	};

	static void parse(const std::string& file, NameSpace::SymbolTable& st, const std::string& exec, const std::string& options, const std::string& path);
		/// Preprocesses and parses the file. The resulting symboltable has base class references already fixed,

	static void parseDir(const std::vector <std::string>& includePattern, const std::vector <std::string>& excludePattern, NameSpace::SymbolTable& st, const std::string& exec, const std::string& options, const std::string& path);
		/// Preprocesses and parses all files specified by the include pattern (e.g.: p:/poco/Foundation/include/*/*.h) minus the ones defined in the exclude pattern

	static void collectDependencies(const std::vector<std::string>& includePattern,
		const std::vector<std::string>& excludePattern,
		std::set<std::string>& files,
		std::set<std::string>& directories);
		/// Inserts into files the absolute paths of the files that parseDir() parses for the given patterns,
		/// and into directories the absolute paths of the directories whose entries decide that list.
		/// A searched directory is listed where the pattern component has a wildcard, or no entry of that name
		/// resolves to a directory (for the last component of a pattern that names a file, exists); for a
		/// symbolic link or junction at a directory component that does not resolve to a directory, the deepest
		/// existing directory on its target path is listed too, as far as that path can be followed. On Windows,
		/// where the search ignores the letter case, every searched directory is listed.
		///
		/// Note: Where the file system ignores the letter case or the Unicode normalization of names and the
		/// search does not, as on macOS, a rename that changes only the case or normalization of a name that
		/// a pattern spells literally can change the file list without changing a listed file or directory.
		/// So can a new target of a link before a ".." component in the target path of a link that does not
		/// resolve to a directory. So can replacing a directory that a pattern or a link target names, by a
		/// rename or a new link target, with one whose timestamp is older.

	static void fixup(NameSpace::SymbolTable& st);
		/// Fixes all base pointers in the symbol table

	static void detectPrefixAndIncludes(const std::string& origHFile, std::vector<std::string>& lines, std::string& prefix);
		/// This method is poco coding style specific! It looks for a $(PREFIX)_BEGIN and extracts from it a prefix, also include files and fwd declarations are extracted from the h file.

	static void removeFile(const std::string& preprocessedfile);
		/// Tries to remove the file. If it fails, the error is silently ignored.

protected:
	static std::string preprocessFile(const std::string& file, const std::string& exec, const std::string& options, const std::string& path);
		/// Preprocess the include file with name file. Parameter exec must contain the name of the preprocessor binary (e.g.: "cl" for Visual Studio).
		/// Parameter options contains the flag for the preprocessor, and parameter path sets the environment PATH settings during preprocessing.
		/// Returns the name of the created file or throws an exception.

	static void parseOnly(const std::string& file, NameSpace::SymbolTable& st, const std::string& preprocessedFile, bool removePreprocessedFile = true);
		/// Parses the file, throws an exception if anything goes wrong.

	static void buildFileList(std::set<std::string>& files, const std::vector<std::string>& includePattern, const std::vector<std::string>& excludePattern);
		/// Searches all files that match the defined patterns and inserts them into files.
private:
	Utility();
	~Utility();
	Utility(const Utility&);
	Utility& operator=(const Utility&);

};


std::string CppParser_API replace(const std::string& input, const std::string& oldToken, const std::string& newToken);
	/// Replaces in character input all oldTokens with the newToken


} // namespace Poco::CppParser


#endif // CppParser_Utility_INCLUDED
