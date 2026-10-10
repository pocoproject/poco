//
// MethodInfo.h
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef CodeGen_MethodInfo_H_INCLUDED
#define CodeGen_MethodInfo_H_INCLUDED


#include "AbstractMethod.h"


class MethodInfo: public AbstractMethod
{
public:
	MethodInfo(const std::string& name, Utility::Access acc, bool isConst, bool isStatic, bool isVirtual, bool isAbstract);

	void setReturnParameter(Poco::SharedPtr<Parameter> pParam);
		/// Sets the return parameter, set to null for void return.

	void addParameter(const Parameter& param);
		/// Adds a parameter to the method.
};


//
// inlines
//
inline void MethodInfo::setReturnParameter(Poco::SharedPtr<Parameter> pParam)
{
	_pReturn = pParam;
}


inline void MethodInfo::addParameter(const Parameter& param)
{
	bool ok = _parameters.try_emplace(param.getOrder(), param).second;
	poco_assert (ok);
}


#endif // CodeGen_MethodInfo_H_INCLUDED
