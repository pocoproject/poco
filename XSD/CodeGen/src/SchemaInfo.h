//
// SchemaInfo.h
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef CodeGen_SchemaInfo_H_INCLUDED
#define CodeGen_SchemaInfo_H_INCLUDED


#include "Poco/Path.h"
#include <string>
#include <vector>
#include <set>


class SchemaInfo
{
public:
	SchemaInfo(const std::string& id,
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
		bool timestamps);
		//Constructor

	[[nodiscard]] bool operator<(const SchemaInfo& si) const;

	[[nodiscard]] const std::string& id() const;
		/// The unqiue id for the schemaInfo, typically the xml namespace of the Schema

	[[nodiscard]] const std::string& nameSpace() const;
		/// Returns the cpp namespace. Can be empty

	[[nodiscard]] const std::string& dllPrefix() const;
		/// Returns the prefix that defines the declspec macro. Can be empty

	[[nodiscard]] const Poco::Path& includeDir() const;
		/// Returns the path where header files will be generated to.

	[[nodiscard]] const Poco::Path& sourceDir() const;
		/// Returns the path where source files will be generated to.

	[[nodiscard]] const Poco::Path& rootInclude() const;
		/// Returns the root path from which files will be included for this project.
		/// Must be a parent of includeDir

	[[nodiscard]] const std::vector<std::string>& copyright() const;
		/// Returns the copyright notice. Can be empty.

	[[nodiscard]] const std::set<std::string>& extraIncludes() const;
		/// Returns extra includes that each generated header file must include.
		/// Can be empty.

	[[nodiscard]] bool remotingAttributes() const;
		/// Returns whether remoting headers should be generated.
		
	[[nodiscard]] bool preserveOptional() const;
		/// Returns whether Poco::Optional should be used together with Poco::Nullable.
		
	[[nodiscard]] bool timestamps() const;
		/// Returns true whether header timestamps should be generated.

	[[nodiscard]] std::string createInclude(const std::string& className) const;

private:
	std::string              _id;
	std::string              _ns;
	std::string              _dllPrefix;
	Poco::Path               _includeDir;
	Poco::Path               _sourceDir;
	Poco::Path               _rootInclude;
	std::vector<std::string> _copyright;
	std::set<std::string>    _extraIncludes;
	std::string              _relativeInclude;
	bool                     _remotingAttributes;
	bool                     _preserveOptional;
	bool                     _nameSpaceInHeaderFileName;
	bool                     _timestamps;
};


//
// inlines
//
inline const std::string& SchemaInfo::id() const
{
	return _id;
}


inline const std::string& SchemaInfo::nameSpace() const
{
	return _ns;
}


inline const std::string& SchemaInfo::dllPrefix() const
{
	return _dllPrefix;
}


inline const Poco::Path& SchemaInfo::includeDir() const
{
	return _includeDir;
}


inline const Poco::Path& SchemaInfo::sourceDir() const
{
	return _sourceDir;
}


inline const Poco::Path& SchemaInfo::rootInclude() const
{
	return _rootInclude;
}


inline const std::vector<std::string>& SchemaInfo::copyright() const
{
	return _copyright;
}


inline const std::set<std::string>& SchemaInfo::extraIncludes() const
{
	return _extraIncludes;
}


inline bool SchemaInfo::remotingAttributes() const
{
	return _remotingAttributes;
}


inline bool SchemaInfo::preserveOptional() const
{
	return _preserveOptional;
}


inline bool SchemaInfo::timestamps() const
{
	return _timestamps;
}


inline bool SchemaInfo::operator<(const SchemaInfo& si) const
{
	return _id < si._id;
}


#endif // CodeGen_SchemaInfo_H_INCLUDED
