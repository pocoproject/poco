//
// ChoiceIterator.cpp
//
// Library: XSD/Types
// Package: Iterator
// Module:  ChoiceIterator
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/ChoiceIterator.h"
#include "Poco/XSD/Types/Choice.h"
#include "Poco/XSD/Types/OrderIterator.h"
#include "Poco/XSD/Types/XSDException.h"


namespace Poco::XSD::Types {


ChoiceIterator::ChoiceIterator(const Choice& choice):
	_min(choice.getMinOccurs()),
	_max(choice.getMaxOccurs()),
	_vecIt(),
	_next()
{
	for (const auto& pChild: choice.getContent())
	{
		OrderIterator itOrd = pChild->iterator();
		if (!itOrd.validNexts().empty())
			_vecIt.push_back(itOrd);
	}
}


ChoiceIterator::~ChoiceIterator() = default;


OrderContent::Ptr ChoiceIterator::next(const std::string& name)
{
	if (_lastChosenPos != NO_CHOICE)
	{
		if (!_vecIt[_lastChosenPos].validNext(name))
		{
			_vecIt[_lastChosenPos].close();
			_vecIt[_lastChosenPos].reset();
		}
		else
		{
			_dirtyFlag = true;
			return _vecIt[_lastChosenPos].next(name);
		}
	}

	if (_chosenElementCnt >= _max)
		throw IllegalOrderException("MaxOccurs exceeded for choice");

	for (Iterators::size_type i = 0; i < _vecIt.size(); ++i)
	{
		if (_vecIt[i].validNext(name))
		{
			++_chosenElementCnt;
			_lastChosenPos = i;
			_dirtyFlag = true;
			return _vecIt[_lastChosenPos].next(name);
		}
	}

	throw IllegalOrderException("Element not found in choice: " + name);
}


const std::set<std::string>& ChoiceIterator::validNexts() const
{
	if (!_dirtyFlag)
		return _next;

	_dirtyFlag = false;
	_next.clear();
	bool addAllOthers = true;
	if (_lastChosenPos != NO_CHOICE)
	{
		const std::set<std::string>& child = _vecIt[_lastChosenPos].validNexts();
		if (child.empty() && _chosenElementCnt < _max && _vecIt[_lastChosenPos].canClose())
		{
			// reset
			_vecIt[_lastChosenPos].close();
			_vecIt[_lastChosenPos].reset();
			_lastChosenPos = NO_CHOICE;
		}
		else
		{
			_next.insert(child.begin(), child.end());
			addAllOthers = _vecIt[_lastChosenPos].canClose();
		}
	}

	if (_chosenElementCnt >= _max)
		return _next;

	if (addAllOthers)
	{
		for (const auto& itOrd: _vecIt)
		{
			const std::set<std::string>& child = itOrd.validNexts();
			_next.insert(child.begin(), child.end());
		}
	}

	return _next;
}


bool ChoiceIterator::end() const
{
	if (_vecIt.empty())
		return true;

	return (_chosenElementCnt >= _max);
}


bool ChoiceIterator::validNext(const std::string& name) const
{
	return (validNexts().find(name) != _next.end());
}


void ChoiceIterator::close()
{
	if (_vecIt.empty())
		return;

	_dirtyFlag = true;

	if (_lastChosenPos != NO_CHOICE)
	{
		_vecIt[_lastChosenPos].close();
	}

	if (_chosenElementCnt < _min)
		throw IllegalOrderException("MinOccurs exceeded for choice");

	_chosenElementCnt = _max;
}


bool ChoiceIterator::canClose() const
{
	if (_vecIt.empty())
		return true;

	bool ok = true;
	if (_lastChosenPos != NO_CHOICE)
	{
		ok = _vecIt[_lastChosenPos].canClose();
	}

	ok &= (_chosenElementCnt >= _min);
	return ok;
}


void ChoiceIterator::reset()
{
	_lastChosenPos = NO_CHOICE;
	_dirtyFlag = true;
	_chosenElementCnt = 0;
	for (auto& itOrd: _vecIt)
		itOrd.reset();
}


} // namespace Poco::XSD::Types
