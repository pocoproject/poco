//
// Whitespace.h
//
// Library: XSD/Types
// Package: XSDFacets
// Module:  Whitespace
//
// Definition of the Whitespace class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_Whitespace_INCLUDED
#define XSDTypes_Whitespace_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"


namespace Poco {
namespace XSD {
namespace Types {


class XSDTypes_API Whitespace
	/// This class represents a whiteSpace facet in an XML Schema.
{
public:
	Whitespace();
		/// Creates the Whitespace.

	virtual ~Whitespace();
		/// Destroys the Whitespace.

protected:

private:
};


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_Whitespace_INCLUDED
