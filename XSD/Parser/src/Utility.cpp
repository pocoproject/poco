//
// Utility.cpp
//
// Library: XSD/Parser
// Package: XSDParser
// Module:  Utility
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "Poco/XSD/Parser/Utility.h"
#include "Poco/XSD/Parser/XSDContentHandler.h"
#include "Poco/XSD/Types/TypesManager.h"
#include "Poco/NumberParser.h"


namespace Poco {
namespace XSD {
namespace Parser {


void Utility::getBlock(const CompactAttributes::const_iterator& itEnd, const CompactAttributes::const_iterator& itAttr, bool& blockRestriction, bool& blockExtension, bool& blockSubstitution)
{
	if (itAttr == itEnd)
		return;
	else
	{
		if (std::string::npos != itAttr->second.find(Constants::XSD_HASHALL))
		{
			blockRestriction = blockExtension = blockSubstitution = true;
		}
		else
		{
			blockRestriction = (std::string::npos != itAttr->second.find(Constants::XSD_RESTRICTION));
			blockExtension = (std::string::npos != itAttr->second.find(Constants::XSD_EXTENSION));
			blockSubstitution = (std::string::npos != itAttr->second.find(Constants::XSD_SUBSTITUTION));
		}
	}
}


void Utility::getFinalDefault(const CompactAttributes::const_iterator& itEnd, const CompactAttributes::const_iterator& itAttr, bool& finalRestriction, bool& finalExtension, bool& finalList, bool& finalUnion)
{
	if (itAttr == itEnd)
		finalRestriction = finalExtension = finalList = finalUnion = false;
	else
	{
		if (std::string::npos != itAttr->second.find(Constants::XSD_HASHALL))
		{
			finalRestriction = finalExtension = finalList = finalUnion = true;
		}
		else
		{
			finalRestriction = (std::string::npos != itAttr->second.find(Constants::XSD_RESTRICTION));
			finalExtension = (std::string::npos != itAttr->second.find(Constants::XSD_EXTENSION));
			finalList = (std::string::npos != itAttr->second.find(Constants::XSD_LIST));
			finalUnion = (std::string::npos != itAttr->second.find(Constants::XSD_UNION));
		}
	}
}


void Utility::getFinal(const CompactAttributes::const_iterator& itEnd, const CompactAttributes::const_iterator& itAttr, bool& finalRestriction, bool& finalExtension)
{
	if (itAttr == itEnd)
		return;

	if (std::string::npos != itAttr->second.find(Constants::XSD_HASHALL))
	{
		finalRestriction = finalExtension = true;
	}
	else
	{
		finalRestriction = (std::string::npos != itAttr->second.find(Constants::XSD_RESTRICTION));
		finalExtension = (std::string::npos != itAttr->second.find(Constants::XSD_EXTENSION));
	}
}


void Utility::getSimpleTypeFinal(const CompactAttributes::const_iterator& itEnd, const CompactAttributes::const_iterator& itAttr, bool& finalRestriction, bool& finalList, bool& finalUnion)
{
	if (itAttr == itEnd)
		return;

	if (std::string::npos != itAttr->second.find(Constants::XSD_HASHALL))
	{
		finalRestriction = finalList = finalUnion = true;
	}
	else
	{
		finalRestriction = (std::string::npos != itAttr->second.find(Constants::XSD_RESTRICTION));
		finalList = (std::string::npos != itAttr->second.find(Constants::XSD_LIST));
		finalUnion = (std::string::npos != itAttr->second.find(Constants::XSD_UNION));
	}
}


Poco::UInt32 Utility::getMaxOccurs(const CompactAttributes::const_iterator& itEnd, const CompactAttributes::const_iterator& itAttr)
{
	if (itAttr == itEnd)
		return 1;
	
	if (itAttr->second.empty())
		return 1;

	if (itAttr->second == Constants::XSD_UNBOUNDED)
		return Constants::VAL_UNBOUNDED;

	return Poco::NumberParser::parseUnsigned(itAttr->second);
}


Poco::UInt32 Utility::getMinOccurs(const CompactAttributes::const_iterator& itEnd, const CompactAttributes::const_iterator& itAttr)
{
	if (itAttr == itEnd)
		return 1;
	
	if (itAttr->second.empty())
		return 1;

	return Poco::NumberParser::parseUnsigned(itAttr->second);
}


Poco::XSD::Types::Any::ProcessStyle Utility::getAnyProcessStyle(const CompactAttributes::const_iterator& itEnd, const CompactAttributes::const_iterator& itAttr)
{
	if (itAttr == itEnd)
		return Poco::XSD::Types::Any::PS_STRICT;

	if (itAttr->second == Constants::XSD_LAX)
		return Poco::XSD::Types::Any::PS_LAX;

	if (itAttr->second == Constants::XSD_SKIP)
		return Poco::XSD::Types::Any::PS_SKIP;

	return Poco::XSD::Types::Any::PS_STRICT;
}


Poco::XSD::Types::AnyAttribute::ProcessStyle Utility::getAnyAttrProcessStyle(const CompactAttributes::const_iterator& itEnd, const CompactAttributes::const_iterator& itAttr)
{
	if (itAttr == itEnd)
		return Poco::XSD::Types::AnyAttribute::PS_STRICT;

	if (itAttr->second == Constants::XSD_LAX)
		return Poco::XSD::Types::AnyAttribute::PS_LAX;

	if (itAttr->second == Constants::XSD_SKIP)
		return Poco::XSD::Types::AnyAttribute::PS_SKIP;

	return Poco::XSD::Types::AnyAttribute::PS_STRICT;
}


Poco::XSD::Types::AbstractAttribute::Usage Utility::getAttrUsage(const CompactAttributes::const_iterator& itEnd, const CompactAttributes::const_iterator& itAttr, Poco::XSD::Types::AbstractAttribute::Usage def)
{
	if (itAttr == itEnd)
		return def;

	if (itAttr->second == Constants::XSD_PROHIBITED)
		return Poco::XSD::Types::AbstractAttribute::USE_PROHIBITED;

	if (itAttr->second == Constants::XSD_REQUIRED)
		return Poco::XSD::Types::AbstractAttribute::USE_REQUIRED;

	if (itAttr->second == Constants::XSD_OPTIONAL)
		return Poco::XSD::Types::AbstractAttribute::USE_OPTIONAL;

	return def;
}


} } } // namespace Poco::XSD::Parser
