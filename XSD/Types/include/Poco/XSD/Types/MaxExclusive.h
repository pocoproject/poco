//
// MaxExclusive.h
//
// Library: XSD/Types
// Package: XSDFacets
// Module:  MaxExclusive
//
// Definition of the MaxExclusive class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_MaxExclusive_INCLUDED
#define XSDTypes_MaxExclusive_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"


namespace Poco {
namespace XSD {
namespace Types {


class XSDTypes_API MaxExclusive
	/// This class represents a maxExclusive facet in an XML Schema.
{
public:
	MaxExclusive();
		/// Creates the MaxExclusive.

	virtual ~MaxExclusive();
		/// Destroys the MaxExclusive.

protected:

private:
};


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_MaxExclusive_INCLUDED
