//
// Date.cpp
//
// Library: Data
// Package: DataCore
// Module:  Date
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/SQL/Date.h"
#include "Poco/DateTime.h"
#include "Poco/NumberFormatter.h"
#include "Poco/SQL/DynamicDateTime.h"
#include "Poco/Dynamic/Var.h"


using Poco::DateTime;
using Poco::Dynamic::Var;
using Poco::NumberFormatter;


namespace Poco {
namespace SQL {


Date::Date()
{
	DateTime dt;
	assign(dt.year(), dt.month(), dt.day());
}


Date::Date(int dateYear, int dateMonth, int dateDay)
{
	assign(dateYear, dateMonth, dateDay);
}


Date::Date(const DateTime& dt)
{
	assign(dt.year(), dt.month(), dt.day());
}


Date::~Date()
{
}


void Date::assign(int dateYear, int dateMonth, int dateDay)
{
	if (dateYear < 0 || dateYear > 9999)
		throw InvalidArgumentException("Year must be between 0 and 9999");

	if (dateMonth < 1 || dateMonth > 12)
		throw InvalidArgumentException("Month must be between 1 and 12");

	if (dateDay < 1 || dateDay > DateTime::daysOfMonth(dateYear, dateMonth))
		throw InvalidArgumentException("Month must be between 1 and " +
			NumberFormatter::format(DateTime::daysOfMonth(dateYear, dateMonth)));

	_year = dateYear;
	_month = dateMonth;
	_day = dateDay;
}


bool Date::operator < (const Date& date) const
{
	int dateYear = date.year();

	if (_year < dateYear) return true;
	else if (_year > dateYear) return false;
	else // years equal
	{
		int dateMonth = date.month();
		if (_month < dateMonth) return true;
		else
		if (_month > dateMonth) return false;
		else // months equal
		if (_day < date.day()) return true;
	}

	return false;
}


Date& Date::operator = (const Var& var)
{
#ifndef __GNUC__
// g++ used to choke on this, newer versions seem to digest it fine
// TODO: determine the version able to handle it properly
	*this = var.extract<Date>();
#else
	*this = var.operator Date();
#endif
	return *this;
}


} } // namespace Poco::SQL


#ifdef __GNUC__
// only needed for g++ (see comment in Date::operator = above)

namespace Poco {
namespace Dynamic {


using Poco::SQL::Date;
using Poco::DateTime;


template <>
Var::operator Date () const
{
	VarHolder* pHolder = content();

	if (!pHolder)
		throw InvalidAccessException("Can not convert empty value.");

	if (typeid(Date) == pHolder->type())
		return extract<Date>();
	else
	{
		Poco::DateTime result;
		pHolder->convert(result);
		return Date(result);
	}
}


} } // namespace Poco::Dynamic


#endif // __GNUC__
