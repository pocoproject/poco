//
// Visitable.h
//
// Library: XSD/Types
// Package: Visitor
// Module:  Visitable
//
// Definition of the Visitable class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_Visitable_INCLUDED
#define XSDTypes_Visitable_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/RefCountedObject.h"


namespace Poco::XSD::Types {


class Visitor;


class XSDTypes_API Visitable: public Poco::RefCountedObject
	/// Interface class Visitable, used to implement visitor pattern.
{
public:
	Visitable();
		/// Creates the Visitable.

	explicit Visitable(const std::string& id);
		/// Creates the Visitable with the given ID.

	~Visitable() override;
		/// Destroys the Visitable.

	[[nodiscard]] const std::string& id() const;
		/// Returns the id of the object

	virtual void accept(Visitor& v) const = 0;
		/// Implements the visitor pattern. Each subclass must implement it as
		/// v.visit(*this);

private:
	std::string _id;
};


//
// inlines
//
inline const std::string& Visitable::id() const
{
	return _id;
}


} // namespace Poco::XSD::Types


#endif // XSDTypes_Visitable_INCLUDED
