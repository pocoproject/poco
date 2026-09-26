 //
// AllIterator.h
//
// Library: XSD/Types
// Package: Iterator
// Module:  AllIterator
//
// Definition of the AllIterator class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_AllIterator_INCLUDED
#define XSDTypes_AllIterator_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/OrderIteratorImpl.h"
#include <map>


namespace Poco {
namespace XSD {
namespace Types {


class All;
class ElementIterator;


class XSDTypes_API AllIterator: public OrderIteratorImpl
	/// An iterator for iterating over All content groups.
{
public:
	AllIterator(const All& all);
		/// Creates the AllIterator.

	virtual ~AllIterator();
		/// Destroys the AllIterator.

	// OrderIteratorImpl
	OrderContent::Ptr next(const std::string& name);
	const std::set<std::string>& validNexts() const;
	bool end() const;
	bool validNext(const std::string& name) const;
	void close();
	bool canClose() const;
	void reset();

private:
	using Content = std::map<std::string, ElementIterator*>;
	using NextSet = std::set<std::string>;

	UInt32 _min;
	UInt32 _cnt;
	Content _mapIt;
	Content::iterator _itLastUsed;
	mutable NextSet _nexts;
	mutable bool _dirtyFlag;
};


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_AllIterator_INCLUDED
