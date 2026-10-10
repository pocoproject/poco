//
// ElementIterator.h
//
// Library: XSD/Types
// Package: Iterator
// Module:  ElementIterator
//
// Definition of the ElementIterator class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_ElementIterator_INCLUDED
#define XSDTypes_ElementIterator_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/OrderIteratorImpl.h"


namespace Poco::XSD::Types {


class Element;


class XSDTypes_API ElementIterator: public OrderIteratorImpl
	/// An iterator for iterating over an Element's content.
{
public:
	explicit ElementIterator(Element& elem);
		/// Creates the ElementIterator.

	~ElementIterator() override;
		/// Destroys the ElementIterator.

	// OrderIteratorImpl
	OrderContent::Ptr next(const std::string& name) override;
	[[nodiscard]] const std::set<std::string>& validNexts() const override;
	[[nodiscard]] bool validNext(const std::string& name) const override;
	[[nodiscard]] bool end() const override;
	void close() override;
	[[nodiscard]] bool canClose() const override;
	void reset() override;

private:
	Element* _pElem;
	UInt32 _cnt = 0;
	mutable std::set<std::string> _next;
};


} // namespace Poco::XSD::Types


#endif // XSDTypes_ElementIterator_INCLUDED
