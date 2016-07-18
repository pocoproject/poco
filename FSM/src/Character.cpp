/*
 * Character.cpp
 *
 *  Created on: 3 août 2014
 *      Author: FrancisANDRE
 */

#include "Poco/StreamTokenizer.h"
#include "Poco/Exception.h"
#include "Poco/Ascii.h"
#include "parser/Character.h"

using Poco::StreamTokenizer;
using Poco::SyntaxException;
using Poco::Token;
using Poco::InvalidToken;
using Poco::EOFToken;
using Poco::WhitespaceToken;
using Poco::Ascii;

namespace Poco
{
namespace FSM
{
namespace PARSER
{

Character::Character()
{
}


Character::~Character()
{
}


Token::Class Character::tokenClass() const
{
    return Token::CHAR_LITERAL_TOKEN;
}


bool Character::start(char c, std::istream& istr)
{
    _value = c;
    return c == '\'';
}


void Character::finish(std::istream& istr)
{
    int next = istr.peek();
    while (next != -1 && next != '\'' && next != '\n' && next != '\r')
    {
        if (next == '\\') _value += (char) istr.get();
        _value += (char) istr.get();
        next = istr.peek();
    }
    if (next == '\'')
    {
        next = istr.get();
        _value += (char) next;
    }
    else throw SyntaxException("Unterminated character literal");
}


char Character::asChar() const
{
    char result('\0');
    std::string::const_iterator it  = _value.begin();
    std::string::const_iterator end = _value.end();
    if (it != end)
    {
        if (*it == '\'') ++it;
        while (it != end && *it != '\'')
        {
            if (*it == '\\') ++it;
            if (it != end) result = *it++;
        }
    }
    return result;
}


}
}
}
