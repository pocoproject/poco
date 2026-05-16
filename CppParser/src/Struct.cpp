//
// Struct.cpp
//
// Library: CppParser
// Package: SymbolTable
// Module:  Struct
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/CppParser/Struct.h"
#include "Poco/CppParser/Function.h"
#include "Poco/String.h"
#include <algorithm>
#include <sstream>
#include <cstddef>


namespace Poco::CppParser {


Struct::Struct(const std::string& decl, bool isClass, NameSpace* pNameSpace):
	NameSpace(extractName(decl), pNameSpace),
	_decl(decl),
	_flags(0),
	_isClass(isClass)
{
	std::size_t pos = decl.find("template");
	if (pos != std::string::npos)
	{
		_flags |= FN_TEMPLATE;
		std::size_t templTypeStart = decl.find("<", pos);
		std::size_t templTypeEnd = decl.find(">", templTypeStart);
		if (templTypeStart != std::string::npos && templTypeEnd != std::string::npos)
		{
			std::string templParam = decl.substr(templTypeStart+1, templTypeEnd-templTypeStart-1);
			Poco::trimInPlace(templParam);
			if (templParam.empty())
				_flags |= FN_TEMPLATE_SPECIALIZATION;
		}
	}
}


Struct::~Struct()
{
}


void Struct::addBase(const std::string& name, Symbol::Access access, bool isVirtual)
{
	Base base;
	base.name      = name;
	base.access    = access;
	base.isVirtual = isVirtual;
	base.pClass = nullptr;
	_bases.push_back(base);
}


Struct::BaseIterator Struct::baseBegin() const
{
	return _bases.begin();
}


Struct::BaseIterator Struct::baseEnd() const
{
	return _bases.end();
}


void Struct::addDerived(Struct* pClass)
{
	poco_check_ptr (pClass);

	_derived.push_back(pClass);
}


void Struct::fixupBases()
{
	for (auto& base : _bases)
	{
		auto* pSym = lookup(base.name);
		base.pClass = (pSym && pSym->kind() == Symbol::SYM_STRUCT) ? static_cast<Struct*>(pSym) : nullptr;
		if (base.pClass)
			base.pClass->addDerived(this);
	}
}


Struct::DerivedIterator Struct::derivedBegin() const
{
	return _derived.begin();
}


Struct::DerivedIterator Struct::derivedEnd() const
{
	return _derived.end();
}


namespace
{
	struct CPLT
	{
		bool operator() (const Function* pF1, const Function* pF2) const
		{
			int pc1 = pF1->countParameters();
			int pc2 = pF2->countParameters();
			return pc1 < pc2;
		}
	};
};


void Struct::constructors(Functions& functions) const
{
	for (const auto& [name, pSym] : *this)
	{
		if (pSym->kind() == Symbol::SYM_FUNCTION)
		{
			auto* pFunc = static_cast<Function*>(pSym);
			if (pFunc->isConstructor())
				functions.push_back(pFunc);
		}
	}
	std::sort(functions.begin(), functions.end(), CPLT());
}


void Struct::bases(std::set<std::string>& bases) const
{
	for (const auto& base : _bases)
	{
		if (base.pClass)
		{
			bases.insert(base.pClass->fullName());
			base.pClass->bases(bases);
		}
		else bases.insert(base.name);
	}
}


Function* Struct::destructor() const
{
	for (const auto& [name, pSym] : *this)
	{
		if (pSym->kind() == Symbol::SYM_FUNCTION)
		{
			auto* pFunc = static_cast<Function*>(pSym);
			if (pFunc->isDestructor())
				return pFunc;
		}
	}
	return nullptr;
}


void Struct::methods(Symbol::Access access, Functions& functions) const
{
	for (const auto& [name, pSym] : *this)
	{
		if (pSym->kind() == Symbol::SYM_FUNCTION)
		{
			auto* pFunc = static_cast<Function*>(pSym);
			if (pFunc->getAccess() == access && pFunc->isMethod())
				functions.push_back(pFunc);
		}
	}
}


void Struct::inheritedMethods(FunctionSet& functions) const
{
	for (const auto& base : _bases)
	{
		const auto* pBase = base.pClass;
		if (pBase)
		{
			for (const auto& [name, pSym] : *pBase)
			{
				if (pSym->kind() == Symbol::SYM_FUNCTION)
				{
					auto* pFunc = static_cast<Function*>(pSym);
					if (pFunc->getAccess() != Symbol::ACC_PRIVATE && pFunc->isMethod())
						functions.insert(pFunc);
				}
			}
			pBase->inheritedMethods(functions);
		}
	}
}


void Struct::derived(StructSet& derived) const
{
	for (auto* pDerived : _derived)
	{
		derived.insert(pDerived);
		pDerived->derived(derived);
	}
}


Symbol::Kind Struct::kind() const
{
	return Symbol::SYM_STRUCT;
}


Function* Struct::findFunction(const std::string& signature) const
{
	for (const auto& [name, pSym] : *this)
	{
		if (pSym->kind() == Symbol::SYM_FUNCTION)
		{
			auto* pFunc = static_cast<Function*>(pSym);
			if (pFunc->signature() == signature)
				return pFunc;
		}
	}
	for (const auto& base : _bases)
	{
		if (base.pClass)
		{
			auto* pFunc = base.pClass->findFunction(signature);
			if (pFunc) return pFunc;
		}
	}
	return nullptr;
}


bool Struct::hasVirtualDestructor() const
{
	const auto* pFunc = destructor();
	if (pFunc && (pFunc->flags() & Function::FN_VIRTUAL))
		return true;
	for (const auto& base : _bases)
	{
		if (base.pClass && base.pClass->hasVirtualDestructor())
			return true;
	}
	return false;
}


std::string Struct::toString() const
{
	std::ostringstream ostr;
	ostr << declaration() << "\n{\n";
	for (const auto& [name, pSym] : *this)
	{
		ostr << pSym->fullName() << "\n";
		ostr << pSym->toString() << "\n";
	}
	ostr << "};\n";
	return ostr.str();
}


Symbol* Struct::lookup(const std::string& name) const
{
	auto* pSymbol = NameSpace::lookup(name);
	if (!pSymbol)
	{
		for (const auto& base : _bases)
		{
			if (base.access != Symbol::ACC_PRIVATE)
			{
				if (base.pClass)
				{
					pSymbol = base.pClass->lookup(name);
					if (pSymbol)
					{
						return pSymbol;
					}
				}
			}
		}
	}
	return pSymbol;
}


} // namespace Poco::CppParser
