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
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef XSDTypes_AnyIterator_INCLUDED
#define XSDTypes_AnyIterator_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/OrderIteratorImpl.h"


namespace Poco {
namespace XSD {
namespace Types {


class Any;


class XSDTypes_API AnyIterator: public OrderIteratorImpl
	/// An iterator for iterating over Any content groups.
{
public:
	AnyIterator(Any& any);
		/// Creates the AnyIterator.

	virtual ~AnyIterator();
		/// Destroys the AnyIterator.

	// OrderIteratorImpl
	OrderContent::Ptr next(const std::string& name);
	const std::set<std::string>& validNexts() const;
	bool end() const;
	bool validNext(const std::string& name) const;
	void close();
	bool canClose() const;
	void reset();

private:
	UInt32 _min;
	UInt32 _max;
	UInt32 _cnt;
	Any*   _pAny;
	mutable std::set<std::string> _next;
};


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_AnyIterator_INCLUDED
