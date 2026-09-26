//
// MinInclusive.h
//
// Library: XSD/Types
// Package: XSDFacets
// Module:  MinInclusive
//
// Definition of the MinInclusive class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_MinInclusive_INCLUDED
#define XSDTypes_MinInclusive_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"


namespace Poco {
namespace XSD {
namespace Types {


class XSDTypes_API MinInclusive
	/// This class represents a minInclusive facet in an XML Schema.
{
public:
	MinInclusive();
		/// Creates the MinInclusive.

	virtual ~MinInclusive();
		/// Destroys the MinInclusive.

protected:

private:
};


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_MinInclusive_INCLUDED
