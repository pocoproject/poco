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


MetaColumn::MetaColumn():
	_length(),
	_precision(),
	_position(),
	_type(),
	_nullable()
{
}


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


MetaColumn::MetaColumn(const MetaColumn& other):
	_name(other._name),
	_length(other._length),
	_precision(other._precision),
	_position(other._position),
	_type(other._type),
	_nullable(other._nullable)
{
}


MetaColumn::MetaColumn(MetaColumn&& other) noexcept:
	_name(std::move(other._name)),
	_length(other._length),
	_precision(other._precision),
	_position(other._position),
	_type(other._type),
	_nullable(other._nullable)
{
}


MetaColumn::~MetaColumn()
{
}


MetaColumn& MetaColumn::operator = (const MetaColumn& other)
{
	MetaColumn tmp(other);
	swap(tmp);
	return *this;
}


MetaColumn& MetaColumn::operator = (MetaColumn&& other) noexcept
{
	_name = std::move(other._name);
	_length = other._length;
	_precision = other._precision;
	_position = other._position;
	_type = other._type;
	_nullable = other._nullable;
	return *this;
}


void MetaColumn::swap(MetaColumn& other)
{
	std::swap(_name, other._name);
	std::swap(_length, other._length);
	std::swap(_precision, other._precision);
	std::swap(_position, other._position);
	std::swap(_type, other._type);
	std::swap(_nullable, other._nullable);
}


} } // namespace Poco::Data
