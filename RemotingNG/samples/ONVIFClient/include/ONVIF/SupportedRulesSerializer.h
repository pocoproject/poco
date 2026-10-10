//
// SupportedRulesSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_SupportedRules_INCLUDED
#define TypeSerializer_ONVIF_SupportedRules_INCLUDED


#include "ONVIF/ConfigDescriptionDeserializer.h"
#include "ONVIF/ConfigDescriptionSerializer.h"
#include "ONVIF/SupportedRules.h"
#include "ONVIF/SupportedRulesExtensionDeserializer.h"
#include "ONVIF/SupportedRulesExtensionSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::SupportedRules>
{
public:
	static void prepareSerializeAttributes(Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Limit"s,"http://www.onvif.org/ver10/schema"s};
		ser.pushAttribute(REMOTING__NAMES[1], REMOTING__NAMES[0]);
	}

	static void serialize(const std::string& name, const ONVIF::SupportedRules& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		prepareSerializeAttributes(ser);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeAttributes(value, ser);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeAttributes(const ONVIF::SupportedRules& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Limit"s};
		TypeSerializer<Poco::Optional<int>>::serialize(REMOTING__NAMES[0], value.getLimit(), ser);
	}

	static void serializeImpl(const ONVIF::SupportedRules& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"RuleContentSchemaLocation"s,"RuleDescription"s,"Extension"s,""s};
		TypeSerializer<std::vector<Poco::URI>>::serialize(REMOTING__NAMES[0], value.getRuleContentSchemaLocation(), ser);
		TypeSerializer<std::vector<ONVIF::ConfigDescription>>::serialize(REMOTING__NAMES[1], value.getRuleDescription(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::SupportedRulesExtension>>::serialize(REMOTING__NAMES[2], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_SupportedRules_INCLUDED

