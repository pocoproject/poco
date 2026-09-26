//
// AppInfo.h
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  AppInfo
//
// Definition of the AppInfo class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_AppInfo_INCLUDED
#define XSDTypes_AppInfo_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/AnnotationContent.h"


namespace Poco {
namespace XSD {
namespace Types {


class XSDTypes_API AppInfo: public AnnotationContent
	/// Class AppInfo. Can be used to transport additional constraints.
	/// An AppInfo can store anything, from simple text to elements.
	///    <xsd:annotation>
	///        <xsd:appinfo>
	///            <sch:pattern name="Check A greater than B">
	///                <sch:rule context="d:Demo">
	///                    <sch:assert test="d:A > d:B" 
	///                                diagnostics="lessThan">
	///                            A should be greater than B.
	///                    </sch:assert>
	///                </sch:rule>
	///            </sch:pattern>
	///            <sch:diagnostics>
	///                <sch:diagnostic id="lessThan">
	///                    Error! A is less than B 
	///                    A = <sch:value-of select="d:A"/>
	///                    B = <sch:value-of select="d:B"/>
	///                </sch:diagnostic>
	///            </sch:diagnostics>
	///        </xsd:appinfo>
	///    </xsd:annotation>
{
public:
	AppInfo(const std::string& sourceUri);
		/// Creates the AppInfo.

	virtual ~AppInfo();
		/// Destroys the AppInfo.

	void accept(Visitor& v) const;

private:
};


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_AppInfo_INCLUDED
