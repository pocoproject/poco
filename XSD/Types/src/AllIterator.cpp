//
// AllIterator.cpp
//
// Library: XSD/Types
// Package: Iterator
// Module:  AllIterator
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/AllIterator.h"
#include "Poco/XSD/Types/All.h"
#include "Poco/XSD/Types/OrderIterator.h"
#include "Poco/XSD/Types/ElementIterator.h"
#include "Poco/XSD/Types/XSDException.h"


namespace Poco::XSD::Types {


AllIterator::AllIterator(const All& all):
	_min(all.getMinOccurs()),
	_mapIt(),
	_itLastUsed(),
	_nexts()
{
	for (const auto& [name, pElem]: all.getContent())
	{
		if (pElem->getMaxOccurs() > 0)
		{
			// insert can never fail: all.content is a map, ie. all keys are guaranteed to be unique
			_mapIt.try_emplace(name, makeAuto<ElementIterator>(*const_cast<Element*>(pElem.get())));
		}
	}

	_itLastUsed = _mapIt.end();
}


AllIterator::~AllIterator() = default;


OrderContent::Ptr AllIterator::next(const std::string& name)
{
	_dirtyFlag = true;

	if (_itLastUsed != _mapIt.end())
	{
		if (_itLastUsed->first == name && _itLastUsed->second->validNext(name))
		{
			return _itLastUsed->second->next(name);
		}
		else
		{
			// disable the current one
			_itLastUsed->second->close();
		}
	}

	_itLastUsed = _mapIt.find(name);
	if (_itLastUsed == _mapIt.end())
		throw IllegalOrderException("AllIterator: " + name);
	++_cnt;
	return _itLastUsed->second->next(name);
}


const std::set<std::string>& AllIterator::validNexts() const
{
	if (!_dirtyFlag)
		return _nexts;

	_nexts.clear();
	_dirtyFlag = false;

	for (const auto& [name, pElemIt]: _mapIt)
	{
		const std::set<std::string>& ins = pElemIt->validNexts();
		_nexts.insert(ins.begin(), ins.end());
	}

	return _nexts;
}


bool AllIterator::end() const
{
	return validNexts().empty();
}


bool AllIterator::validNext(const std::string& name) const
{
	return (validNexts().find(name) != _nexts.end());
}


void AllIterator::close()
{
	// we can close if we accessed all mandatory elements
	for (auto& [name, pElemIt]: _mapIt)
	{
		pElemIt->close();
	}
	_dirtyFlag = true;
}


bool AllIterator::canClose() const
{
	// we can close if we accessed all mandatory elements
	bool can = true;
	for (const auto& [name, pElemIt]: _mapIt)
	{
		if (!pElemIt->closed())
			can &= pElemIt->canClose();
	}

	return can;
}


void AllIterator::reset()
{
	for (auto& [name, pElemIt]: _mapIt)
	{
		pElemIt->reset();
	}
	_cnt = 0;
	_itLastUsed = _mapIt.end();
	_dirtyFlag = true;
}


} // namespace Poco::XSD::Types
