//
// Utility.h
//
// Library: XSD/Parser
// Package: XSDParser
// Module:  Utility
//
// Definition of the Utility class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDParser_Utility_INCLUDED
#define XSDParser_Utility_INCLUDED


#include "Poco/XSD/Parser/XSDParser.h"
#include "Poco/XSD/Parser/Constants.h"
#include "Poco/XSD/Types/Any.h"
#include "Poco/XSD/Types/AnyAttribute.h"
#include "Poco/XSD/Types/AbstractAttribute.h"
#include "Poco/XSD/Types/XSDException.h"
#include "Poco/URI.h"
#include <map>


namespace Poco {
namespace XSD {
namespace Parser {


class Utility
	/// Utility class, used to extract attribute values.
{
public:
	static bool getQualified(const CompactAttributes::const_iterator& itEnd, const CompactAttributes::const_iterator& itAttr, bool defaultValue);

	static void getBlock(const CompactAttributes::const_iterator& itEnd, const CompactAttributes::const_iterator& itAttr, bool& blockRestriction, bool& blockExtension, bool& blockSubstitution);
		/// if itAttr == itEnd, blockRestriction and blockExtension and blockSubstitution will not be changed!

	static void getFinalDefault(const CompactAttributes::const_iterator& itEnd, const CompactAttributes::const_iterator& itAttr, bool& finalRestriction, bool& finalExtension, bool& finalList, bool& finalUnion);

	static void getFinal(const CompactAttributes::const_iterator& itEnd, const CompactAttributes::const_iterator& itAttr, bool& finalRestriction, bool& finalExtension);
		/// if itAttr == itEnd, finalRestriction and finalExtension will not be changed!

	static void getSimpleTypeFinal(const CompactAttributes::const_iterator& itEnd, const CompactAttributes::const_iterator& itAttr, bool& finalRestriction, bool& finalList, bool& finalUnion);
		/// if itAttr == itEnd, finalRestriction, finalUnion and finalList will not be changed!

	static bool getBool(const CompactAttributes::const_iterator& itEnd, const CompactAttributes::const_iterator& itAttr, bool defaultValue);

	static const std::string& getString(const CompactAttributes::const_iterator& itEnd, const CompactAttributes::const_iterator& itAttr, const std::string& defaultValue);
		/// For optional string attributes

	static const std::string& getString(const CompactAttributes::const_iterator& itEnd, const CompactAttributes::const_iterator& itAttr);
		/// For mandatory attributes

	static Poco::UInt32 getMaxOccurs(const CompactAttributes::const_iterator& itEnd, const CompactAttributes::const_iterator& itAttr);

	static Poco::UInt32 getMinOccurs(const CompactAttributes::const_iterator& itEnd, const CompactAttributes::const_iterator& itAttr);

	static Poco::XSD::Types::Any::ProcessStyle getAnyProcessStyle(const CompactAttributes::const_iterator& itEnd, const CompactAttributes::const_iterator& itAttr);

	static Poco::XSD::Types::AnyAttribute::ProcessStyle getAnyAttrProcessStyle(const CompactAttributes::const_iterator& itEnd, const CompactAttributes::const_iterator& itAttr);

	static Poco::XSD::Types::AbstractAttribute::Usage getAttrUsage(const CompactAttributes::const_iterator& itEnd, const CompactAttributes::const_iterator& itAttr, Poco::XSD::Types::AbstractAttribute::Usage def);

private:
	Utility();
	Utility(const Utility&);
	Utility& operator=(const Utility&);
	~Utility();
};


//
// inlines
//
inline bool Utility::getQualified(const CompactAttributes::const_iterator& itEnd, const CompactAttributes::const_iterator& itAttr, bool defaultValue)
{
	if (itAttr == itEnd)
		return defaultValue;

	return (itAttr->second == Constants::XSD_QUALIFIED);
}


inline bool Utility::getBool(const CompactAttributes::const_iterator& itEnd, const CompactAttributes::const_iterator& itAttr, bool defaultValue)
{
	if (itAttr == itEnd)
		return defaultValue;

	return (itAttr->second == Constants::XSD_TRUE  || itAttr->second == "1");
}


inline const std::string& Utility::getString(const CompactAttributes::const_iterator& itEnd, const CompactAttributes::const_iterator& itAttr, const std::string& defaultValue)
{
	if (itAttr == itEnd)
		return defaultValue;

	return itAttr->second;
}


inline const std::string& Utility::getString(const CompactAttributes::const_iterator& itEnd, const CompactAttributes::const_iterator& itAttr)
{
	if (itAttr == itEnd)
		throw Poco::XSD::Types::SchemaException("Mandatory attribute not found");

	return itAttr->second;
}


} } } // namespace Poco::XSD::Parser


#endif // XSDParser_Utility_INCLUDED
