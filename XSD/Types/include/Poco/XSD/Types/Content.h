//
// Content.h
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  Content
//
// Definition of the Content class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef XSDTypes_Content_INCLUDED
#define XSDTypes_Content_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/AnnotatedObject.h"
#include "Poco/XSD/Types/AttributeContent.h"


namespace Poco {
namespace XSD {
namespace Types {


class Type;
class OrderIterator;


class XSDTypes_API Content: public AnnotatedObject
	/// Super class for all different types of Content that can be stored in a ComplexType.
{
public:
	using Ptr = AutoPtr<Content>;

	Content();
		/// Creates the Content.

	Content(const std::string& id);
		/// Creates the Content.

	virtual ~Content();
		/// Destroys the Content.

	virtual const std::vector<const Type*>& types() const = 0;
		/// Returns the type referenced by the content. Can be null.

	virtual void fixup() = 0;
		/// Resolves references to actual types.
	
	virtual OrderIterator iterator() const = 0;
		/// Creates an iterator for the given order type.
};


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_Content_INCLUDED
