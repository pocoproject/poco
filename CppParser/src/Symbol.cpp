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
#include <cctype>
#include <cstddef>
#include <string_view>


namespace Poco::CppParser {


static constexpr std::string_view CONCEPT_KEYWORD("concept");
static constexpr std::string_view REQUIRES_KEYWORD("requires");
static constexpr std::string_view AND_KEYWORD("and");
static constexpr std::string_view OR_KEYWORD("or");
static constexpr std::string_view TEMPLATE_DISAMBIGUATOR("::template");


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

	std::string::size_type templateEnd = 0;
	if (decl.compare(0, 8, "template") == 0)
	{
		templateEnd = 8;
		while (templateEnd < decl.size() && Poco::Ascii::isSpace(decl[templateEnd])) ++templateEnd;
		if (templateEnd < decl.size() && decl[templateEnd] == '<')
			templateEnd = skipBalanced(decl, templateEnd, '<', '>');
	}
	// The template parameter list and a requires clause after it can contain
	// '(', '=' and operator names, which do not belong to the declarator.
	const std::string::size_type clauseEnd = skipRequiresClause(decl, templateEnd);

	// A concept definition is any constant expression, so its name is not
	// found by the rules for variables and functions below.
	std::string::size_type conceptPos = clauseEnd;
	while (conceptPos < decl.size() && Poco::Ascii::isSpace(decl[conceptPos])) ++conceptPos;
	if (templateEnd > 0 && isKeywordAt(decl, conceptPos, CONCEPT_KEYWORD))
	{
		std::string::size_type nameStart = conceptPos + CONCEPT_KEYWORD.size();
		while (nameStart < decl.size() && Poco::Ascii::isSpace(decl[nameStart])) ++nameStart;
		std::string::size_type nameEnd = nameStart;
		while (nameEnd < decl.size() && isIdent(decl[nameEnd])) ++nameEnd;
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
	return std::isalnum(c) || c == '_';
}


bool Symbol::hasAttr(const std::string& decl, const std::string& attr)
{
	std::string attrS(attr);
	attrS += ' ';
	std::string::size_type pos = decl.find(attrS);
	return pos == 0 || (pos != std::string::npos && decl[pos - 1] == ' ');
}


bool Symbol::isKeywordAt(const std::string& decl, std::string::size_type pos, std::string_view keyword)
{
	const std::string::size_type end = pos + keyword.size();
	return decl.compare(pos, keyword.size(), keyword) == 0 && (end == decl.size() || !isIdent(decl[end]));
}


std::string::size_type Symbol::skipBalanced(const std::string& decl, std::string::size_type pos, char open, char close)
{
	poco_assert (pos < decl.size() && decl[pos] == open);

	const auto isLiteralStart = [&decl](std::string::size_type i)
	{
		if (decl[i] != '"' && decl[i] != '\'') return false;
		// an identifier before the quote is an encoding prefix, a number before ' has a digit separator
		std::string::size_type prefixStart = i;
		while (prefixStart > 0 && isIdent(decl[prefixStart - 1])) --prefixStart;
		const std::string_view prefix = std::string_view(decl).substr(prefixStart, i - prefixStart);
		return prefix.empty() || prefix == "L" || prefix == "u" || prefix == "U" || prefix == "u8";
	};

	int depth = 0;
	int parenDepth = 0;
	std::string::size_type i = pos;
	do
	{
		const char c = decl[i];
		if (isLiteralStart(i))
		{
			++i;
			while (i < decl.size() && decl[i] != c)
			{
				if (decl[i] == '\\') ++i;
				++i;
			}
		}
		// in template arguments, '<' and '>' in parentheses are comparisons
		else if (open == '<' && c == '(')
			++parenDepth;
		else if (open == '<' && c == ')')
			--parenDepth;
		else if (parenDepth == 0 && c == open)
			++depth;
		else if (parenDepth == 0 && c == close)
			--depth;
		++i;
	}
	while (depth > 0 && i < decl.size());
	return std::min(i, decl.size());
}


std::string::size_type Symbol::skipRequiresClause(const std::string& decl, std::string::size_type pos)
{
	const auto skipSpace = [&decl](std::string::size_type i)
	{
		while (i < decl.size() && Poco::Ascii::isSpace(decl[i])) ++i;
		return i;
	};
	const auto skipNameChars = [&decl](std::string::size_type i)
	{
		while (i < decl.size() && (isIdent(decl[i]) || decl[i] == ':')) ++i;
		return i;
	};
	const auto skipName = [&decl, &skipSpace, &skipNameChars](std::string::size_type i)
	{
		i = skipNameChars(i);
		// the name after T::template is separated by a space
		if (i >= TEMPLATE_DISAMBIGUATOR.size() &&
			decl.compare(i - TEMPLATE_DISAMBIGUATOR.size(), TEMPLATE_DISAMBIGUATOR.size(), TEMPLATE_DISAMBIGUATOR) == 0)
			i = skipNameChars(skipSpace(i));
		return i;
	};
	const auto skipOperator = [&decl](std::string::size_type i)
	{
		if (decl.compare(i, 2, "&&") == 0 || decl.compare(i, 2, "||") == 0)
			return i + 2;
		else if (isKeywordAt(decl, i, AND_KEYWORD))
			return i + AND_KEYWORD.size();
		else if (isKeywordAt(decl, i, OR_KEYWORD))
			return i + OR_KEYWORD.size();
		else
			return i;
	};

	std::string::size_type i = skipSpace(pos);
	if (!isKeywordAt(decl, i, REQUIRES_KEYWORD)) return pos;
	i += REQUIRES_KEYWORD.size();
	bool hasMore = true;
	while (hasMore)
	{
		i = skipSpace(i);
		if (i < decl.size() && decl[i] == '(')
		{
			i = skipBalanced(decl, i, '(', ')');
		}
		else if (isKeywordAt(decl, i, REQUIRES_KEYWORD))
		{
			i = skipSpace(i + REQUIRES_KEYWORD.size());
			if (i < decl.size() && decl[i] == '(')
				i = skipSpace(skipBalanced(decl, i, '(', ')'));
			if (i < decl.size() && decl[i] == '{')
				i = skipBalanced(decl, i, '{', '}');
		}
		else
		{
			// a qualified name or true or false; template arguments may follow every part
			i = skipSpace(skipName(i));
			while (i < decl.size() && decl[i] == '<')
				i = skipSpace(skipName(skipBalanced(decl, i, '<', '>')));
		}
		i = skipSpace(i);
		const std::string::size_type next = skipOperator(i);
		hasMore = next != i;
		i = next;
	}
	return i;
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
