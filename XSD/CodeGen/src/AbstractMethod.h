//
// AbstractMethod.h
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef CodeGen_AbstractMethod_H_INCLUDED
#define CodeGen_AbstractMethod_H_INCLUDED


#include "Parameter.h"
#include "PropertyHolder.h"
#include "Utility.h"
#include "Poco/SharedPtr.h"
#include <map>
#include <vector>


class AbstractMethod: public PropertyHolder
{
public:
	AbstractMethod(const std::string& name, Utility::Access acc, bool isConst, bool isStatic, bool isVirtual, bool isAbstract);

	virtual ~AbstractMethod();

	Utility::Access getAccess() const;

	void setAccess(Utility::Access acc);

	Poco::SharedPtr<Parameter> getReturnParameter() const;
		/// Returns the return parameter which can be null.

	const std::string& name() const;

	const std::map<int, Parameter>& getParameters() const;

	bool isConst() const;

	bool isStatic() const;

	bool isVirtual() const;

	bool isAbstract() const;

	void addCode(const std::string& line);
		/// Adds a single code line, strips leading/trailing whitechars

	void addCode(const std::vector<std::string>& lines);
		/// Adds a sequence of code lines

	void addDocu(const std::string& line);
		/// Adds a single line of documentation

	void addDocu(const std::vector<std::string>& line);
		/// Adds several lines of documentation

	const std::vector<std::string>& getDocu() const;
		/// Returns the documentation

	const std::vector<std::string>& getCode() const;
		/// Returns the code

protected:
	std::string _name;
	Utility::Access _access;
	bool        _isConst;
	bool        _isStatic;
	bool        _isVirtual;
	bool        _isAbstract;
	std::map<int, Parameter>   _parameters;
	Poco::SharedPtr<Parameter> _pReturn;
	std::vector<std::string>   _code;
	std::vector<std::string>   _docu;
};


//
// inlines
//
inline Utility::Access AbstractMethod::getAccess() const
{
	return _access;
}


inline void AbstractMethod::setAccess(Utility::Access acc)
{
	_access = acc;
}


inline Poco::SharedPtr<Parameter> AbstractMethod::getReturnParameter() const
{
	return _pReturn;
}


inline const std::string& AbstractMethod::name() const
{
	return _name;
}


inline const std::map<int, Parameter>& AbstractMethod::getParameters() const
{
	return _parameters;
}


inline bool AbstractMethod::isConst() const
{
	return _isConst;
}


inline bool AbstractMethod::isStatic() const
{
	return _isStatic;
}


inline bool AbstractMethod::isVirtual() const
{
	return _isVirtual;
}


inline bool AbstractMethod::isAbstract() const
{
	return _isAbstract;
}


inline const std::vector<std::string>& AbstractMethod::getDocu() const
{
	return _docu;
}


inline const std::vector<std::string>& AbstractMethod::getCode() const
{
	return _code;
}


#endif // CodeGen_AbstractMethod_H_INCLUDED
