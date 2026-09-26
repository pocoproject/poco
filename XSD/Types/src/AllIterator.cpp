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
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "Poco/XSD/Types/AllIterator.h"
#include "Poco/XSD/Types/All.h"
#include "Poco/XSD/Types/OrderIterator.h"
#include "Poco/XSD/Types/ElementIterator.h"
#include "Poco/XSD/Types/XSDException.h"


namespace Poco {
namespace XSD {
namespace Types {


AllIterator::AllIterator(const All& all):
	_min(all.getMinOccurs()),
	_cnt(0),
	_mapIt(),
	_itLastUsed(),
	_nexts(),
	_dirtyFlag(true)
{
	All::Content::const_iterator it = all.getContent().begin();
	All::Content::const_iterator itEnd = all.getContent().end();
	for (; it != itEnd; ++it)
	{
		if (it->second->getMaxOccurs() > 0)
		{
			// insert can never fail: all.content is a map, ie. all keys are guaranteed to be unique
			_mapIt.insert(std::make_pair(it->first, new ElementIterator(*const_cast<Element*>(it->second.get()))));
		}
	}

	_itLastUsed = _mapIt.end();
}


AllIterator::~AllIterator()
{
	AllIterator::Content::iterator it = _mapIt.begin();
	AllIterator::Content::iterator itEnd = _mapIt.end();
	for (; it != itEnd; ++it)
	{
		delete it->second;
	}
}


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

	AllIterator::Content::const_iterator it = _mapIt.begin();
	AllIterator::Content::const_iterator itEnd = _mapIt.end();
	for (; it != itEnd; ++it)
	{
		const std::set<std::string>& ins = it->second->validNexts();
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
	AllIterator::Content::iterator it = _mapIt.begin();
	AllIterator::Content::iterator itEnd = _mapIt.end();
	for (; it != itEnd; ++it)
	{
		it->second->close();
	}
	_dirtyFlag = true;
}


bool AllIterator::canClose() const
{
	// we can close if we accessed all mandatory elements
	AllIterator::Content::const_iterator it = _mapIt.begin();
	AllIterator::Content::const_iterator itEnd = _mapIt.end();
	bool can = true;
	for (; it != itEnd; ++it)
	{
		if (!it->second->closed())
			can &= it->second->canClose();
	}

	return can;
}


void AllIterator::reset()
{
	AllIterator::Content::iterator it = _mapIt.begin();
	AllIterator::Content::iterator itEnd = _mapIt.end();
	for (; it != itEnd; ++it)
	{
		it->second->reset();
	}
	_cnt = 0;
	_itLastUsed = itEnd;
	_dirtyFlag = true;
}


} } } // namespace Poco::XSD::Types
