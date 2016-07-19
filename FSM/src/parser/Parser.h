/*
 * Parser.h
 *
 *  Created on: 16 janv. 2016
 *      Author: FrancisANDRE
 */

#ifndef POCO_FSM_PARSER_H_
#define POCO_FSM_PARSER_H_

#include <stack>
using std::stack;

#include "Poco/Path.h"
#include "Poco/File.h"
#include "Poco/Logger.h"
#include "Poco/Token.h"
#include "Poco/CountingStream.h"
using Poco::Logger;
using Poco::Token;
using Poco::Path;
using Poco::File;
using Poco::CountingInputStream;

#include "Tokenizer.h"
#include "model/SMC.h"

using Poco::FSM::MODEL::StatePtr;
using Poco::FSM::MODEL::EntryPtr;
using Poco::FSM::MODEL::ExitPtr;
using Poco::FSM::MODEL::GuardPtr;
using Poco::FSM::MODEL::MapPtr;
using Poco::FSM::MODEL::ActionPtr;
using Poco::FSM::MODEL::TransitionPtr;
using Poco::FSM::MODEL::ParameterPtr;
using Poco::FSM::MODEL::FSMPtr;
using Poco::FSM::MODEL::ExpressionPtr;
using Poco::FSM::MODEL::ArgumentPtr;
using Poco::FSM::MODEL::ReferencePtr;

namespace Poco {
	namespace FSM {
		namespace PARSER {

			class Parser {
			public:
				Parser(Logger& logger, const File& smfile, std::istream& istr);
				virtual ~Parser();
				FSMPtr parse(const Path& out);

			protected:
				const Poco::Token* parseAccess(const Poco::Token* next);
				const Poco::Token* parseAction(const Poco::Token* next);
				const Poco::Token* parseActions(const Poco::Token* next, List<ActionPtr>& actions);
				const Poco::Token* parseArgument(const Poco::Token* next, List<ArgumentPtr>& list);
				const Poco::Token* parseArguments(const Poco::Token* next, List<ArgumentPtr>& list);
				const Poco::Token* parseClass(const Poco::Token* next);
				const Poco::Token* parseDeclare(const Poco::Token* next);
				const Poco::Token* parseEntry(const Poco::Token* next);
				const Poco::Token* parseExit(const Poco::Token* next);
				const Poco::Token* parseExpression(const Poco::Token* next, ExpressionPtr& expression);
				const Poco::Token* parseFSM(const Poco::Token* next);
				const Poco::Token* parseFFSMlass(const Poco::Token* next);
				const Poco::Token* parseFSMFile(const Poco::Token* next);
				const Poco::Token* parseFile(const Poco::Token* next, const Path& out);
				const Poco::Token* parseGuard(const Poco::Token* next);
				const Poco::Token* parseHeader(const Poco::Token* next);
				const Poco::Token* parseImport(const Poco::Token* next);
				const Poco::Token* parseInclude(const Poco::Token* next);
				const Poco::Token* parseIncludes(const Poco::Token* next);
				const Poco::Token* parseMap(const Poco::Token* next);
				const Poco::Token* parseNext(const Poco::Token* next);
				const Poco::Token* parsePackage(const Poco::Token* next);
				const Poco::Token* parseParameter(const Poco::Token* next);
				const Poco::Token* parseParameters(const Poco::Token* next);
				const Poco::Token* parsePopArguments(const Poco::Token* next);
				const Poco::Token* parsePopTransition(const Poco::Token* next);
				const Poco::Token* parsePushTransition(const Poco::Token* next);
				const Poco::Token* parseRawCode(const Poco::Token* next);
				const Poco::Token* parseReturn(const Poco::Token* next);
				const Poco::Token* parseSource(const Poco::Token* next);
				const Poco::Token* parseStart(const Poco::Token* next);
				const Poco::Token* parseState(const Poco::Token* next);
				const Poco::Token* parseStates(const Poco::Token* next);
				const Poco::Token* parseTransition(const Poco::Token* next);
				const Poco::Token* parseTransitionArgs(const Poco::Token* next);
				const Poco::Token* parseTransitions(const Poco::Token* next);
				const Poco::Token* parseTerminal(const Poco::Token* next, ReferencePtr& reference);

				static bool isString(const Poco::Token* token);
				static bool isCharacter(const Poco::Token* token);
				static bool isNumber(const Poco::Token* token);
				static bool isIdentifier(const Poco::Token* token);
				static bool isOperator(const Poco::Token* token);
				static bool isOperator(const Poco::Token* token, int kind);
				static bool isKeyword(const Poco::Token* token, int kind);
				static bool isEOF(const Poco::Token* token);
				static void expectOperator(const Poco::Token* token, int kind, const string& msg);
				static void syntaxError(const string& msg);
				static void append(string& decl, const string& token);
				static void append(string& decl, const Poco::Token* token);

				const Poco::Token* nextToken();
				const Poco::Token* nextPreprocessed();
				const Poco::Token* nextParserToken();
				void reduce();

			public:
				const File& file() const { return _file; }

			private:
				CountingInputStream _istr;
				Tokenizer			_tokenizer;
				File		_file;
				Path		_path;
				string		_currentPath;
				bool		_inFile;
				string		_package;
				string		_library;
				string		_doc;
				string		_attrs;

				FSMPtr			fsm = nullptr;
				StatePtr		state= nullptr;
				GuardPtr		guard= nullptr;
				MapPtr			map= nullptr;
				ActionPtr		action= nullptr;
				TransitionPtr	transition= nullptr;
				ParameterPtr	parameter= nullptr;
				List<ParameterPtr> parameters;
				EntryPtr		entry = nullptr;
				ExitPtr			exit = nullptr;

				Logger&					_logger;
				stack<ExpressionPtr>	_stack;
			};

		}
	}
}
#endif
