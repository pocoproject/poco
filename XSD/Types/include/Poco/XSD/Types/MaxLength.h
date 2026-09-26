//
// MaxLength.h
//
// Library: XSD/Types
// Package: XSDFacets
// Module:  MaxLength
//
// Definition of the MaxLength class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef XSDTypes_MaxLength_INCLUDED
#define XSDTypes_MaxLength_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"


namespace Poco {
namespace XSD {
namespace Types {


class XSDTypes_API MaxLength
	/// This class represents a maxLength facet in an XML Schema.
{
public:
	MaxLength();
		/// Creates the MaxLength.

	virtual ~MaxLength();
		/// Destroys the MaxLength.

protected:

private:
};


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_MaxLength_INCLUDED
