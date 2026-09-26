//
// Field.h
//
// Library: XSD/Types
// Package: XSDFacets
// Module:  Field
//
// Definition of the Field class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_Field_INCLUDED
#define XSDTypes_Field_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"


namespace Poco {
namespace XSD {
namespace Types {


class XSDTypes_API Field
	/// This class represents a field property in an XML Schema.
{
public:
	Field();
		/// Creates the Field.

	virtual ~Field();
		/// Destroys the Field.

protected:

private:
};


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_Field_INCLUDED
