//
// Key.h
//
// Library: XSD/Types
// Package: XSDFacets
// Module:  Key
//
// Definition of the Key class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_Key_INCLUDED
#define XSDTypes_Key_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"


namespace Poco {
namespace XSD {
namespace Types {


class XSDTypes_API Key
	/// This class represents a key property in an XML Schema.
{
public:
	Key();
		/// Creates the Key.

	virtual ~Key();
		/// Destroys the Key.

protected:

private:
};


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_Key_INCLUDED
