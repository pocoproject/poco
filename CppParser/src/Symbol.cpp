//
// Symbol.cpp
//
// Library: CppParser
// Package: SymbolTable
// Module:  Symbol
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/CppParser/Symbol.h"
#include "Poco/CppParser/NameSpace.h"
#include "Poco/CppParser/Utility.h"
#include "Poco/String.h"
#include "Poco/Ascii.h"
#include <algorithm>
#include <array>
#include <cctype>
#include <cstddef>
#include <string_view>


namespace Poco::CppParser {


namespace {


constexpr std::string_view TEMPLATE_KEYWORD("template");
constexpr std::string_view CONCEPT_KEYWORD("concept");
constexpr std::string_view REQUIRES_KEYWORD("requires");
constexpr std::string_view DECLTYPE_KEYWORD("decltype");
constexpr std::string_view AND_KEYWORD("and");
constexpr std::string_view OR_KEYWORD("or");
constexpr std::string_view AND_OPERATOR("&&");
constexpr std::string_view OR_OPERATOR("||");
constexpr std::string_view SCOPE_OPERATOR("::");
constexpr std::array<std::string_view, 5> ENCODING_PREFIXES{"", "L", "u", "U", "u8"};
constexpr std::array<std::string_view, 5> RAW_STRING_PREFIXES{"R", "LR", "uR", "UR", "u8R"};


bool isIdentChar(char c)
{
	return Poco::Ascii::isAlphaNumeric(c) || c == '_';
}


std::size_t skipSpace(std::string_view decl, std::size_t pos)
{
	while (pos < decl.size() && Poco::Ascii::isSpace(decl[pos])) ++pos;
	return std::min(pos, decl.size());
}


bool isKeywordAt(std::string_view decl, std::size_t pos, std::string_view keyword)
	/// Returns true if decl contains the keyword as a whole word at pos,
	/// which must not be greater than the size of decl.
{
	const std::string_view rest = decl.substr(pos);
	return rest.starts_with(keyword) && (rest.size() == keyword.size() || !isIdentChar(rest[keyword.size()]));
}


std::size_t skipBalanced(std::string_view decl, std::size_t pos, char open, char close)
	/// Returns the position after the bracket that closes the open
	/// bracket at pos. Brackets in string and character literals do not
	/// count. When open is '<', brackets in a nested (), [] or {} group
	/// and the '>' of "->" do not count either.
{
	poco_assert (pos < decl.size() && decl[pos] == open);

	int depth = 0;
	int groupDepth = 0;
	std::size_t i = pos;
	do
	{
		const char c = decl[i];
		if (c == '"' || c == '\'')
		{
			// an identifier before the quote is an encoding prefix, a number before ' has a digit separator
			std::size_t prefixStart = i;
			while (prefixStart > 0 && isIdentChar(decl[prefixStart - 1])) --prefixStart;
			const std::string_view prefix = decl.substr(prefixStart, i - prefixStart);
			if (c == '"' && std::ranges::find(RAW_STRING_PREFIXES, prefix) != RAW_STRING_PREFIXES.end())
			{
				const std::size_t delimiterEnd = decl.find('(', i);
				const std::string terminator = ")" + std::string(decl.substr(i + 1, delimiterEnd - i - 1)) + "\"";
				const std::size_t terminatorPos = decl.find(terminator, delimiterEnd);
				i = terminatorPos == std::string_view::npos ? decl.size() : terminatorPos + terminator.size() - 1;
			}
			else if (std::ranges::find(ENCODING_PREFIXES, prefix) != ENCODING_PREFIXES.end())
			{
				++i;
				while (i < decl.size() && decl[i] != c)
				{
					if (decl[i] == '\\') ++i;
					++i;
				}
			}
		}
		// in template arguments, '<' and '>' in a nested group are comparisons
		else if (open == '<' && (c == '(' || c == '[' || c == '{'))
			++groupDepth;
		else if (open == '<' && (c == ')' || c == ']' || c == '}'))
			--groupDepth;
		else if (groupDepth == 0 && c == open)
			++depth;
		else if (groupDepth == 0 && c == close && !(open == '<' && decl[i - 1] == '-'))
			--depth;
		++i;
	}
	while (depth > 0 && i < decl.size());
	return std::min(i, decl.size());
}


std::size_t skipQualifiedName(std::string_view decl, std::size_t pos)
	/// Returns the position after the qualified name that begins at pos.
	/// Every part may have template arguments, a part after :: the template
	/// disambiguator, and the first part may be decltype(...).
{
	std::size_t i = pos;
	if (isKeywordAt(decl, i, DECLTYPE_KEYWORD))
	{
		i = skipSpace(decl, i + DECLTYPE_KEYWORD.size());
		if (i < decl.size() && decl[i] == '(')
			i = skipBalanced(decl, i, '(', ')');
	}
	while (true)
	{
		while (i < decl.size() && isIdentChar(decl[i])) ++i;
		std::size_t next = skipSpace(decl, i);
		if (next < decl.size() && decl[next] == '<')
		{
			i = skipBalanced(decl, next, '<', '>');
			next = skipSpace(decl, i);
		}
		if (!decl.substr(next).starts_with(SCOPE_OPERATOR))
			return i;
		i = skipSpace(decl, next + SCOPE_OPERATOR.size());
		if (isKeywordAt(decl, i, TEMPLATE_KEYWORD))
			i = skipSpace(decl, i + TEMPLATE_KEYWORD.size());
	}
}


std::size_t skipRequiresClause(std::string_view decl, std::size_t pos)
	/// Returns the position after the requires clause that begins
	/// at pos (after white space), or pos if there is none.
{
	std::size_t i = skipSpace(decl, pos);
	if (!isKeywordAt(decl, i, REQUIRES_KEYWORD)) return pos;
	i += REQUIRES_KEYWORD.size();
	bool hasMore = true;
	while (hasMore)
	{
		i = skipSpace(decl, i);
		if (i < decl.size() && decl[i] == '(')
		{
			i = skipBalanced(decl, i, '(', ')');
		}
		else if (isKeywordAt(decl, i, REQUIRES_KEYWORD))
		{
			i = skipSpace(decl, i + REQUIRES_KEYWORD.size());
			if (i < decl.size() && decl[i] == '(')
				i = skipSpace(decl, skipBalanced(decl, i, '(', ')'));
			if (i < decl.size() && decl[i] == '{')
				i = skipBalanced(decl, i, '{', '}');
		}
		else
		{
			// a qualified name or true or false
			i = skipQualifiedName(decl, i);
		}
		i = skipSpace(decl, i);
		const std::string_view rest = decl.substr(i);
		if (rest.starts_with(AND_OPERATOR))
			i += AND_OPERATOR.size();
		else if (rest.starts_with(OR_OPERATOR))
			i += OR_OPERATOR.size();
		else if (isKeywordAt(decl, i, AND_KEYWORD))
			i += AND_KEYWORD.size();
		else if (isKeywordAt(decl, i, OR_KEYWORD))
			i += OR_KEYWORD.size();
		else
			hasMore = false;
	}
	return i;
}


} // namespace


int Symbol::_nextId = 0;


Symbol::Symbol():
	_id(_nextId++),
	_pNameSpace(nullptr),
	_access(ACC_PUBLIC),
	_line(-1),
	_order(0)
{
}


Symbol::Symbol(const std::string& name, NameSpace* pNameSpace):
	_id(_nextId++),
	_name(name),
	_pNameSpace(pNameSpace),
	_access(ACC_PUBLIC),
	_line(-1),
	_order(0)
{
	if (_pNameSpace)
		_pNameSpace->addSymbol(this);
}


Symbol::~Symbol()
{
}


void Symbol::setAccess(Access access)
{
	_access = access;
}


void Symbol::setAttributeList(const std::string& attrs)
{
	_attributeList = attrs;
}


void Symbol::setDocumentation(const std::string& text)
{
	_documentation = text;
}


void Symbol::addDocumentation(const std::string& text)
{
	if (!_documentation.empty())
		_documentation.append("\n");
	_documentation.append(text);
}


void Symbol::setFile(const std::string& path)
{
	_file = path;
}


void Symbol::setLineNumber(int line)
{
	_line = line;
}


void Symbol::setPackage(const std::string& package)
{
	_package = package;
}


void Symbol::setLibrary(const std::string& library)
{
	_library = library;
}


void Symbol::setOrder(std::size_t order)
{
	_order = order;
}


std::string Symbol::fullName() const
{
	std::string fullName;
	if (_pNameSpace)
	{
		fullName = _pNameSpace->fullName();
		if (!fullName.empty()) fullName.append("::");
	}
	fullName.append(_name);
	return fullName;
}


std::string Symbol::extractName(const std::string& decl)
{
	poco_assert (!decl.empty());

	// The template parameter list and a requires clause after it can contain
	// '(', '=' and operator names, which do not belong to the declarator.
	const std::string::size_type clauseEnd = templateHeadEnd(decl);

	// A concept definition is any constant expression, so its name is not
	// found by the rules for variables and functions below.
	const std::string::size_type conceptPos = skipSpace(decl, clauseEnd);
	if (clauseEnd > 0 && isKeywordAt(decl, conceptPos, CONCEPT_KEYWORD))
	{
		const std::string::size_type nameStart = skipSpace(decl, conceptPos + CONCEPT_KEYWORD.size());
		std::string::size_type nameEnd = nameStart;
		while (nameEnd < decl.size() && isIdentChar(decl[nameEnd])) ++nameEnd;
		return decl.substr(nameStart, nameEnd - nameStart);
	}

	// special cases: operator () and operator []
	if (decl.find("operator ()", clauseEnd) != std::string::npos)
		return "operator ()";
	else if (decl.find("operator[]", clauseEnd) != std::string::npos)
		return "operator []";

	std::string::size_type pos = decl.find('(', clauseEnd);
	if (pos != std::string::npos)
	{
		// special case std::function<void(int)> a
		//                                ^   ^^
		// In case the marked patterns are found,
		// reset pos to npos to make sure "(" is not seen as the beginning of a function
		int bracket = 1;
		std::string::size_type i = pos + 1;
		while (i < decl.size() && bracket != 0)
		{
			if (decl[i] == '(')
			{
				bracket++;
			}
			else if (decl[i] == ')')
			{
				bracket--;
			}
			i++;
		}

		while (i < decl.size() && std::isspace(decl[i])) i++;
		if (i < decl.size() && decl[i] == '>')
		{
			pos = std::string::npos;
		}
	}

	// another special case: function pointer
	if (pos != std::string::npos && pos < decl.size() - 1)
	{
		std::string::size_type i = pos + 1;
		while (i < decl.size() && std::isspace(decl[i])) i++;
		if (i < decl.size() && decl[i] == '*')
		{
			i++;
			std::string name;
			while (i < decl.size() && std::isspace(decl[i])) i++;
			while (i < decl.size() && !std::isspace(decl[i]) && decl[i] != ')') name += decl[i++];
			return name;
		}
	}
	if (pos == std::string::npos || (pos > 0 && decl[pos - 1] == '('))
		pos = decl.size();
	if (pos > 0) --pos;
	// check for constant; start searching after template and requires clause
	std::string::size_type eqPos = decl.find('=', clauseEnd);
	if (eqPos != std::string::npos)
	{
		// special case: default template parameter
		std::string::size_type gtPos = decl.find('>', eqPos);
		std::string::size_type ltPos = decl.find('<', eqPos);
		if ((gtPos == std::string::npos || gtPos > pos || (ltPos != std::string::npos && gtPos > ltPos)) && eqPos < pos && eqPos > 0 && decl[eqPos + 1] != '=')
			pos = eqPos - 1;
	}
	while (pos > 0 && std::isspace(decl[pos])) --pos;
	while (pos > 0 && decl[pos] == ']')
	{
		--pos;
		while (pos > 0 && decl[pos] != '[') --pos;
		if (pos > 0) --pos;
		while (pos > 0 && std::isspace(decl[pos])) --pos;
	}
	// iterate over template (specialization)
	int nestedTemplateCount = 0;
	if (pos > 1 && decl[pos] == '>' && decl[pos-1] != '-' && decl[pos-1] != '>') // the operators ->, >>
	{
		--pos;
		++nestedTemplateCount;
		while (pos > 0 && nestedTemplateCount != 0)
		{
			if (decl[pos] == '<')
				--nestedTemplateCount;
			if (decl[pos] == '>')
				++nestedTemplateCount;
			--pos;
		}
		while (pos > 0 && std::isspace(decl[pos])) --pos;
	}
	std::string::size_type end = pos;
	std::string::size_type op = decl.find("operator ", clauseEnd);
	if (op != std::string::npos && (op == 0 || std::isspace(decl[op - 1]) || decl[op - 1] == ':'))
	{
		pos = op;
	}
	else
	{
		while (pos > 0 && !isIdent(decl[pos])) --pos;
		while (pos > 0 && std::isspace(decl[pos])) --pos;
		while (pos > 0 && isIdent(decl[pos - 1])) --pos;
		if (pos > 0 && decl[pos - 1] == '~') --pos;
	}

	while (pos > 2 && decl[pos - 1] == ':')
	{
		pos -= 3;
		while (pos > 0 && isIdent(decl[pos - 1])) --pos;
	}
	if (pos != std::string::npos)
		return decl.substr(pos, end - pos + 1);
	else
		return std::string();
}


bool Symbol::isIdent(char c)
{
	return isIdentChar(c);
}


bool Symbol::hasAttr(const std::string& decl, const std::string& attr)
{
	std::string attrS(attr);
	attrS += ' ';
	std::string::size_type pos = decl.find(attrS);
	return pos == 0 || (pos != std::string::npos && decl[pos - 1] == ' ');
}


std::size_t Symbol::templateHeadEnd(std::string_view decl)
{
	if (!isKeywordAt(decl, 0, TEMPLATE_KEYWORD)) return 0;
	const std::size_t listStart = skipSpace(decl, TEMPLATE_KEYWORD.size());
	if (listStart == decl.size() || decl[listStart] != '<') return 0;
	return skipRequiresClause(decl, skipBalanced(decl, listStart, '<', '>'));
}


void Symbol::setAttributes(const Attributes& attrs)
{
	_attrs = attrs;
}


const Attributes& Symbol::getAttributes() const
{
	return _attrs;
}


} // namespace Poco::CppParser
