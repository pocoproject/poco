//
// AnnotationContent.h
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  AnnotationContent
//
// Definition of the AnnotationContent class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_AnnotationContent_INCLUDED
#define XSDTypes_AnnotationContent_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/Visitable.h"
#include "Poco/AutoPtr.h"


namespace Poco {
namespace XSD {
namespace Types {


class XSDTypes_API AnnotationContent: public Visitable
	/// AnnotationContent can be of type Any. Most common is a simple string, but a whole XML subtree is valid.
{
public:
	using Ptr = Poco::AutoPtr<AnnotationContent>;

	AnnotationContent(const std::string& sourceUri);
		/// Creates the AnnotationContent.

	virtual ~AnnotationContent();
		/// Destroys the AnnotationContent.

	const std::string& source() const;
		/// Returns the source URL for the AnnotationContent

	const std::string& getData() const;
		/// Returns the content in string form

	void setData(const std::string& data);
		/// Sets the content as string

private:
	std::string _source;
	std::string _data;
};


//
// inlines
//
inline const std::string& AnnotationContent::source() const
{
	return _source;
}


inline const std::string& AnnotationContent::getData() const
{
	return _data;
}


inline void AnnotationContent::setData(const std::string& data)
{
	_data = data;
}


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_AnnotationContent_INCLUDED
