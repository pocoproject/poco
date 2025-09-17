//
// CppParser.cppm
//
// C++ module file
//
// Copyright (c) 2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

module;

#ifdef ENABLE_CPPPARSER
#include "Poco/CppParser/Attributes.h"
#include "Poco/CppParser/BuiltIn.h"
#include "Poco/CppParser/CppParser.h"
#include "Poco/CppParser/CppToken.h"
#include "Poco/CppParser/Decl.h"
#include "Poco/CppParser/Enum.h"
#include "Poco/CppParser/EnumValue.h"
#include "Poco/CppParser/Function.h"
#include "Poco/CppParser/NameSpace.h"
#include "Poco/CppParser/Parameter.h"
#include "Poco/CppParser/Parser.h"
#include "Poco/CppParser/Struct.h"
#include "Poco/CppParser/Symbol.h"
#include "Poco/CppParser/Tokenizer.h"
#include "Poco/CppParser/TypeDef.h"
#include "Poco/CppParser/Utility.h"
#include "Poco/CppParser/Variable.h"
#endif

export module Poco.CppParser;

export namespace Poco::CppParser {
	#ifdef ENABLE_CPPPARSER
	using Poco::CppParser::Attributes;
	using Poco::CppParser::AttributesParser;
	using Poco::CppParser::BuiltIn;
	using Poco::CppParser::CharLiteralToken;
	using Poco::CppParser::CommentToken;
	using Poco::CppParser::CppToken;
	using Poco::CppParser::Decl;
	using Poco::CppParser::Enum;
	using Poco::CppParser::EnumValue;
	using Poco::CppParser::Function;
	using Poco::CppParser::IdentifierToken;
	using Poco::CppParser::NameSpace;
	using Poco::CppParser::NumberLiteralToken;
	using Poco::CppParser::OperatorToken;
	using Poco::CppParser::Parameter;
	using Poco::CppParser::Parser;
	using Poco::CppParser::PreprocessorToken;
	using Poco::CppParser::StringLiteralToken;
	using Poco::CppParser::Struct;
	using Poco::CppParser::Symbol;
	using Poco::CppParser::Tokenizer;
	using Poco::CppParser::TypeDef;
	using Poco::CppParser::Utility;
	using Poco::CppParser::Variable;

	using Poco::CppParser::replace;
	#endif
}
