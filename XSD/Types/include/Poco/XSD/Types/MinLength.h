//
// MinLength.h
//
// Library: XSD/Types
// Package: XSDFacets
// Module:  MinLength
//
// Definition of the MinLength class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_MinLength_INCLUDED
#define XSDTypes_MinLength_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"


namespace Poco {
namespace XSD {
namespace Types {


class XSDTypes_API MinLength
	/// This class represents a minLength facet in an XML Schema.
{
public:
	MinLength();
		/// Creates the MinLength.

	virtual ~MinLength();
		/// Destroys the MinLength.

protected:

private:
};


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_MinLength_INCLUDED
