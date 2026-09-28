//
// SequenceIterator.h
//
// Library: XSD/Types
// Package: Iterator
// Module:  SequenceIterator
//
// Definition of the SequenceIterator class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_SequenceIterator_INCLUDED
#define XSDTypes_SequenceIterator_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/OrderIteratorImpl.h"
#include "Poco/XSD/Types/OrderIterator.h"
#include <vector>
#include <limits>


POCO_CHECK_MINMAX_MACROS


namespace Poco::XSD::Types {


class Sequence;


class XSDTypes_API SequenceIterator: public OrderIteratorImpl
	/// An iterator for iterating over Sequence content groups.
{
public:
	explicit SequenceIterator(const Sequence& data);
		/// Creates the SequenceIterator.

	SequenceIterator(const std::vector<OrderIterator>& its, UInt32 min);
		/// Creates the SequenceIterator.

	~SequenceIterator() override;
		/// Destroys the SequenceIterator.

	// OrderIteratorImpl
	OrderContent::Ptr next(const std::string& name) override;
	[[nodiscard]] const std::set<std::string>& validNexts() const override;
	[[nodiscard]] bool end() const override;
	[[nodiscard]] bool validNext(const std::string& name) const override;
	void close() override;
	[[nodiscard]] bool canClose() const override;
	void reset() override;

private:
	using Iterators = std::vector<OrderIterator>;

	static constexpr Iterators::size_type NO_POS = std::numeric_limits<Iterators::size_type>::max();
		/// Value of _curPos before an element has been chosen.

	UInt32 _min;
	UInt32 _max;
	Iterators _vecIt;
	Iterators::size_type _curPos = NO_POS;
	mutable std::set<std::string> _next;
	mutable bool _dirtyFlag = true;
};


} // namespace Poco::XSD::Types


#endif // XSDTypes_SequenceIterator_INCLUDED
