//
// Pattern.h
//
// Library: XSD/Types
// Package: XSDFacets
// Module:  Pattern
//
// Definition of the Pattern class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef XSDTypes_Pattern_INCLUDED
#define XSDTypes_Pattern_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"


namespace Poco {
namespace XSD {
namespace Types {


class XSDTypes_API Pattern
	/// This class represents a pattern facet in an XML Schema.
{
public:
	Pattern();
		/// Creates the Pattern.

	virtual ~Pattern();
		/// Destroys the Pattern.

protected:

private:
};


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_Pattern_INCLUDED
