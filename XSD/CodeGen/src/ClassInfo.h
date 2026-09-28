//
// ClassInfo.h
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef CodeGen_ClassInfo_H_INCLUDED
#define CodeGen_ClassInfo_H_INCLUDED


#include "Variable.h"
#include "MethodInfo.h"
#include "TypeInfo.h"
#include "Constructor.h"
#include "Destructor.h"
#include "PropertyHolder.h"
#include <vector>
#include <map>
#include <set>


class ClassInfo: public PropertyHolder
{
public:
	ClassInfo(const std::string& name, 
		const std::string& nameSpace, 
		const std::string& schemaNameSpace,
		const std::string& includeFile, 
		const std::string& dllExportMacro);

	[[nodiscard]] const std::string& name() const;

	[[nodiscard]] const std::string& getNameSpace() const;
	
	[[nodiscard]] const std::string& getSchemaNameSpace() const;

	[[nodiscard]] const std::string& getIncludeFile() const;

	[[nodiscard]] bool isSystemInclude() const;

	[[nodiscard]] const TypeInfo& getTypeInfo() const;

	void setIncludeFile(const std::string& incFile, bool isSystemInclude);

	void addVariable(const Variable& var);

	void addMethod(const MethodInfo& method);

	void addConstructor(const Constructor& constr);

	void setDestructor(Utility::Access acc, bool isVirtual);

	[[nodiscard]] const std::map<int, Variable>& getVariables() const;

	[[nodiscard]] const std::multimap<std::string, MethodInfo>& getMethods() const;

	[[nodiscard]] const std::vector<Constructor>& getConstructors() const;

	[[nodiscard]] std::vector<Constructor>& getConstructors();

	[[nodiscard]] const Destructor& getDestructor() const;

	[[nodiscard]] Destructor& getDestructor();

	[[nodiscard]] const std::string& getDllExportMacro() const;

	void setParent(const TypeInfo& parent);

	[[nodiscard]] const TypeInfo& getParent() const;

	void addSrcInclude(const std::string& file, bool isSystemInclude);

	[[nodiscard]] const std::set<std::string>& getSrcIncludes() const;

	[[nodiscard]] const std::set<std::string>& getSrcSystemIncludes() const;

	void addFwdDeclare(const std::string& className, const std::string& nameSpace, const std::string& include);

	[[nodiscard]] const std::set<std::string>& getForwardDeclarations() const;

private:
	std::map<int, Variable> _variables;
	std::multimap<std::string, MethodInfo> _methods;
	std::vector<Constructor> _constructors;
	Destructor _destructor;
	TypeInfo    _info;
	TypeInfo    _parent;
	std::string _dllExportMacro;
	std::set<std::string> _srcIncludes;
	std::set<std::string> _srcSystemIncludes;
	std::set<std::string> _fwdDeclarations;
};


//
// inlines
//
inline const std::string& ClassInfo::name() const
{
	return _info.name();
}


inline const std::string& ClassInfo::getNameSpace() const
{
	return _info.getNameSpace();
}


inline const std::string& ClassInfo::getSchemaNameSpace() const
{
	return _info.getSchemaNameSpace();
}


inline const TypeInfo& ClassInfo::getTypeInfo() const
{
	return _info;
}


inline const std::string& ClassInfo::getIncludeFile() const
{
	return _info.getIncludeFile();
}


inline bool ClassInfo::isSystemInclude() const
{
	return _info.isSystemInclude();
}


inline void ClassInfo::setIncludeFile(const std::string& incFile, bool isSystemInclude)
{
	_info.setIncludeFile(incFile, isSystemInclude);
}


inline void ClassInfo::addVariable(const Variable& var)
{
	bool ok = _variables.try_emplace(var.getOrder(), var).second;
	poco_assert (ok);
}


inline void ClassInfo::addMethod(const MethodInfo& method)
{
	_methods.emplace(method.name(), method);
}


inline const std::map<int, Variable>& ClassInfo::getVariables() const
{
	return _variables;
}


inline const std::multimap<std::string, MethodInfo>& ClassInfo::getMethods() const
{
	return _methods;
}


inline const std::string& ClassInfo::getDllExportMacro() const
{
	return _dllExportMacro;
}


inline void ClassInfo::setParent(const TypeInfo& parent)
{
	_parent = parent;
}


inline const TypeInfo& ClassInfo::getParent() const
{
	return _parent;
}


inline const std::vector<Constructor>& ClassInfo::getConstructors() const
{
	return _constructors;
}


inline std::vector<Constructor>& ClassInfo::getConstructors()
{
	return _constructors;
}


inline const Destructor& ClassInfo::getDestructor() const
{
	return _destructor;
}


inline Destructor& ClassInfo::getDestructor()
{
	return _destructor;
}


inline void ClassInfo::addSrcInclude(const std::string& file, bool isSystemInclude)
{
	if (isSystemInclude) _srcSystemIncludes.insert(file);
	else _srcIncludes.insert(file);
}


inline const std::set<std::string>& ClassInfo::getSrcIncludes() const
{
	return _srcIncludes;
}


inline const std::set<std::string>& ClassInfo::getSrcSystemIncludes() const
{
	return _srcSystemIncludes;
}


inline const std::set<std::string>& ClassInfo::getForwardDeclarations() const
{
	return _fwdDeclarations;
}


#endif // CodeGen_ClassInfo_H_INCLUDED
