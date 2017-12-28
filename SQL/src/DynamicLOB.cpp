//
// DynamicLOB.cpp
//
// Library: Data
// Package: DataCore
// Module:  DynamicLOB
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifdef __GNUC__
// TODO: determine g++ version able to do the right thing without these specializations

#include "Poco/SQL/DynamicLOB.h"
#include "Poco/SQL/LOB.h"
#include "Poco/Dynamic/Var.h"


namespace Poco {
namespace Dynamic {


using Poco::SQL::CLOB;
using Poco::SQL::BLOB;


template <>
Var::operator CLOB () const
{
	VarHolder* pHolder = content();

	if (!pHolder)
		throw InvalidAccessException("Can not convert empty value.");

	if (typeid(CLOB) == pHolder->type())
		return extract<CLOB>();
	else
	{
		std::string result;
		pHolder->convert(result);
		return CLOB(result);
	}
}


template <>
Var::operator BLOB () const
{
	VarHolder* pHolder = content();

	if (!pHolder)
		throw InvalidAccessException("Can not convert empty value.");

	if (typeid(BLOB) == pHolder->type())
		return extract<BLOB>();
	else
	{
		std::string result;
		pHolder->convert(result);
		return BLOB(reinterpret_cast<const unsigned char*>(result.data()),
			result.size());
	}
}


} } // namespace Poco::SQL


#endif // __GNUC__

