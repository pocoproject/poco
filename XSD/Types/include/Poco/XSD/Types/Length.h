//
// Length.h
//
// Library: XSD/Types
// Package: XSDFacets
// Module:  Length
//
// Definition of the Length class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef XSDTypes_Length_INCLUDED
#define XSDTypes_Length_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"


namespace Poco {
namespace XSD {
namespace Types {


class XSDTypes_API Length
	/// This class represents a length facet in an XML Schema.
{
public:
	Length();
		/// Creates the Length.

	virtual ~Length();
		/// Destroys the Length.

protected:

private:
};


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_Length_INCLUDED
