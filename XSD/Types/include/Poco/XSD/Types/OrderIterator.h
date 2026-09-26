//
// OrderIterator.h
//
// Library: XSD/Types
// Package: Iterator
// Module:  OrderIterator
//
// Definition of the OrderIterator class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_OrderIterator_INCLUDED
#define XSDTypes_OrderIterator_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/OrderContent.h"
#include "Poco/XSD/Types/OrderIteratorImpl.h"
#include <set>


namespace Poco {
namespace XSD {
namespace Types {


class XSDTypes_API OrderIterator
	/// An iterator type for iterating over the content of XML schema types.
{
public:
	OrderIterator();
		/// Creates the uninitialized OrderIterator. 
		///
		/// Note that end() will return true for uninitialized iterators.

	OrderIterator(OrderIteratorImpl::Ptr pImpl);
		/// Creates the OrderIterator using the given OrderIteratorImpl.

	~OrderIterator();
		/// Destroys the OrderIterator.

	OrderContent::Ptr next(const std::string& name);
		/// Returns the next OrderContent with the given name.
		///
		/// Will fail with an IllegalOrderException if name is not part of
		/// the set returned by validNexts().

	const std::set<std::string>& validNexts() const;
		/// Returns the valid names for the next call. If the set contains the string "*",
		/// it will accept any string.

	bool validNext(const std::string& name) const;
		/// Returns true if the element with the given name is a valid next element.

	bool end() const;
		/// Returns true if at end of iteration. This is the only method which
		/// is safe to call on uninitialized OrderIterators.

	void close();
		/// Closes an iterator. Use this method to indicate that you are finished with iteration
		/// and we want to detect that you are at the end of iteration (i.e. minoccurs restriction are satisfied).
		///
		/// Will throw an IllegalOrderException when not safe to close.

	bool canClose() const;
		/// Checks if it is safe to close the iterator, i.e. minoccurs restriction are satisfied.

	bool closed() const;
		/// Returns true if closed was called for this iterator.

	void reset();
		/// Resets an iterator to its initial state.

private:
	OrderIteratorImpl::Ptr _pImpl;
};


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_OrderIterator_INCLUDED
