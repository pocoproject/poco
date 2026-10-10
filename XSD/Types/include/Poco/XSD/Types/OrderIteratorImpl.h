//
// OrderIteratorImpl.h
//
// Library: XSD/Types
// Package: Iterator
// Module:  OrderIteratorImpl
//
// Definition of the OrderIteratorImpl class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_OrderIteratorImpl_INCLUDED
#define XSDTypes_OrderIteratorImpl_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/OrderContent.h"
#include "Poco/RefCountedObject.h"
#include "Poco/AutoPtr.h"
#include <set>


namespace Poco::XSD::Types {


class XSDTypes_API OrderIteratorImpl: public Poco::RefCountedObject
	/// OrderIteratorImpl is the interface for all different Iterator implementations.
{
public:
	using Ptr = AutoPtr<OrderIteratorImpl>;

	OrderIteratorImpl();
		/// Creates the OrderIteratorImpl.

	~OrderIteratorImpl() override;
		/// Destroys the OrderIteratorImpl.

	virtual OrderContent::Ptr next(const std::string& name) = 0;
		/// Returns the next OrderContent with the given name.
		/// Will fail with an IllegalOrderException if name is not part of
		/// the set returned by validNexts().

	[[nodiscard]] virtual const std::set<std::string>& validNexts() const = 0;
		/// Returns the valid names for the next call.

	[[nodiscard]] virtual bool end() const = 0;
		/// Returns true if at end of iteration. 

	[[nodiscard]] virtual bool validNext(const std::string& name) const = 0;
		/// Returns true if the element with the given name is a valid next element.

	[[nodiscard]] virtual bool canClose() const = 0;
		/// Checks if it is safe to close the iterator, i.e. minoccurs restriction are satisfied.

	[[nodiscard]] bool closed() const;
		/// Returns true if closed was called for this iterator.

	void resetImpl();
		/// Resets an iterator to its initial state.

	void closeImpl();
		/// Closes an iterator. Use this method to indicate that you are finished with iteration
		/// and we want to detect that you are at the end of iteration (ie minoccurs restriction are satisified).

protected:
	virtual void reset() = 0;
		/// Resets an iterator to its initial state.

	virtual void close() = 0;
		/// Closes an iterator. Use this method to indicate that you are finished with iteration
		/// and we want to detect that you are at the end of iteration (ie minoccurs restriction are satisified).

private:
	bool _closed = false;
};


//
// inlines
//
inline bool OrderIteratorImpl::closed() const
{
	return _closed;
}


inline void OrderIteratorImpl::resetImpl()
{
	reset();
	_closed = false;
}


inline void OrderIteratorImpl::closeImpl()
{
	close();
	_closed = true;
}


} // namespace Poco::XSD::Types


#endif // XSDTypes_OrderIteratorImpl_INCLUDED
