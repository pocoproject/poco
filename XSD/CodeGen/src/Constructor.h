//
// Constructor.h
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef CodeGen_Constructor_H_INCLUDED
#define CodeGen_Constructor_H_INCLUDED


#include "AbstractMethod.h"


class ClassInfo;


class Constructor: public AbstractMethod
{
public:
	Constructor(const ClassInfo& owner, Utility::Access acc);

	void addParameter(const Parameter& param);

	void addInitializationCode(const std::string& constrLine);

	void addInitializationCode(const std::vector<std::string>& constrLines);

	[[nodiscard]] const std::vector<std::string>& getInitializationCode() const;

private:
	std::vector<std::string>   _constrCode;
};


//
// inlines
//
inline void Constructor::addParameter(const Parameter& param)
{
	bool ok = _parameters.try_emplace(param.getOrder(), param).second;
	poco_assert (ok);
}


inline const std::vector<std::string>& Constructor::getInitializationCode() const
{
	return _constrCode;
}


#endif // CodeGen_Constructor_H_INCLUDED
