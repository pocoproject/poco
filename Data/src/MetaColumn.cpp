//
// MetaColumn.cpp
//
// $Id: //poco/Main/Data/src/MetaColumn.cpp#2 $
//
// Library: Data
// Package: DataCore
// Module:  MetaColumn
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Data/MetaColumn.h"


namespace Poco {
namespace Data {


MetaColumn::MetaColumn():
	_length(),
	_precision(),
	_position(),
	_type(),
	_nullable()
{
}


MetaColumn::MetaColumn(std::size_t columnPosition,
	const std::string& rName,
	ColumnDataType columnType,
	std::size_t columnLength,
	std::size_t columnPrecision,
	bool nullable): 
	_name(rName),
	_length(columnLength),
	_precision(columnPrecision),
	_position(columnPosition),
	_type(columnType),
	_nullable(nullable)
{
}


MetaColumn::~MetaColumn()
{
}


} } // namespace Poco::Data
