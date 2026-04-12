//
// Function.cpp
//
// Library: CppParser
// Package: SymbolTable
// Module:  Function
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/CppParser/Function.h"
#include "Poco/CppParser/Parameter.h"
#include "Poco/CppParser/NameSpace.h"
#include "Poco/CppParser/Struct.h"
#include "Poco/CppParser/Utility.h"
#include "Poco/String.h"
#include <sstream>
#include <cctype>
#include <cstddef>


namespace Poco {
namespace CppParser {


Function::Function(const std::string& decl, NameSpace* pNameSpace):
	Decl(decl, pNameSpace),
	_flags(0),
	_retParam()
{
	if (hasAttr(decl, "static"))
		_flags |= FN_STATIC;
	if (hasAttr(decl, "virtual"))
		_flags |= FN_VIRTUAL;
	if (hasAttr(decl, "inline"))
		_flags |= FN_INLINE;
	if (hasAttr(decl, "template"))
		_flags |= FN_TEMPLATE;

	if (isMethod() || isFunction())
	{
		// parse the decl
		std::size_t pos = decl.rfind(name());
		_retParam = decl.substr(0, pos-1);
		// eliminate static, virtual, inline, template
		_retParam = replace(_retParam, "static ", "");
		_retParam = replace(_retParam, "virtual ", "");
		_retParam = replace(_retParam, "inline ", "");
		if (_flags & FN_TEMPLATE)
		{
			std::size_t pos2 = _retParam.find(">");
			poco_assert (pos2 != std::string::npos);
			_retParam = _retParam.substr(pos2+1);
		}
		Poco::trimInPlace(_retParam);
	}
}


Function::~Function()
{
	for (const auto* pParam : _params)
	{
		delete pParam;
	}
}


void Function::addParameter(Parameter* pParam)
{
	_params.push_back(pParam);
}


Function::Iterator Function::begin() const
{
	return _params.begin();
}


Function::Iterator Function::end() const
{
	return _params.end();
}


void Function::makeInline()
{
	_flags |= FN_INLINE;
}


void Function::makeConst()
{
	_flags |= FN_CONST;
}


void Function::makePureVirtual()
{
	_flags |= FN_PURE_VIRTUAL;
}


void Function::makeFinal()
{
	_flags |= FN_FINAL;
}


void Function::makeOverride()
{
	_flags |= FN_OVERRIDE;
}


void Function::makeNoexcept()
{
	_flags |= FN_NOEXCEPT;
}


void Function::makeDefault()
{
	_flags |= FN_DEFAULT;
}


void Function::makeDelete()
{
	_flags |= FN_DELETE;
}


bool Function::isConstructor() const
{
	return name() == nameSpace()->name();
}


bool Function::isDestructor() const
{
	return name()[0] == '~';
}


bool Function::isMethod() const
{
	return !isConstructor() && !isDestructor() && nameSpace()->kind() == Symbol::SYM_STRUCT;
}


bool Function::isFunction() const
{
	return nameSpace()->kind() == Symbol::SYM_NAMESPACE;
}


int Function::countParameters() const
{
	return (int) _params.size();
}


Symbol::Kind Function::kind() const
{
	return Symbol::SYM_FUNCTION;
}


std::string Function::signature() const
{
	std::string sig(declaration());
	if (sig.compare(0, 8, "virtual ") == 0)
		sig.erase(0, 8);
	else if (sig.compare(0, 7, "static ") == 0)
		sig.erase(0, 8);
	if (sig.compare(0, 7, "inline ") == 0)
		sig.erase(0, 7);
	sig += "(";
	bool isFirst = true;
	for (const auto* pParam : _params)
	{
		if (isFirst)
			isFirst = false;
		else
			sig += ", ";
		const auto arg = pParam->declaration();
		auto pos = arg.size() - 1;
		while (pos > 0 && !std::isspace(arg[pos])) --pos;
		while (pos > 0 && std::isspace(arg[pos])) --pos;
		sig.append(arg, 0, pos + 1);
	}
	sig += ")";
	if (_flags & FN_CONST)
		sig += " const";
	return sig;
}


bool Function::isVirtual() const
{
	if (_flags & FN_VIRTUAL)
	{
		return true;
	}
	else if (isDestructor())
	{
		// isDestructor() implies nameSpace()->kind() == SYM_STRUCT
		const auto* pClass = static_cast<const Struct*>(nameSpace());
		return pClass->hasVirtualDestructor();
	}
	else return getOverridden() != nullptr;
}


Function* Function::getOverridden() const
{
	if (isMethod() && !(_flags & FN_STATIC))
	{
		// isMethod() guarantees nameSpace()->kind() == SYM_STRUCT
		const auto* pClass = static_cast<const Struct*>(nameSpace());
		for (auto it = pClass->baseBegin(); it != pClass->baseEnd(); ++it)
		{
			if (it->pClass)
			{
				auto* pOverridden = it->pClass->findFunction(signature());
				if (pOverridden && pOverridden->isVirtual())
					return pOverridden;
			}
		}
	}
	return nullptr;
}


std::string Function::toString() const
{
	std::ostringstream ostr;
	ostr << Decl::toString() << "(\n";
	for (const auto* pParam : _params)
	{
		ostr << "\t" << pParam->toString() << "\n";
	}
	ostr << ");";
	return ostr.str();
}


} } // namespace Poco::CppParser
