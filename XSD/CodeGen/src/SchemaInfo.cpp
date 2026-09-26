//
// SchemaInfo.cpp
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "SchemaInfo.h"
#include "Poco/String.h"


SchemaInfo::SchemaInfo(const std::string& id,
		const std::string& ns, 
		const std::string& dllprefix, 
		const Poco::Path& includeDir, 
		const Poco::Path& sourceDir, 
		const Poco::Path& rootInclude, 
		const std::vector<std::string>& copyright, 
		const std::set<std::string>& extraIncludes,
		bool remotingAttributes,
		bool preserveOptional,
		bool nameSpaceInHeaderFileName,
		bool timestamps):
	_id(id),
	_ns(ns),
	_dllPrefix(dllprefix),
	_includeDir(includeDir),
	_sourceDir(sourceDir),
	_rootInclude(rootInclude),
	_copyright(copyright),
	_extraIncludes(extraIncludes),
	_remotingAttributes(remotingAttributes),
	_preserveOptional(preserveOptional),
	_nameSpaceInHeaderFileName(nameSpaceInHeaderFileName),
	_timestamps(timestamps)
{
	_includeDir.makeDirectory();
	_includeDir.makeAbsolute();
	_sourceDir.makeDirectory();
	_sourceDir.makeAbsolute();
	_rootInclude.makeDirectory();
	_rootInclude.makeAbsolute();
	Poco::Path incDir (_includeDir);
	incDir.makeDirectory();
	incDir.makeAbsolute();
	Poco::Path rootDir (_rootInclude);
	rootDir.makeDirectory();
	rootDir.makeAbsolute();
	std::string rootStr = rootDir.toString();
	std::string childStr = incDir.toString();
	std::string::size_type pos = childStr.find(rootStr);
	poco_assert (pos == 0);
	_relativeInclude = childStr.substr(rootStr.size());
}


SchemaInfo::~SchemaInfo()
{
}


std::string SchemaInfo::createInclude(const std::string& className) const
{
 	Poco::Path incFile(_relativeInclude);
	incFile.makeDirectory();

	if (_nameSpaceInHeaderFileName)
	{
		std::string name = nameSpace();
		Poco::replaceInPlace(name, std::string("::"), std::string("_"));
		name += "_";
		name += className;
		incFile.setBaseName(name);
	}
	else
	{
		incFile.setBaseName(className);
	}
	incFile.setExtension("h");
	return incFile.toString(Poco::Path::PATH_UNIX);
}
