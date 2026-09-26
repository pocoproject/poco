//
// Selector.h
//
// Library: XSD/Types
// Package: XSDFacets
// Module:  Selector
//
// Definition of the Selector class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_Selector_INCLUDED
#define XSDTypes_Selector_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"


namespace Poco {
namespace XSD {
namespace Types {


class XSDTypes_API Selector
	/// This class represents a selector property in an XML Schema.
{
public:
	Selector();
		/// Creates the Selector.

	virtual ~Selector();
		/// Destroys the Selector.

protected:

private:
};


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_Selector_INCLUDED
