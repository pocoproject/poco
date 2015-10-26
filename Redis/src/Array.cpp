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


Array& Array::add(RedisType::Ptr value)
{
	checkNull();

	_elements.value().push_back(value);

	return *this;
}


std::string Array::toString() const 
{
	return ElementTraits<Array>::toString(*this);
}

} }
