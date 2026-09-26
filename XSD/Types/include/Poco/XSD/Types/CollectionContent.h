//
// CollectionContent.h
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  CollectionContent
//
// Definition of the CollectionContent class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef XSDTypes_CollectionContent_INCLUDED
#define XSDTypes_CollectionContent_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/OrderContent.h"


namespace Poco {
namespace XSD {
namespace Types {


class XSDTypes_API CollectionContent: public OrderContent
	/// CollectionContent is the super-class for all collections.
{
public:
	using Ptr = Poco::AutoPtr<CollectionContent>;

	CollectionContent();
		/// Creates the CollectionContent.

	CollectionContent(const std::string& id, Poco::UInt32 minOcc, Poco::UInt32 maxOcc);
		/// Creates the CollectionContent.

	virtual ~CollectionContent();
		/// Destroys the CollectionContent.

protected:

private:
};


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_CollectionContent_INCLUDED
