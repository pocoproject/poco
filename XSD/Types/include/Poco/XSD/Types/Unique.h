//
// Unique.h
//
// Library: XSD/Types
// Package: XSDFacets
// Module:  Unique
//
// Definition of the Unique class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef XSDTypes_Unique_INCLUDED
#define XSDTypes_Unique_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"


namespace Poco {
namespace XSD {
namespace Types {


class XSDTypes_API Unique
	/// This class represents a unique property in an XML Schema.
{
public:
	Unique();
		/// Creates the Unique.

	virtual ~Unique();
		/// Destroys the Unique.

protected:

private:
};


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_Unique_INCLUDED
