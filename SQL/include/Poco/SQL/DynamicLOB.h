//
// DynamicLOB.h
//
// Library: Data
// Package: DataCore
// Module:  DynamicLOB
//
// Definition of the Poco::Dynamic::Var LOB cast operators.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SQL_DynamicLOB_INCLUDED
#define SQL_DynamicLOB_INCLUDED


#include "Poco/SQL/SQL.h"
#include "Poco/SQL/LOB.h"
#include "Poco/Dynamic/Var.h"


namespace Poco {
namespace SQL {

template <typename T> class LOB;
typedef LOB<unsigned char> BLOB;
typedef LOB<char> CLOB;

} } // namespace Poco::SQL


namespace Poco {
namespace Dynamic {

template <> Poco_SQL_API Var::operator Poco::SQL::CLOB () const;
template <> Poco_SQL_API Var::operator Poco::SQL::BLOB () const;

} } // namespace Poco::Dynamic


#endif // Data_DynamicLOB_INCLUDED
