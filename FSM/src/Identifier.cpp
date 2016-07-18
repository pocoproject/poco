/*
 * Identifier.cpp
 *
 *  Created on: 3 août 2014
 *      Author: FrancisANDRE
 */

#include "Poco/Ascii.h"
#include "Poco/StreamTokenizer.h"
using Poco::StreamTokenizer;
using Poco::Token;
using Poco::InvalidToken;
using Poco::EOFToken;
using Poco::WhitespaceToken;
using Poco::Ascii;
#include "parser/Identifier.h"


namespace Poco
{
namespace FSM
{
namespace PARSER
{
Token::Class
Identifier::tokenClass() const
{
    return Token::IDENTIFIER_TOKEN;
}
bool
Identifier::start(char c, std::istream& istr)
{
    if (c != -1 && Ascii::isAlpha(c))
    {
        _value = c;
        return true;
    }
    else
        return false;
}
void
Identifier::finish(std::istream& istr)
{
    int c = istr.peek();
    while (c != -1 && (Ascii::isAlphaNumeric(c) || Ascii::isDigit(c) || c == '_') || c == '-')
    {
        istr.get();
        _value += c;
        c = istr.peek();
    }
}

}
}
}
