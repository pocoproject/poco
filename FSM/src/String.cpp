/*
 * String.cpp
 *
 *  Created on: 3 août 2014
 *      Author: FrancisANDRE
 */

#include "Poco/Ascii.h"
#include "Poco/StreamTokenizer.h"
#include "Poco/Exception.h"

using Poco::StreamTokenizer;
using Poco::SyntaxException;
using Poco::Token;
using Poco::InvalidToken;
using Poco::EOFToken;
using Poco::WhitespaceToken;
using Poco::Ascii;

#include "parser/String.h"

namespace Poco
{
namespace FSM
{
namespace PARSER
{

String::String()
{
}


String::~String()
{
}


Token::Class String::tokenClass() const
{
    return Token::STRING_LITERAL_TOKEN;
}


bool String::start(char c, std::istream& istr)
{
    _value = c;
    return c == '"';
}


void String::finish(std::istream& istr)
{
    int next = istr.peek();
    while (next != -1 && next != '"' && next != '\n' && next != '\r')
    {
        if (next == '\\') _value += (char) istr.get();
        _value += (char) istr.get();
        next = istr.peek();
    }
    if (next == '"')
    {
        next = istr.get();
        _value += (char) next;
    }
    else throw SyntaxException("Unterminated string literal");
}


std::string String::asString() const
{
    std::string result;
    std::string::const_iterator it  = _value.begin();
    std::string::const_iterator end = _value.end();
    if (it != end)
    {
        if (*it == '"') ++it;
        while (it != end && *it != '"')
        {
            if (*it == '\\') ++it;
            if (it != end) result += *it++;
        }
    }
    return result;
}


}
}
}
