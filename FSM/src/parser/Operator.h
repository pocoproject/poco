/*
 * Operator.h
 *
 *  Created on: 3 août 2014
 *      Author: FrancisANDRE
 */

#ifndef POCO_FSM_PARSER_OPERATOR_H_
#define POCO_FSM_PARSER_OPERATOR_H_

#include <map>
using std::map;
#include "Token.h"

namespace Poco {
	namespace FSM {
		namespace PARSER {

			class Operator : public Token {
			public:
				enum OP	{
					OPENBRACKET = 1,// [
					CLOSBRACKET,    // ]
					OPENPARENT,     // (
					CLOSPARENT,     // )
					OPENBRACE,      // {
					CLOSBRACE,      // }
					LT,             // <
					LE,             // <=
					SHL,            // <<
					SHL_ASSIGN,		// <<=
					GT,				// >
					GE,             // >=
					SHR,            // >>
					SHR_ASSIGN,     // >>=
					ASSIGN,         // =
					EQ,             // ==
					NOT,            // !
					NE,             // !=
					BITAND,         // &
					BITAND_ASSIGN,  // &=
					AND,            // &&
					BITOR,          // |
					BITOR_ASSIGN,   // |= 
					OR,             // ||
					XOR,            // ^
					XOR_ASSIGN,     // ^=
					COMPL,          // ~
					STAR,		    // *
					STAR_ASSIGN,	// *=
					SLASH,          // /
					SLASH_ASSIGN,   // /=
					PLUS,           // +
					PLUS_ASSIGN,    // +=
					INCR,           // ++
					MINUS,          // -
					MINUS_ASSIGN,   // -=
					DECR,           // --
					ARROW,          // ->
					MOD,            // %
					MOD_ASSIGN,     // %=
					COMMA,          // ,
					PERIOD,         // .
					ELLIPSIS,		// ...
					COLON,          // :
					DBL_COLON,      // ::
					SEMICOLON,      // ;
					QUESTION,       // ?
					OPENCODE,		// %{
					CLOSCODE,		// %}
					FSM				// %%
				};

				Operator();
				~Operator();
				Poco::Token::Class tokenClass() const;
				bool start(char c, std::istream& istr);
				void finish(std::istream& istr);
				int asInteger() const;

			private:
				typedef map<std::string, int> OpMap;

				OpMap _opMap;
			};

		}
	}
}
#endif
