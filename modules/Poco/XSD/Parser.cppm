//
// Parser.cppm
//
// C++ module file
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

module;

#ifdef ENABLE_XSD_PARSER
#include "Poco/XSD/Parser/Constants.h"
#include "Poco/XSD/Parser/StateMachine.h"
#include "Poco/XSD/Parser/XSDContentHandler.h"
#include "Poco/XSD/Parser/XSDParser.h"
#endif

export module Poco:XSD.Parser;

export namespace Poco::XSD::Parser {
	#ifdef ENABLE_XSD_PARSER
	using Poco::XSD::Parser::CompactAttributes;
	using Poco::XSD::Parser::Constants;
	using Poco::XSD::Parser::StateMachine;
	using Poco::XSD::Parser::XSDContentHandler;
	#endif
}
