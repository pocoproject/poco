//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// and Contributors.
//

#ifndef POCO_FSM_PARSER_IDENTIFIER_H_
#define POCO_FSM_PARSER_IDENTIFIER_H_

#include "Token.h"

namespace Poco {
	namespace FSM {
		namespace PARSER {

			class Identifier : public Token {
			public:
				Identifier() {}
				~Identifier() {}
				Class tokenClass() const;
				bool start(char c, std::istream& istr);
				void finish(std::istream& istr);
			};

		}
	}
}

#endif
