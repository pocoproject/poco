/*
 * Character.h
 *
 *  Created on: 3 août 2014
 *      Author: FrancisANDRE
 */

#ifndef POCO_FSM_PARSER_CHARACTER_H_
#define POCO_FSM_PARSER_CHARACTER_H_

#include "Token.h"

namespace Poco { namespace FSM {
namespace PARSER {

class Character : public Token {
public:
	Character();
	~Character();
	Poco::Token::Class tokenClass() const;
	bool start(char c, std::istream& istr);
	void finish(std::istream& istr);
	char asChar() const;
};

}
}
}

#endif
