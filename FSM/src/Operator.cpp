/*
 * Operator.cpp
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

#include "parser/Operator.h"

namespace Poco
{
namespace FSM
{
namespace PARSER
{
Operator::Operator()
{
    _opMap["["] = OP::OPENBRACKET;
    _opMap["]"] = OP::CLOSBRACKET;
    _opMap["("] = OP::OPENPARENT;
    _opMap[")"] = OP::CLOSPARENT;
    _opMap["{"] = OP::OPENBRACE;
    _opMap["}"] = OP::CLOSBRACE;
    _opMap["<"] = OP::LT;
    _opMap["<="] = OP::LE;
    _opMap["<<"] = OP::SHL;
    _opMap["<<="] = OP::SHL_ASSIGN;
    _opMap[">"] = OP::GT;
    _opMap[">="] = OP::GE;
    _opMap[">>"] = OP::SHR;
    _opMap[">>="] = OP::SHR_ASSIGN;
    _opMap["="] = OP::ASSIGN;
    _opMap["=="] = OP::EQ;
    _opMap["!"] = OP::NOT;
    _opMap["!="] = OP::NE;
    _opMap["&"] = OP::BITAND;
    _opMap["&="] = OP::BITAND_ASSIGN;
    _opMap["&&"] = OP::AND;
    _opMap["|"] = OP::BITOR;
    _opMap["|="] = OP::BITOR_ASSIGN;
    _opMap["||"] = OP::OR;
    _opMap["^"] = OP::XOR;
    _opMap["^="] = OP::XOR_ASSIGN;
    _opMap["~"] = OP::COMPL;
    _opMap["*"] = OP::STAR;
    _opMap["*="] = OP::STAR_ASSIGN;
    _opMap["/"] = OP::SLASH;
    _opMap["/="] = OP::SLASH_ASSIGN;
    _opMap["+"] = OP::PLUS;
    _opMap["+="] = OP::PLUS_ASSIGN;
    _opMap["++"] = OP::INCR;
    _opMap["-"] = OP::MINUS;
    _opMap["-="] = OP::MINUS_ASSIGN;
    _opMap["--"] = OP::DECR;
    _opMap["->"] = OP::ARROW;
    _opMap["%"] = OP::MOD;
    _opMap["%="] = OP::MOD_ASSIGN;
    _opMap[","] = OP::COMMA;
    _opMap["."] = OP::PERIOD;
    _opMap["..."] = OP::ELLIPSIS;
    _opMap[":"] = OP::COLON;
    _opMap["::"] = OP::DBL_COLON;
    _opMap[";"] = OP::SEMICOLON;
    _opMap["?"] = OP::QUESTION;
    _opMap["%{"] = OP::OPENCODE;
    _opMap["%}"] = OP::CLOSCODE;
    _opMap["%%"] = OP::FSM;
}


Operator::~Operator()
{
}


Token::Class Operator::tokenClass() const
{
    return Token::OPERATOR_TOKEN;
}


bool Operator::start(char c, std::istream& istr)
{
    _value = c;
    char next = (char) istr.peek();
    switch (_value[0])
    {
    case '[':
    case ']':
    case '(':
    case ')':
    case '{':
    case '}':
    case '<':
    case '>':
    case '=':
    case '!':
    case '&':
    case '|':
    case '*':
    case '+':
    case '-':
    case '^':
    case '~':
    case ',':
    case ':':
    case ';':
    case '%':
    case '?':
        return true;
    case '.':
        return !(next >= '0' && next <= '9');
    case '/':
        return !(next == '/' || next == '*');
    default:
        return false;
    }
}


void Operator::finish(std::istream& istr)
{
    int next = (char) istr.peek();
    switch (_value[0])
    {
    case '(':
    case ')':
    case '{':
    case '}':
    case '[':
    case ']':
    case ';':
    case '?':
    case '~':
    case ',':
        break;
    case '.':
        if (next == '.')
        {
            _value += (char) istr.get();
            if (istr.peek() != '.')  syntaxError(".", std::string(1, (char)istr.peek()));
            _value += (char) istr.get();
        }
        break;
    case ':':
        if (next == ':') _value += (char) istr.get();
        break;
    case '<':
        if (next == '<')
        {
            _value += (char) istr.get();
            next = (char) istr.peek();
        }
        if (next == '=') _value += (char) istr.get();
        break;
    case '>':
        if (next == '>')
        {
            _value += (char) istr.get();
            next = (char) istr.peek();
        }
        if (next == '=') _value += (char) istr.get();
        break;
    case '&':
        if (next == '&' || next == '=') _value += (char) istr.get();
        break;
    case '|':
        if (next == '|' || next == '=') _value += (char) istr.get();
        break;
    case '+':
        if (next == '+' || next == '=') _value += (char) istr.get();
        break;
    case '-':
        if (next == '-' || next == '=' || next == '>') _value += (char) istr.get();
        break;
    case '=':
    case '!':
    case '*':
    case '/':
    case '^':
    case '%':
        if (next == '=' || next == '{' || next == '}' || next == '%') _value += (char) istr.get();
        break;
    default:
        poco_bugcheck();
    }
}


int Operator::asInteger() const
{
    OpMap::const_iterator it = _opMap.find(_value);
    if (it != _opMap.end())
        return it->second;
    else
        return 0;
}


}
}
}
