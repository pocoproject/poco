//
// FractionDigits.h
//
// Library: XSD/Types
// Package: XSDFacets
// Module:  FractionDigits
//
// Definition of the FractionDigits class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef XSDTypes_FractionDigits_INCLUDED
#define XSDTypes_FractionDigits_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"


namespace Poco {
namespace XSD {
namespace Types {


class XSDTypes_API FractionDigits
	/// This class represents an fractionDigits facet in an XML Schema.
{
public:
	FractionDigits();
		/// Creates the FractionDigits.

	virtual ~FractionDigits();
		/// Destroys the FractionDigits.

protected:

private:
};


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_FractionDigits_INCLUDED
