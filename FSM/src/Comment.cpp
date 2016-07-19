/*
 * Comment.cpp
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
#include "parser/Comment.h"

namespace Poco
{
namespace FSM
{
namespace PARSER
{

Comment::Comment()
{
}


Comment::~Comment()
{
}


Token::Class Comment::tokenClass() const
{
    return (_value.length() > 2 && _value[2] == '/') ? Token::SPECIAL_COMMENT_TOKEN : Token::COMMENT_TOKEN;
}


bool Comment::start(char c, std::istream& istr)
{
    _value = c;
    int next  = istr.peek();
    return c == '/' && (next == '*' || next == '/');
}


void Comment::finish(std::istream& istr)
{
    int next = istr.peek();
    if (next == '/')
    {
        while (next != -1 && next != '\r' && next != '\n')
        {
            _value += (char) istr.get();
            next = istr.peek();
        }
    }
    else
    {
        _value += (char) istr.get(); // *
        next = istr.peek();
        while (next != -1)
        {
            next = istr.get();
            _value += (char) next;
            if (next == '*' && istr.peek() == '/')
            {
                _value += (char) istr.get();
                break;
            }
        }
    }
}


std::string Comment::asString() const
{
    if (_value.length() > 2 && _value[2] == '/')
        return _value.substr(3);
    else
        return _value.substr(2);
}


}
}
}
