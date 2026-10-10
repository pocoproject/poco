//
// TypeInfo.h
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef CodeGen_TypeInfo_H_INCLUDED
#define CodeGen_TypeInfo_H_INCLUDED


#include <string>


class TypeInfo
{
public:
	TypeInfo();
		// NULL typeinfo

	TypeInfo(const std::string& name, const std::string& nameSpace, const std::string& schemaNameSpace, const std::string& xsdType, const std::string& includeFile, bool isSystemInclude, bool isVector, bool isScalar);

	[[nodiscard]] const std::string& name() const;

	[[nodiscard]] const std::string& getNameSpace() const;
		/// Returns the cpp namespace
		
	[[nodiscard]] const std::string& getSchemaNameSpace() const;
		/// Returns the schema namespace.

	[[nodiscard]] const std::string& getFullName() const;

	[[nodiscard]] const std::string& getIncludeFile() const;

	void setIncludeFile(const std::string& incFile, bool isSystemInclude);

	[[nodiscard]] bool isSystemInclude() const;

	[[nodiscard]] bool isVector() const;

	void setVector(bool isVec);
	
	[[nodiscard]] bool isNullable() const;
	
	void setNullable(bool isNullable);

	[[nodiscard]] bool isScalar() const;

	[[nodiscard]] const std::string& xsdType() const;

	[[nodiscard]] bool operator < (const TypeInfo& other) const;
		/// comnpares two types by name and namespace

private:
	std::string _name;
	std::string _nameSpace;
	std::string _schemaNameSpace;
	std::string _fullName;
	std::string _xsdType;
	std::string _includeFile;
	bool        _isSystemInclude;
	bool        _isVector;
	bool        _isNullable = false;
	bool        _isScalar;
};


//
// inlines
//
inline const std::string& TypeInfo::name() const
{
	return _name;
}


inline const std::string& TypeInfo::getNameSpace() const
{
	return _nameSpace;
}


inline const std::string& TypeInfo::getSchemaNameSpace() const
{
	return _schemaNameSpace;
}


inline const std::string& TypeInfo::getIncludeFile() const
{
	return _includeFile;
}


inline const std::string& TypeInfo::getFullName() const
{
	return _fullName;
}


inline void TypeInfo::setIncludeFile(const std::string& incFile, bool isSystemInclude)
{
	_includeFile = incFile;
}


inline bool TypeInfo::isSystemInclude() const
{
	return _isSystemInclude;
}


inline bool TypeInfo::isVector() const
{
	return _isVector;
}


inline void TypeInfo::setVector(bool isVec)
{
	_isVector = isVec;
}


inline bool TypeInfo::isNullable() const
{
	return _isNullable;
}


inline void TypeInfo::setNullable(bool isNullable)
{
	_isNullable = isNullable;
}


inline bool TypeInfo::isScalar() const
{
	return _isScalar;
}


inline const std::string& TypeInfo::xsdType() const
{
	return _xsdType;
}


inline bool TypeInfo::operator < (const TypeInfo& other) const
{
	if (_name == other._name)
		return _nameSpace < other._nameSpace;
	return _name < other._name;
}


#endif // CodeGen_TypeInfo_H_INCLUDED
