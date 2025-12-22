//
// Handler.cpp
//
// Library: JSON
// Package: JSON
// Module:  Handler
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/JSON/Handler.h"


namespace Poco {
namespace JSON {


Handler::Handler() = default;


Handler::~Handler() = default;


Dynamic::Var Handler::asVar() const
{
	return {};
}


Poco::DynamicStruct Handler::asStruct() const
{
	return {};
}


} } // namespace Poco::JSON
