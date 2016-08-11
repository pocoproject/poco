/*
 * Tokenizer.cpp
 *
 *  Created on: 3 août 2014
 *      Author: FrancisANDRE
 */

#include "Poco/Token.h"
using Poco::WhitespaceToken;
#include "parser/Tokenizer.h"
#include "parser/Identifier.h"
#include "parser/Number.h"
#include "parser/Keyword.h"
#include "parser/Comment.h"
#include "parser/Character.h"
#include "parser/Operator.h"
#include "parser/String.h"


namespace Poco
{
namespace FSM
{
namespace PARSER
{
Tokenizer::Tokenizer(std::istream& istr):
    StreamTokenizer(istr)
{
    addToken(new WhitespaceToken());
    addToken(new Comment());
    addToken(new Identifier());
    addToken(new Number());
    addToken(new Keyword());
    addToken(new Operator());
    addToken(new Character());
    addToken(new String());

}


Tokenizer::~Tokenizer()
{
}

}
}
}
