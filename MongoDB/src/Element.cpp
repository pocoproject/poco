//
// Element.cpp
//
// Library: MongoDB
// Package: MongoDB
// Module:  Element
//
// Copyright (c) 2012-2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/MongoDB/Element.h"


namespace Poco {
namespace MongoDB {


Element::Element(const std::string& name) : _name(name)
{
}


Element::Element(std::string&& name) : _name(std::move(name))
{
}


Element::~Element()
{
}


} } // namespace Poco::MongoDB
