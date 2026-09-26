//
// TotalDigits.h
//
// Library: XSD/Types
// Package: XSDFacets
// Module:  TotalDigits
//
// Definition of the TotalDigits class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef XSDTypes_TotalDigits_INCLUDED
#define XSDTypes_TotalDigits_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"


namespace Poco {
namespace XSD {
namespace Types {


class XSDTypes_API TotalDigits
	/// This class represents a totalDigits facet in an XML Schema.
{
public:
	TotalDigits();
		/// Creates the TotalDigits.

	virtual ~TotalDigits();
		/// Destroys the TotalDigits.

protected:

private:
};


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_TotalDigits_INCLUDED
