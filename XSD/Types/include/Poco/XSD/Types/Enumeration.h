//
// Enumeration.h
//
// Library: XSD/Types
// Package: XSDFacets
// Module:  Enumeration
//
// Definition of the Enumeration class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_Enumeration_INCLUDED
#define XSDTypes_Enumeration_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"


namespace Poco {
namespace XSD {
namespace Types {


class XSDTypes_API Enumeration
	/// This class represents an enumeration facet in an XML Schema.
{
public:
	Enumeration();
		/// Creates the Enumeration.

	virtual ~Enumeration();
		/// Destroys the Enumeration.
};


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_Enumeration_INCLUDED
