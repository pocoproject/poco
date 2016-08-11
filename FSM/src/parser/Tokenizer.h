/*
 * SourceDelimiterToken.h
 *
 *  Created on: 3 août 2014
 *      Author: FrancisANDRE
 */

#ifndef POCO_FSM_PARSER_TOKENIZER_H_
#define POCO_FSM_PARSER_TOKENIZER_H_
#include "Poco/StreamTokenizer.h"
using Poco::StreamTokenizer;


namespace Poco {
namespace FSM {
namespace PARSER {

class  Tokenizer : public StreamTokenizer {
  public:
	Tokenizer(std::istream& istr);
	~Tokenizer();
	const Poco::Token* nextToken() {
		return StreamTokenizer::next();
	}
};

}
}
}

#endif
