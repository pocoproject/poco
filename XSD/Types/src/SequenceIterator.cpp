//
// SequenceIterator.cpp
//
// Library: XSD/Types
// Package: Iterator
// Module:  SequenceIterator
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/SequenceIterator.h"
#include "Poco/XSD/Types/Sequence.h"
#include "Poco/XSD/Types/XSDException.h"


namespace Poco::XSD::Types {


SequenceIterator::SequenceIterator(const Sequence& data):
	OrderIteratorImpl(),
	_min(data.getMinOccurs()),
	_max(data.getMaxOccurs()),
	_vecIt(),
	_next()
{
	for (const auto& pChild: data.getContent())
		_vecIt.push_back(pChild->iterator());

	poco_assert (_max <= 1); // > 1 not supported yet
}


SequenceIterator::SequenceIterator(const std::vector<OrderIterator>& its, UInt32 min):
	OrderIteratorImpl(),
	_min(min),
	_max(1),
	_vecIt(),
	_next()
{
}


SequenceIterator::~SequenceIterator() = default;


OrderContent::Ptr SequenceIterator::next(const std::string& name)
{
	if (_vecIt.empty())
		throw IllegalOrderException("Element not found:" + name);
	// either access the current element or the next
	if (_curPos == NO_POS)
		_curPos = 0;
	do
	{
		// can we access the current element again?
		if (_vecIt[_curPos].validNext(name))
		{
			// jep
			_dirtyFlag = true;
			return _vecIt[_curPos].next(name);
		}
		// we can't access the current element, check if we fulfil minOccurs restriction
		_vecIt[_curPos].close();

		// we are done with the current element, move to next
		++_curPos;
		_dirtyFlag = true;
	}
	while (_curPos < _vecIt.size());

	throw IllegalOrderException("Element not found:" + name);
}


const std::set<std::string>& SequenceIterator::validNexts() const
{
	if (!_dirtyFlag)
		return _next;

	_next.clear();
	_dirtyFlag = false;
	if (_vecIt.empty())
		return _next;
	bool stop = false;
	Iterators::size_type curPos = _curPos;
	if (curPos == NO_POS)
		curPos = 0;
	// take all elements starting with the current until we have a minOccurs > curCnt situation
	do
	{
		// can we access the current element again?
		const std::set<std::string>& child = _vecIt[curPos].validNexts();
		_next.insert(child.begin(), child.end());
		
		// now: do we have to access the current element or can we move on to the next?
		if (!_vecIt[curPos].canClose())
		{
			stop = true;
		}
		else
		{
			// we are done with the current element, move to next
			++curPos;
		}
	}
	while (curPos < _vecIt.size() && !stop);

	return _next;
}


bool SequenceIterator::end() const
{
	if (_max == 0)
		return true;

	if (_vecIt.empty())
		return true;
	
	if (_curPos == NO_POS)
		return false;

	if (_curPos >= _vecIt.size())
		return true;

	return validNexts().empty();
}


void SequenceIterator::close()
{
	_dirtyFlag = true;
	if (_max == 0 || (_min == 0 && (_curPos == NO_POS || _curPos >= _vecIt.size())))
	{
		_curPos = _vecIt.size();
		return;
	}

	if (_vecIt.empty())
		return;

	if (_curPos == NO_POS)
		_curPos = 0;

	if (_curPos >= _vecIt.size())
		return;

	
	// check that current+all successors are optional
	for (; _curPos < _vecIt.size(); ++_curPos)
		_vecIt[_curPos].close();
}


bool SequenceIterator::canClose() const
{
	if (_max == 0)
		return true;

	if (_min == 0 && _curPos == NO_POS)
		return true;

	if (_vecIt.empty())
		return true;

	if (_curPos >= _vecIt.size())
		return true;

	bool res = true;
	Iterators::size_type curPos = _curPos;
	if (curPos == NO_POS || curPos >= _vecIt.size())
		curPos = 0;
	// check that current+all successors are optional
	for (; curPos < _vecIt.size() && res; ++curPos)
		res &= _vecIt[curPos].canClose();

	return res;
}


bool SequenceIterator::validNext(const std::string& name) const
{
	return (validNexts().find(name) != _next.end());
}


void SequenceIterator::reset()
{
	_curPos = NO_POS;
	_dirtyFlag = true;
	for (auto& itOrd: _vecIt)
		itOrd.reset();
}


} // namespace Poco::XSD::Types
