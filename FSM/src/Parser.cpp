/*
 * Parser.cpp
 *
 *  Created on: 16 janv. 2016
 *      Author: FrancisANDRE
 */
#include <iostream>
#include <sstream>
#include <cassert>
#include <typeinfo>
#include <string.h>

#include "Poco/Bugcheck.h"
#include "Poco/NumberFormatter.h"
#include "Poco/StreamTokenizer.h"
#include "Poco/Path.h"
#include "Poco/File.h"
#include "Poco/Ascii.h"

#include "Poco/String.h"

using Poco::Bugcheck;
using Poco::NumberFormatter;
using Poco::StreamTokenizer;
using Poco::SyntaxException;
using Poco::InvalidToken;
using Poco::EOFToken;
using Poco::WhitespaceToken;
using Poco::Path;
using Poco::File;
using Poco::Ascii;

#include "parser/Parser.h"
#include "parser/Identifier.h"
#include "parser/Number.h"
#include "parser/Keyword.h"
#include "parser/Comment.h"
#include "parser/Character.h"
#include "parser/Operator.h"
#include "parser/String.h"

#include "model/Action.h"
#include "model/FSM.h"
#include "model/Guard.h"
#include "model/Map.h"
#include "model/Parameter.h"
#include "model/State.h"
#include "model/Entry.h"
#include "model/Exit.h"
#include "model/Transition.h"
#include "model/Expression.h"
#include "model/Transition.h"
#include "model/Reference.h"
#include "model/Variable.h"
#include "model/Function.h"
#include "model/Literal.h"
#include "model/Reference.h"
#include "model/Argument.h"
#include "model/UnaryOperation.h"
#include "model/BinaryOperation.h"

#include "model/Factory.h"
#include "cpp/FactoryForCpp.h"

using namespace std;

using Poco::FSM::MODEL::Action;
using Poco::FSM::MODEL::FSM;
using Poco::FSM::MODEL::Guard;
using Poco::FSM::MODEL::Map;
using Poco::FSM::MODEL::Parameter;
using Poco::FSM::MODEL::State;
using Poco::FSM::MODEL::Transition;
using Poco::FSM::MODEL::ActionPtr;
using Poco::FSM::MODEL::FSMPtr;
using Poco::FSM::MODEL::GuardPtr;
using Poco::FSM::MODEL::MapPtr;
using Poco::FSM::MODEL::ParameterPtr;
using Poco::FSM::MODEL::StatePtr;
using Poco::FSM::MODEL::TransitionPtr;
using Poco::FSM::MODEL::FunctionPtr;
using Poco::FSM::MODEL::VariablePtr;
using Poco::FSM::MODEL::LiteralPtr;
using Poco::FSM::MODEL::ExpressionPtr;
using Poco::FSM::MODEL::ArgumentPtr;
using Poco::FSM::MODEL::UnaryOperation;
using Poco::FSM::MODEL::BinaryOperation;

static const char TAB = '\t';

static const Poco::FSM::MODEL::Factory& factory = Poco::FSM::PARSER::CPP::FactoryForCpp();

namespace Poco
{
namespace FSM
{
namespace PARSER
{

Parser::Parser(Logger& logger, const File& file, istream& istr) :
    _logger(logger),
    _istr(istr),
    _tokenizer(_istr),
    _file(file)
{
}
Parser:: ~Parser()
{
}

FSMPtr Parser::parse(const Path& out)
{
    try
    {
        fsm = factory.newFSM(this);
        const Poco::Token* next = nextToken();
        next = parseFile(next, out);
        if (next && !isEOF(next))
            syntaxError("Additional tokens behind supposed EOF");
    }
    catch (SyntaxException& exc)
    {
        string m(exc.message());
        string where(_currentPath);
        where.append("(");
        where.append(NumberFormatter::format(_istr.getCurrentLineNumber()));
        where.append(")");
        throw SyntaxException(m, where);
    }
    return fsm;
}
inline bool Parser::isString(const Poco::Token* token)
{
    return token->is(Token::STRING_LITERAL_TOKEN);
}
inline bool Parser::isCharacter(const Poco::Token* token)
{
    return token->is(Token::CHAR_LITERAL_TOKEN);
}
inline bool Parser::isNumber(const Poco::Token* token)
{
    return token->is(Token::INTEGER_LITERAL_TOKEN) || token->is(Token::LONG_INTEGER_LITERAL_TOKEN);
}

inline bool Parser::isIdentifier(const Poco::Token* token)
{
    return token->is(Token::IDENTIFIER_TOKEN) || isOperator(token, Operator::DBL_COLON);
}


inline bool Parser::isOperator(const Poco::Token* token)
{
    return token->is(Token::OPERATOR_TOKEN);
}

inline bool Parser::isOperator(const Poco::Token* token, int kind)
{
    return token->is(Token::OPERATOR_TOKEN) && token->asInteger() == kind;
}


inline bool Parser::isKeyword(const Poco::Token* token, int kind)
{
    return token->is(Token::KEYWORD_TOKEN) && token->asInteger() == kind;
}


inline bool Parser::isEOF(const Poco::Token* token)
{
    return token->is(Token::EOF_TOKEN);
}
const char* classToString(Token::Class klass)
{
    switch (klass)
    {
    case Token::IDENTIFIER_TOKEN:
        return "IDENTIFIER";
    case Token::KEYWORD_TOKEN:
        return "KEYWORD";
    case Token::SEPARATOR_TOKEN:
        return "SEPARATOR";
    case Token::OPERATOR_TOKEN:
        return "OPERATOR";
    case Token::STRING_LITERAL_TOKEN:
        return "STRING_LITERAL";
    case Token::CHAR_LITERAL_TOKEN:
        return "CHAR_LITERAL";
    case Token::INTEGER_LITERAL_TOKEN:
        return "INTEGER_LITERAL";
    case Token::LONG_INTEGER_LITERAL_TOKEN:
        return "LONG_INTEGER_LITERAL";
    case Token::FLOAT_LITERAL_TOKEN:
        return "FLOAT_LITERAL";
    case Token::DOUBLE_LITERAL_TOKEN:
        return "DOUBLE_LITERAL";
    case Token::COMMENT_TOKEN:
        return "COMMENT";
    case Token::SPECIAL_COMMENT_TOKEN:
        return "SPECIAL_COMMENT";
    case Token::PREPROCESSOR_TOKEN:
        return "PREPROCESSOR";
    case Token::WHITESPACE_TOKEN:
        return "WHITESPACE";
    case Token::EOF_TOKEN:
        return "EOF";
    case Token::INVALID_TOKEN:
        return "INVALID";
    case Token::USER_TOKEN:
        return "USER";
    }
    return "???";
}

const Poco::Token* Parser::parseFile(const Poco::Token* next, const Path& out)
{
    while (next->is(Token::KEYWORD_TOKEN) || next->is(Token::OPERATOR_TOKEN))
    {
        if (next->is(Token::KEYWORD_TOKEN))
            switch (next->asInteger())
            {
            case Keyword::START:	// %start
                next = parseStart(next);
                break;
            case Keyword::CLASS: 	// %class
                next = parseClass(next);
                break;
            case Keyword::FSMFILE: 	// %fsmfile
                next = parseFSMFile(next);
                break;
            case Keyword::FFSMLASS: 	// %fsmclass
                next = parseFFSMlass(next);
                break;
            case Keyword::HEADER:	// %header
                next = parseHeader(next);
                break;
            case Keyword::INCLUDE: 	// %include
                next = parseIncludes(next);
                break;
            case Keyword::PACKAGE: 	// %package
                next = parsePackage(next);
                break;
            case Keyword::IMPORT: 	// %import
                next = parseImport(next);
                break;
            case Keyword::DECLARE: 	// %declare
                next = parseDeclare(next);
                break;
            case Keyword::ACCESS: 	// %access
                next = parseAccess(next);
                break;
            case Keyword::MAP:		// %map
                if (fsm->fsmfile().empty())
                    fsm->fsmfile() = fsm->klass() + "_sm";
                fsm->setcontext();

                if (fsm->updated(out))
                {
                    fsm->updated() = true;
                    return nullptr;
                }

                next = parseMap(next);
                break;
            case Keyword::RETURN: 	// %return
                next = parseReturn(next);
                break;
            default:
                poco_bugcheck();
                break;
            }
        else if (next->is(Token::OPERATOR_TOKEN))
            switch (next->asInteger())
            {
            case Operator::OPENCODE:	// %{
                next = parseRawCode(next);
                break;
            default:
                poco_bugcheck();
            }
    }
    return next;
}
const Poco::Token* Parser::parseSource(const Poco::Token* next)
{
    return next;
}
const Poco::Token* Parser::parseReturn(const Poco::Token* next)
{
    poco_assert(isKeyword(next, Keyword::RETURN));
    const string rcclass = next->tokenString();
    next = nextToken();
    if (next->is(Token::IDENTIFIER_TOKEN))
    {
        string klass = next->tokenString();
        fsm->returnt() = klass;
        next = nextToken();
    }
    return next;
}
const Poco::Token* Parser::parseStart(const Poco::Token* next)
{
    poco_assert(isKeyword(next, Keyword::START));
    const string keyword = next->tokenString();

    next = nextToken();
    if (next->is(Token::IDENTIFIER_TOKEN))
    {
        string startstate = next->tokenString();
        next = nextToken();
        if (isOperator(next, Operator::DBL_COLON))
        {
            startstate += next->tokenString();
            next = nextToken();
            if (next->is(Token::IDENTIFIER_TOKEN))
            {
                startstate += next->tokenString();
                next = nextToken();
            }
        }
        fsm->startState() = startstate;

    }
    return next;
}
const Poco::Token* Parser::parseClass(const Poco::Token* next)
{
    poco_assert(isKeyword(next, Keyword::CLASS));
    const string pcclass = next->tokenString();
    next = nextToken();
    if (next->is(Token::IDENTIFIER_TOKEN))
    {
        string klass = next->tokenString();
        fsm->klass() = klass;
        next = nextToken();
    }
    return next;
}
const Poco::Token* Parser::parseFSMFile(const Poco::Token* next)
{
    poco_assert(isKeyword(next, Keyword::FSMFILE));
    next = nextToken();
    if (next->is(Token::IDENTIFIER_TOKEN))
    {
        string fsmfile = next->tokenString();
        fsm->fsmfile() = fsmfile;
        next = nextToken();
    }
    return next;
}
const Poco::Token* Parser::parseFFSMlass(const Poco::Token* next)
{
    poco_assert(isKeyword(next, Keyword::FFSMLASS));
    next = nextToken();
    if (next->is(Token::IDENTIFIER_TOKEN))
    {
        string fsmclass = next->tokenString();
        fsm->fsmclass() = fsmclass;
        next = nextToken();
    }
    return next;
}
const Poco::Token* Parser::parseHeader(const Poco::Token* next)
{
    poco_assert(isKeyword(next, Keyword::HEADER));
    const string pcheader = next->tokenString();
    next = nextToken();
    string file;
    do
    {
        file += next->tokenString();
        next = nextToken();
    }
    while (!next->is(Token::KEYWORD_TOKEN));
    fsm->header() = file;
    return next;
}
const Poco::Token* Parser::parseIncludes(const Poco::Token* next)
{
    poco_assert(isKeyword(next, Keyword::INCLUDE));
    next = nextToken();
    for (;;)
    {
        next = parseInclude(next);
        if (isOperator(next, Operator::COMMA))
            next = nextToken();
        else
            break;
    }
    return next;
}
const Poco::Token* Parser::parseInclude(const Poco::Token* next)
{
    if (next->is(Token::STRING_LITERAL_TOKEN))
    {
        string file = next->tokenString().substr(1); // remove begin "
        file = file.substr(0, file.size() - 1);		 // remove ending "
        fsm->includes().push_back(file);
        next = nextToken();
    }
    else if (next->is(Token::IDENTIFIER_TOKEN))
    {
        string file = next->tokenString();
        next = nextToken();
        if (isOperator(next, Operator::PERIOD))
        {
            file += '.';
            next = nextToken();
            if (next->is(Token::IDENTIFIER_TOKEN))
            {
                file += next->tokenString();
                fsm->includes().push_back(file);
                next = nextToken();
            }
        }
    }
    return next;
}
const Poco::Token* Parser::parsePackage(const Poco::Token* next)
{
    poco_assert(isKeyword(next, Keyword::PACKAGE));
    const string pcpackage = next->tokenString();
    next = nextToken();
    string packages;
    do
    {
        if (next->is(Token::IDENTIFIER_TOKEN))
        {
            string package = next->tokenString();
            fsm->addPackage(package);
            packages += package;
            next = nextToken();
            if (isOperator(next, Operator::DBL_COLON))
            {
                packages += next->tokenString();
                next = nextToken();
            }
        }
    }
    while (!next->is(Token::KEYWORD_TOKEN));
    return next;
}
const Poco::Token* Parser::parseImport(const Poco::Token* next)
{
    poco_assert(isKeyword(next, Keyword::IMPORT));
    const string pcimport = next->tokenString();
    next = nextToken();
    if (next->is(Token::IDENTIFIER_TOKEN))
    {
        string import = next->tokenString();
        next = nextToken();
    }
    return next;
}
const Poco::Token* Parser::parseDeclare(const Poco::Token* next)
{
    poco_assert(isKeyword(next, Keyword::DECLARE));
    const string pcdeclare = next->tokenString();
    next = nextToken();
    if (next->is(Token::IDENTIFIER_TOKEN))
    {
        string declare = next->tokenString();
        next = nextToken();
    }
    return next;
}
const Poco::Token* Parser::parseAccess(const Poco::Token* next)
{
    poco_assert(isKeyword(next, Keyword::ACCESS));
    const Poco::Token* token = next;
    next = nextToken();
    if (next->is(Token::IDENTIFIER_TOKEN))
    {
        string access = next->tokenString();
        next = nextToken();
    }
    return next;
}
const Poco::Token* Parser::parseMap(const Poco::Token* next)
{
    poco_assert(isKeyword(next, Keyword::MAP));
    next = nextToken();
    if (next->is(Token::IDENTIFIER_TOKEN))
    {
        map = factory.newMap(next->tokenString());
        fsm->add(map);
        next = nextToken();
        next = parseStates(next);
        if (map->defaultState() == nullptr)
            map->defaultState() = factory.newState("Default");
    }
    return next;
}
const Poco::Token* Parser::parseStates(const Poco::Token* next)
{
    poco_assert(isOperator(next, Operator::FSM));
    const Poco::Token* token = next;
// OPEN { 	// Transport connection is open
// 		WAIT										CLOSING				{}
// 		TDISreq(tsdu: TPDU::DR&)	[P7()]			CLOSING				{DR(tsdu);}
// }
    next = nextToken();
    do
    {
        next = parseState(next);
    }
    while (!isOperator(next, Operator::FSM));
    next = nextToken();
    return next;
}
const Poco::Token* Parser::parseState(const Poco::Token* next)
{
    poco_assert(isIdentifier(next));
    state = map->lookfor(next->tokenString());
    if (state == nullptr)
    {
        state = factory.newState(next->tokenString());
        map->add(state);
    }
    map->add(state);
    next = nextToken();
    next = parseEntry(next);
    state->entry() = entry;
    next = parseExit(next);
    state->exit() = exit;
    next = parseTransitions(next);
    next = nextToken();
    return next;
}
const Poco::Token* Parser::parseEntry(const Poco::Token* next)
{
    entry = nullptr;
    if (isOperator(next, Operator::OPENBRACE))
        return next;
    poco_assert(next->tokenString() == "Entry");
    entry = factory.newEntry("Entry");
    next = nextToken();
    next = parseActions(next, entry->actions());
    return next;
}
const Poco::Token* Parser::parseExit(const Poco::Token* next)
{
    exit = nullptr;
    if (isOperator(next, Operator::OPENBRACE))
        return next;
    poco_assert(next->tokenString() == "Exit");
    exit = factory.newExit("Exit");
    next = nextToken();
    next = parseActions(next, exit->actions());
    return next;
}
const Poco::Token* Parser::parseTransitions(const Poco::Token* next)
{
    poco_assert(isOperator(next, Operator::OPENBRACE));
    next = nextToken();
    while (!isOperator(next, Operator::CLOSBRACE))
    {
        next = parseTransition(next);
    }
    return next;
}
const Poco::Token* Parser::parseTransition(const Poco::Token* next)
{
    poco_assert(isIdentifier(next));
    string name = next->tokenString();
    next = nextToken();
    next = parseParameters(next);
    transition = state->get(name, parameters);
    if (transition == nullptr)
    {
        transition = factory.newTransition(name);
        for (auto parameter : parameters)
            if (parameter != nullptr)
                transition->add(parameter);
        state->add(transition);
        map->add(transition);
    }
    guard = nullptr;
    next = parseGuard(next);
    transition->add(guard);
    next = parseNext(next);
    next = parseActions(next, guard->actions());
//	_dump(transition, guard);
    return next;
}
const Poco::Token* Parser::parseTransitionArgs(const Poco::Token* next)
{
    next = nextToken();
    return next;
}
const Poco::Token* Parser::parseGuard(const Poco::Token* next)
{
    guard = factory.newGuard("");
    if (isOperator(next, Operator::OPENBRACKET))
    {
        do
        {
            next = nextToken();
            next = parseExpression(next, guard->condition());
        }
        while (!isOperator(next, Operator::CLOSBRACKET));
        guard->condition()->guard() = guard;
        next = nextToken();
    }
    return next;
}
const Poco::Token* Parser::parseTerminal(const Poco::Token* next, ReferencePtr& reference)
{
    poco_assert(isIdentifier(next));
    if (isIdentifier(next))
    {
        const Poco::Token* identifier = next; // true/false not supported
        next = nextToken();
        if (isOperator(next, Operator::OPENPARENT))
        {
            FunctionPtr function = factory.newFunction(identifier->tokenString());
            next = parseArguments(next, function->arguments());
            reference = factory.newReference(function);
        }
        else
        {
            VariablePtr variable = factory.newVariable(identifier->tokenString());
            reference = factory.newReference(variable);
        }
        reference->guard() = guard;
    }
    return next;
}
void Parser::reduce()
{
    ExpressionPtr top;
    ExpressionPtr right;
    BinaryOperationPtr binop;
    ExpressionPtr left;
    ReferencePtr  ref;

    right = _stack.top();
    _stack.pop();
    poco_check_ptr(right);
    top = _stack.top();
    if (top == nullptr)
    {
        _stack.pop();
        _stack.push(right);
    }
    else
    {
        binop = dynamic_cast<BinaryOperation*>(top);
        _stack.pop();
        left = _stack.top();
        _stack.pop();
        poco_check_ptr(left);
        binop->left() = left;
        left->operation() = binop;
        binop->right() = right;
        right->operation() = binop;
        if (_stack.size() > 1)
            if (_stack.top() == nullptr)
                _stack.pop();

        _stack.push(binop);
    }
}
const Poco::Token* Parser::parseExpression(const Poco::Token* next, ExpressionPtr& expression)
{
    poco_assert(isIdentifier(next) || isOperator(next, Operator::NOT) || isOperator(next, Operator::OPENPARENT));
    poco_assert(_stack.size() == 0);

    for (;;)
    {
        if (isOperator(next, Operator::CLOSBRACKET))
            break;

        if (isIdentifier(next))
        {
            ReferencePtr reference;
            next = parseTerminal(next, reference);
            expression = reference;
            if (_stack.size() > 0)
            {
                ExpressionPtr top = _stack.top();
                if (top != nullptr)
                {
                    //
                    // TODO: to be improved using true operator's precedence.
                    /*
                    1 	()   []   ->   .   :: 										Function call, scope, array/member access
                    2 	 !   ~   -   +   *   &   sizeof   type cast   ++   --   	(most) unary operators, sizeof and type casts (right to left)
                    3 	*   /   % MOD 												Multiplication, division, modulo
                    4 	+   - 														Addition and subtraction
                    5 	<<   >> 													Bitwise shift left and right
                    6 	<   <=   >   >= 											Comparisons: less-than, ...
                    7 	==   != 													Comparisons: equal and not equal
                    8 	& 															Bitwise	AND
                    9 	^ 															Bitwise exclusive OR (XOR)
                    10 	| 															Bitwise inclusive (normal) OR
                    11 	&& 															Logical AND
                    12 	||															Logical OR
                    13 	 ? : 														Conditional expression (ternary)
                    14 	=   +=   -=   *=   /=   %=   &=   |=   ^=   <<=   >>= 		Assignment operators (right to left)
                    15 	, 															Comma operator
                    */
                    //
                    UnaryOperation* monop = dynamic_cast<UnaryOperation*>(top);
                    BinaryOperation* binop = dynamic_cast<BinaryOperation*>(top);
                    if (monop)
                    {
                        monop->operand() = reference;
                        reference->operation() = monop;
                        expression = monop;
                        _stack.pop();
                    }
                    else if (binop && (binop->op() == FSM::MODEL::Operator::equal || binop->op() == FSM::MODEL::Operator::notequal))
                    {
                        _stack.pop();
                        binop->right() = reference;
                        reference->operation() = binop;
                        binop->left() = _stack.top();
                        _stack.top()->operation() = binop;
                        expression = binop;
                        _stack.pop();
                    }
                }
            }
            _stack.push(expression);
        }
        else if (isOperator(next, Operator::OPENPARENT))
        {
            _stack.push(nullptr);
            next = nextToken();
        }
        else if (isOperator(next, Operator::CLOSPARENT))
        {
            reduce();
            next = nextToken();
        }
        else if (isOperator(next))
        {
            switch (next->asInteger())
            {
            case Operator::NE:
            case Operator::EQ:
            case Operator::AND:
            case Operator::OR:
            case Operator::XOR:
            {
                FSM::MODEL::Operator op = FSM::MODEL::Operator::none;
                switch (next->asInteger())
                {
                case Operator::NE:
                    op = FSM::MODEL::Operator::notequal;
                    break;
                case Operator::EQ:
                    op = FSM::MODEL::Operator::equal;
                    break;
                case Operator::AND:
                    op = FSM::MODEL::Operator::and;
                    break;
                case Operator::OR:
                    op = FSM::MODEL::Operator::or;
                    break;
                case Operator::XOR:
                    op = FSM::MODEL::Operator::xor;
                    break;
                default:
                    poco_assert(false);
                }
                BinaryOperation* binop = factory.newBinaryOperation(op);
                _stack.push(binop);
                next = nextToken();
            }
            break;
            case Operator::NOT:
            {
                UnaryOperation* not = factory.newUnaryOperation(FSM::MODEL::Operator::not);
                _stack.push(not);
                next = nextToken();
            }
            break;
            default:
                poco_assert(false);
                break;
            }
        }
    }
    while (_stack.size() > 1)
        reduce();
    expression = _stack.top();
    _stack.pop();
    return next;
}
const Poco::Token* Parser::parseNext(const Poco::Token* next)
{
    poco_assert(isIdentifier(next));

    StatePtr endstate = map->lookfor(next->tokenString());
    if (endstate == nullptr && next->tokenString() != "nil")
    {
        endstate = factory.newState(next->tokenString());
        map->add(endstate);
    }
    guard->endstate() = endstate;
    next = nextToken();
    return next;
}
const Poco::Token* Parser::parseParameters(const Poco::Token* next)
{
    parameters.clear();
    if (isOperator(next, Operator::OPENPARENT))
    {
        do
        {
            next = nextToken();
            next = parseParameter(next);
            parameters.push_back(parameter);
        }
        while (!isOperator(next, Operator::CLOSPARENT));
        next = nextToken();
    }
    return next;
}
const Poco::Token* Parser::parseParameter(const Poco::Token* next)
{
    poco_assert(isIdentifier(next));
    parameter = factory.newParameter(next->tokenString());
    do
    {
        next = nextToken();
        poco_assert(isOperator(next, Operator::COLON));
        string type;
        for (;;)
        {
            next = nextToken();
            if (isOperator(next, Operator::CLOSPARENT) || isOperator(next, Operator::COMMA))
                break;
            string token = next->tokenString();
            if (token == "const")
                token += ' ';
            type += token;
        }
        parameter->type()= type;
    }
    while (!(isOperator(next, Operator::CLOSPARENT) || isOperator(next, Operator::COMMA)));
    return next;
}
const Poco::Token* Parser::parsePushTransition(const Poco::Token* next)
{
    next = nextToken();
    return next;
}
const Poco::Token* Parser::parsePopTransition(const Poco::Token* next)
{
    next = nextToken();
    return next;
}
const Poco::Token* Parser::parsePopArguments(const Poco::Token* next)
{
    next = nextToken();
    return next;
}
const Poco::Token* Parser::parseActions(const Poco::Token* next, List<ActionPtr>& actions)
{
    poco_assert(isOperator(next, Operator::OPENBRACE));
    const Poco::Token* token = next;
    next = nextToken();
    for (;;)
    {
        if (isOperator(next, Operator::CLOSBRACE))
            break;
        action = nullptr;
        next = parseAction(next);
        actions.push_back(action);
    }
    next = nextToken();
    return next;
}
const Poco::Token* Parser::parseAction(const Poco::Token* next)
{
    poco_assert(isIdentifier(next));
    action = factory.newAction(next->tokenString());
    next = nextToken();
    next = parseArguments(next, action->arguments());
    poco_assert(isOperator(next, Operator::SEMICOLON));
    next = nextToken();
    return next;
}
const Poco::Token* Parser::parseArguments(const Poco::Token* next, List<ArgumentPtr>& arguments)
{
    poco_assert(isOperator(next, Operator::OPENPARENT));
    next = nextToken();
    for (;;)
    {
        if (isOperator(next, Operator::CLOSPARENT))
            break;
        if (isOperator(next, Operator::COMMA))
            next = nextToken();
        next = parseArgument(next, arguments);
    }
    poco_assert(isOperator(next, Operator::CLOSPARENT));
    next = nextToken();
    return next;
}
const Poco::Token* Parser::parseArgument(const Poco::Token* next, List<ArgumentPtr>& arguments)
{
    poco_assert(isIdentifier(next) || isNumber(next) || isString(next) || isCharacter(next));
    string code;
    int count = 0;
    for (;;)
    {
        if (isOperator(next, Operator::COMMA))
            break;
        if (isOperator(next, Operator::CLOSPARENT) && count-- == 0)
        {
            break;
        }
        if (isOperator(next, Operator::OPENPARENT))
            count += 1;
        code += next->tokenString();
        next = nextToken();
    }
    ArgumentPtr argument = factory.newArgument(code);
    arguments.push_back(argument);
    return next;
}
const Poco::Token* Parser::parseRawCode(const Poco::Token* next)
{
    do
    {
        next = nextToken();
    }
    while (!isOperator(next, Operator::CLOSCODE));
    next = nextToken();
    return next;
}
const Poco::Token* Parser::nextToken()
{
    const Poco::Token* next = nextPreprocessed();
    while (!_inFile && !isEOF(next))
        next = nextPreprocessed();
    return next;
}
const Poco::Token* Parser::nextPreprocessed()
{
    const Poco::Token* next = nextParserToken();
    while (next->is(Token::PREPROCESSOR_TOKEN))
    {
        istringstream pps(next->tokenString());
        pps.get();
        Tokenizer ppt(pps);
        const Poco::Token* pPPT = ppt.nextToken();
        if (pPPT->tokenString() == "line" || pPPT->is(Token::INTEGER_LITERAL_TOKEN))
        {
            if (!pPPT->is(Token::INTEGER_LITERAL_TOKEN))
                pPPT = ppt.nextToken();
            int line = pPPT->asInteger();
            _istr.setCurrentLineNumber(line);
            pPPT = ppt.nextToken();
            if (pPPT->is(Token::STRING_LITERAL_TOKEN))
            {
                string path = pPPT->asString();
                Path p(path);
                p.makeAbsolute();
                _currentPath = p.toString();
                _inFile = (Poco::icompare(_path.toString(), _currentPath) == 0);
            }
        }
        next = nextParserToken();
    }
    return next;
}


const Poco::Token* Parser::nextParserToken()
{
    const Poco::Token* next = _tokenizer.nextToken();
    while (next->is(Token::COMMENT_TOKEN) || next->is(Token::SPECIAL_COMMENT_TOKEN))
    {
        if (next->is(Token::SPECIAL_COMMENT_TOKEN))
        {
#if 0
            if (_pCurrentSymbol)
            {
                _pCurrentSymbol->addDocumentation(next->asString());
                _doc.clear();
            }
            else
#endif
                if (_inFile)
                {
                    if (!_doc.empty()) _doc += "\n";
                    _doc += next->asString();
                }
        }
        else if (next->is(Token::COMMENT_TOKEN) && _inFile)
        {
            const string& comment = next->tokenString();
            if (comment.compare(0, 3, "//@") == 0)
            {
                _attrs.append(comment.substr(3));
            }
            else if (comment.compare(0, 11, "// Package:") == 0)
            {
                _package = comment.substr(11);
                Poco::trimInPlace(_package);
            }
            else if (comment.compare(0, 11, "// Library:") == 0)
            {
                _library = comment.substr(11);
                Poco::trimInPlace(_library);
            }
        }
        next = _tokenizer.nextToken();
    }
    return next;
}
void Parser::syntaxError(const string& msg)
{
    throw SyntaxException("Expected", msg);
}
}
}
}
