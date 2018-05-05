//
// RowFormatter.cpp
//
// Library: Data
// Package: DataCore
// Module:  RowFormatter
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/SQL/RowFormatter.h"
#include <iomanip>


namespace Poco {
namespace SQL {


RowFormatter::RowFormatter(const std::string& rPrefix,
	const std::string& rPostfix,
	Mode mode):
	_prefix(rPrefix),
	_postfix(rPostfix),
	_mode(mode),
	_totalRowCount(0)
{
}


RowFormatter::~RowFormatter()
{
}


std::string& RowFormatter::formatNames(const NameVecPtr pNames, std::string& formattedNames)
{
	formattedNames.clear();
	return formattedNames;
}


void RowFormatter::formatNames(const NameVecPtr pNames)
{
	return;
}


std::string& RowFormatter::formatValues(const ValueVec& /*vals*/, std::string& formattedValues)
{
	formattedValues.clear();
	return formattedValues;
}


void RowFormatter::formatValues(const ValueVec& /*vals*/)
{
	return;
}


const std::string& RowFormatter::toString()
{
	throw NotImplementedException("RowFormatter::toString()");
}


void RowFormatter::reset()
{
	_prefix = "";
	_postfix = "";
	_totalRowCount = INVALID_ROW_COUNT;
}


} } // namespace Poco::SQL
