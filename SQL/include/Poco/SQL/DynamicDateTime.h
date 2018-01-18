//
// DynamicDateTime.h
//
// Library: Data
// Package: DataCore
// Module:  DynamicDateTime
//
// Definition of the Date and Time cast operators for Poco::Dynamic::Var.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SQL_DynamicDateTime_INCLUDED
#define SQL_DynamicDateTime_INCLUDED


#include "Poco/SQL/SQL.h"
#include "Poco/SQL/Date.h"
#include "Poco/SQL/Time.h"
#include "Poco/Dynamic/Var.h"


namespace Poco {
namespace SQL {

class Date;
class Time;

} } // namespace Poco::SQL


namespace Poco {
namespace Dynamic {


template <> Poco_SQL_API Var::operator Poco::SQL::Date () const;
template <> Poco_SQL_API Var::operator Poco::SQL::Time () const;


} } // namespace Poco::Dynamic


#endif // Data_DynamicDateTime_INCLUDED
