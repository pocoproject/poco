//
// RuleEngineConfigurationSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_RuleEngineConfiguration_INCLUDED
#define TypeSerializer_ONVIF_RuleEngineConfiguration_INCLUDED


#include "ONVIF/ConfigDeserializer.h"
#include "ONVIF/ConfigSerializer.h"
#include "ONVIF/RuleEngineConfiguration.h"
#include "ONVIF/RuleEngineConfigurationExtensionDeserializer.h"
#include "ONVIF/RuleEngineConfigurationExtensionSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::RuleEngineConfiguration>
{
public:
	static void serialize(const std::string& name, const ONVIF::RuleEngineConfiguration& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeImpl(const ONVIF::RuleEngineConfiguration& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Rule"s,"Extension"s,""s};
		TypeSerializer<std::vector<ONVIF::Config>>::serialize(REMOTING__NAMES[0], value.getRule(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::RuleEngineConfigurationExtension>>::serialize(REMOTING__NAMES[1], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_RuleEngineConfiguration_INCLUDED

