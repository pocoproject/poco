//
// AnyIterator.h
//
// Library: XSD/Types
// Package: Iterator
// Module:  AnyIterator
//
// Definition of the AnyIterator class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_AnyIterator_INCLUDED
#define XSDTypes_AnyIterator_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/OrderIteratorImpl.h"


namespace Poco::XSD::Types {


class Any;


class XSDTypes_API AnyIterator: public OrderIteratorImpl
	/// An iterator for iterating over Any content groups.
{
public:
	explicit AnyIterator(Any& any);
		/// Creates the AnyIterator.

	~AnyIterator() override;
		/// Destroys the AnyIterator.

	// OrderIteratorImpl
	OrderContent::Ptr next(const std::string& name) override;
	[[nodiscard]] const std::set<std::string>& validNexts() const override;
	[[nodiscard]] bool end() const override;
	[[nodiscard]] bool validNext(const std::string& name) const override;
	void close() override;
	[[nodiscard]] bool canClose() const override;
	void reset() override;

private:
	UInt32 _min;
	UInt32 _max;
	UInt32 _cnt = 0;
	Any*   _pAny;
	mutable std::set<std::string> _next;
};


} // namespace Poco::XSD::Types


#endif // XSDTypes_AnyIterator_INCLUDED
