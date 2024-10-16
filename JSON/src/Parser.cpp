//
// Parser.cpp
//
// Library: JSON
// Package: JSON
// Module:  Parser
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/JSON/Parser.h"
#undef min
#undef max
#include <clocale>


namespace Poco {
namespace JSON {


Parser::Parser(const Handler::Ptr& pHandler):
	ParserImpl(pHandler)
{
}


Parser::~Parser() = default;


void Parser::setHandler(const Handler::Ptr& pHandler)
{
	setHandlerImpl(pHandler);
}


} } // namespace Poco::JSON
