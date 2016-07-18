/*
 * Keyword.cpp
 *
 *  Created on: 3 août 2014
 *      Author: FrancisANDRE
 */

#include "Poco/Token.h"
#include "Poco/Ascii.h"
using Poco::InvalidToken;
using Poco::EOFToken;
using Poco::WhitespaceToken;
using Poco::Ascii;

#include "parser/Keyword.h"

namespace Poco
{
namespace FSM
{
namespace PARSER
{
Keyword::Keyword()
{
    _opMap["%start"]	= START;		// %start
    _opMap["%class"]	= CLASS;		// %class
    _opMap["%fsmclass"]	= FFSMLASS,		// %fsmclass
                          _opMap["%fsmfile"]	= FSMFILE,		// %fsmfile
                                  _opMap["%header"]	= HEADER;		// %header
    _opMap["%include"]	= INCLUDE;		// %include
    _opMap["%package"]	= PACKAGE;		// %package
    _opMap["%import"]	= IMPORT;		// %import
    _opMap["%declare"]	= DECLARE;		// %declare
    _opMap["%access"]	= ACCESS;		// %access
    _opMap["%map"]		= MAP;			// %map
    _opMap["%return"]	= RETURN;		// %return
}
Keyword::~Keyword()
{

}

Token::Class Keyword::tokenClass() const
{
    return Token::KEYWORD_TOKEN;
}
bool Keyword::start(char c, std::istream& istr)
{
    if (c != -1 && c == '%')
    {
        char next = istr.peek();
        if (islower(next))
        {
            _value = c;
            return true;
        }
    }
    return false;
}
void Keyword::finish(std::istream& istr)
{
    int c = istr.peek();
    while (c != -1 && (Ascii::isLower(c)))
    {
        istr.get();
        _value += c;
        c = istr.peek();
    }
}
int Keyword::asInteger() const
{
    OpMap::const_iterator it = _opMap.find(_value);
    if (it != _opMap.end())
        return it->second;
    else
        return 0;
}
const char* Keyword::asString(int kwno)
{
    switch (kwno)
    {
    case START:
        return "%start";
    case CLASS:
        return "%class";
    case FSMFILE:
        return "%fsmfile";
    case FFSMLASS:
        return "%fsmclass";
    case HEADER:
        return "%header";
    case INCLUDE:
        return "%include";
    case PACKAGE:
        return "%package";
    case IMPORT:
        return "%import";
    case DECLARE:
        return "%declare";
    case ACCESS:
        return "%access";
    case MAP:
        return "%map";
    default:
        return "???";
    }
}

}
}
}
