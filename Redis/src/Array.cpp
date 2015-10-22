//
// Array.h
//
// $Id$
//
// Library: Redis
// Package: Redis
// Module:  Array
//
// Implementation of the Array class.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#include "Poco/Redis/Array.h"

namespace Poco {
namespace Redis {

Array::Array()
{
}


Array::Array(const Array& copy) : _elements(copy._elements)
{
}


Array::~Array()
{
}

void Array::add(Int64 value)
{
	_elements.push_back(new Type<Int64>(value));
}

void Array::add(const std::string& value)
{
	BulkString rs(value);
	_elements.push_back(new Type<BulkString>(rs));
}

void Array::add(const BulkString& value)
{
	_elements.push_back(new Type<BulkString>(value));
}

void Array::add()
{
	BulkString value;
	_elements.push_back(new Type<BulkString>(value));
}

std::string Array::toString() const 
{
	return ElementTraits<Array>::toString(*this);
}

} }