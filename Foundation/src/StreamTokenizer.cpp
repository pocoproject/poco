//
// StreamTokenizer.cpp
//
// $Id: //poco/1.4/Foundation/src/StreamTokenizer.cpp#1 $
//
// Library: Foundation
// Package: Streams
// Module:  StreamTokenizer
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/StreamTokenizer.h"
#include "Poco/Token.h"


namespace Poco {


StreamTokenizer::StreamTokenizer():
	_pIstr(0)
{
}


StreamTokenizer::StreamTokenizer(std::istream& istr):
	_pIstr(&istr)
{
}


StreamTokenizer::~StreamTokenizer()
{
	for (TokenVec::iterator it = _tokens.begin(); it != _tokens.end(); ++it)
	{
		delete *it;
	}
}


void StreamTokenizer::attachToStream(std::istream& istr)
{
	_pIstr = &istr;
}


void StreamTokenizer::addToken(Token* pToken)
{
	poco_check_ptr (pToken);

	pToken->ignore(pToken->tokenClass() == Token::COMMENT_TOKEN || pToken->tokenClass() == Token::WHITESPACE_TOKEN);
	_tokens.push_back(pToken);
}


void StreamTokenizer::addToken(Token* pToken, bool ignore)
{
	poco_check_ptr (pToken);

	pToken->ignore(ignore);
	_tokens.push_back(pToken);
}

	
const Token* StreamTokenizer::next()
{
	poco_check_ptr (_pIstr);
	
	static const int eof = std::char_traits<char>::eof();

	int first = _pIstr->get();
	TokenVec::const_iterator it = _tokens.begin();
	while (first != eof && it != _tokens.end())
	{
		Token* ti = *it;
		if (ti->start((char) first, *_pIstr))
		{
			ti->finish(*_pIstr);
			if (ti->ignored()) 
			{
				first = _pIstr->get();
				it = _tokens.begin();
			}
			else return *it;
		}
		else ++it;
	}
	if (first == eof)
	{
		return &_eofToken;
	}
	else
	{
		_invalidToken.start((char) first, *_pIstr);
		return &_invalidToken;
	}
}


} // namespace Poco
