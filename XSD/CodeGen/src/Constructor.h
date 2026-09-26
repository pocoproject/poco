//
// Constructor.h
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef CodeGen_Constructor_H_INCLUDED
#define CodeGen_Constructor_H_INCLUDED


#include "AbstractMethod.h"


class ClassInfo;


class Constructor: public AbstractMethod
{
public:
	Constructor(const ClassInfo& owner, Utility::Access acc);

	virtual ~Constructor();

	void addParameter(const Parameter& param);

	void addInitializationCode(const std::string& constrLine);

	void addInitializationCode(const std::vector<std::string>& constrLines);

	const std::vector<std::string>& getInitializationCode() const;

private:
	std::vector<std::string>   _constrCode;
};


//
// inlines
//
inline void Constructor::addParameter(const Parameter& param)
{
	bool ok = _parameters.insert(std::make_pair(param.getOrder(), param)).second;
	poco_assert (ok);
}


inline const std::vector<std::string>& Constructor::getInitializationCode() const
{
	return _constrCode;
}


#endif // CodeGen_Constructor_H_INCLUDED
