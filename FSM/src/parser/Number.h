/*
 * Number.h
 *
 *  Created on: 3 août 2014
 *      Author: FrancisANDRE
 */

#ifndef POCO_FSM_PARSER_NUMBER_H_
#define POCO_FSM_PARSER_NUMBER_H_

#include "Token.h"

namespace Poco { namespace FSM {
namespace PARSER {

class Number: public Token {
public:
	Number() {}
	~Number() {}

	Class tokenClass() const ;
	bool start(char c, std::istream& istr);
	void finish(std::istream& istr);
};

}
}
}

#endif
