/*
 * Token.h
 *
 *  Created on: 3 août 2014
 *      Author: FrancisANDRE
 */

#ifndef POCO_FSM_PARSER_TOKEN_H_
#define POCO_FSM_PARSER_TOKEN_H_

#include "Poco/Token.h"
#include <string>
#include <istream>


namespace Poco {
	namespace FSM {
		namespace PARSER {
			class Token : public Poco::Token {
			public:
				Token();
				~Token();

			protected:
				void syntaxError(const std::string& expected, const std::string& actual);
			};

		}
	}
}
#endif
