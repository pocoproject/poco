/*
 * Keyword.h
 *
 *  Created on: 3 août 2014
 *      Author: FrancisANDRE
 */

#ifndef POCO_FSM_PARSER_PERCENT_H_
#define POCO_FSM_PARSER_PERCENT_H_

#include <map>
using std::map;
#include "Token.h"

namespace Poco { namespace FSM {
namespace PARSER {

class Keyword: public Token {
public:
	enum  KW {
		START = 1,	// %start
		CLASS,		// %class
		FSMFILE,	// %fsmfile
		FFSMLASS,	// %fsmclass
		HEADER,		// %header
		INCLUDE,	// %include
		PACKAGE,	// %package
		IMPORT,		// %import
		DECLARE,	// %declare
		ACCESS,		// %access
		MAP,		// %map
		RETURN		// %return
	};
	Keyword();
	~Keyword();

	Class tokenClass() const ;
	bool start(char c, std::istream& istr);
	void finish(std::istream& istr);
	int asInteger() const;
	static const char* asString(int kwno);

private:
	typedef map<std::string, int> OpMap;
	
	OpMap _opMap;
};

}
}
}

#endif
