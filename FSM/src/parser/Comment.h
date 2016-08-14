//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// and Contributors.
//

#ifndef POCO_FSM_PARSER_COMMENT_H_
#define POCO_FSM_PARSER_COMMENT_H_

#include "Token.h"


namespace Poco { namespace FSM {
namespace PARSER {

class Comment : public Token {
public:
	Comment();
	~Comment();
	Poco::Token::Class tokenClass() const;
	bool start(char c, std::istream& istr);
	void finish(std::istream& istr);
	std::string asString() const;
};

}
}
}

#endif
