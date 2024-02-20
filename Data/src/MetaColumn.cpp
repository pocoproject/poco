//
// MetaColumn.cpp
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


MetaColumn::MetaColumn(std::size_t position,
	const std::string& name,
	ColumnDataType type,
	std::size_t length,
	std::size_t precision,
	bool nullable):
	_name(name),
	_length(length),
	_precision(precision),
	_position(position),
	_type(type),
	_nullable(nullable)
{
}


void MetaColumn::swap(MetaColumn& other) noexcept
{
	std::swap(_name, other._name);
	std::swap(_length, other._length);
	std::swap(_precision, other._precision);
	std::swap(_position, other._position);
	std::swap(_type, other._type);
	std::swap(_nullable, other._nullable);
}


} } // namespace Poco::Data
