//
// Documentation.h
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  Documentation
//
// Definition of the Documentation class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_Documentation_INCLUDED
#define XSDTypes_Documentation_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/AnnotationContent.h"


namespace Poco::XSD::Types {


class XSDTypes_API Documentation: public AnnotationContent
	/// This class represents a documentation in an XML Schema.
{
public:
	Documentation(const std::string& sourceUri, const std::string& lang);
		/// Creates the Documentation.

	~Documentation() override;
		/// Destroys the Documentation.

	[[nodiscard]] const std::string& language() const;
		/// The language of the documentation entry. Can be empty.

	void accept(Visitor& v) const override;

private:
	std::string _language;
};


//
// inlines
//
inline const std::string& Documentation::language() const
{
	return _language;
}


} // namespace Poco::XSD::Types


#endif // XSDTypes_Documentation_INCLUDED
