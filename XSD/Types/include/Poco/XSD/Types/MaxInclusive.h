//
// MaxInclusive.h
//
// Library: XSD/Types
// Package: XSDFacets
// Module:  MaxInclusive
//
// Definition of the MaxInclusive class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_MaxInclusive_INCLUDED
#define XSDTypes_MaxInclusive_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"


namespace Poco {
namespace XSD {
namespace Types {


class XSDTypes_API MaxInclusive
	/// This class represents a maxInclusive facet in an XML Schema.
{
public:
	MaxInclusive();
		/// Creates the MaxInclusive.

	virtual ~MaxInclusive();
		/// Destroys the MaxInclusive.

protected:

private:
};


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_MaxInclusive_INCLUDED
