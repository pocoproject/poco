//
// Annotation.h
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  Annotation
//
// Definition of the Annotation class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef XSDTypes_Annotation_INCLUDED
#define XSDTypes_Annotation_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/AnnotationContent.h"
#include "Poco/XSD/Types/Visitable.h"
#include <vector>


namespace Poco {
namespace XSD {
namespace Types {


class XSDTypes_API Annotation: public Visitable
	/// This class represents an Annotation in an XML Schema.
{
public:
	using Ptr = Poco::AutoPtr<Annotation>;

	Annotation(const std::string& id);
		/// Creates the Annotation.

	~Annotation();
		/// Destroys the Annotation.

	Annotation(const Annotation& ann);
		/// Copy constructor

	Annotation& operator=(const Annotation& ann);
		/// Assignement operator

	void swap(Annotation& ann);
		/// Swap implementation

	const std::vector<AnnotationContent::Ptr>& annotationContent() const;
		/// Returns the annotation content read-only

	std::vector<AnnotationContent::Ptr>& annotationContent();
		/// Returns the annotation content writable

	void accept(Visitor& v) const;

	const std::string& id() const;
		/// Returns the id

private:
	std::vector<AnnotationContent::Ptr> _content;
	std::string _id;
};


//
// inlines
//
inline const std::vector<AnnotationContent::Ptr>& Annotation::annotationContent() const
{
	return _content;
}


inline std::vector<AnnotationContent::Ptr>& Annotation::annotationContent()
{
	return _content;
}


inline const std::string& Annotation::id() const
{
	return _id;
}


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_Annotation_INCLUDED
