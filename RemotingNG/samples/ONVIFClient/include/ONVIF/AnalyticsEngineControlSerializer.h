//
// AnalyticsEngineControlSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_AnalyticsEngineControl_INCLUDED
#define TypeSerializer_ONVIF_AnalyticsEngineControl_INCLUDED


#include "ONVIF/AnalyticsEngineControl.h"
#include "ONVIF/ConfigDeserializer.h"
#include "ONVIF/ConfigSerializer.h"
#include "ONVIF/MulticastConfigurationDeserializer.h"
#include "ONVIF/MulticastConfigurationSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::AnalyticsEngineControl>
{
public:
	static void prepareSerializeAttributes(Serializer& ser)
	{
		// ONVIF::ConfigurationEntity
		{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[] = {"token"s,"http://www.onvif.org/ver10/schema"s};
		ser.pushAttribute(REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[1], REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[0]);
		}
		
	}

	static void serialize(const std::string& name, const ONVIF::AnalyticsEngineControl& value, Serializer& ser)
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

	static void serializeAttributes(const ONVIF::AnalyticsEngineControl& value, Serializer& ser)
	{
		// ONVIF::ConfigurationEntity
		{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[] = {"token"s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[0], value.getToken(), ser);
		}
		
	}

	static void serializeImpl(const ONVIF::AnalyticsEngineControl& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		// ONVIF::ConfigurationEntity
		{
		static const std::string REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[] = {"Name"s,"UseCount"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[0], value.getName(), ser);
		TypeSerializer<int>::serialize(REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[1], value.getUseCount(), ser);
		}
		
		static const std::string REMOTING__NAMES[] = {"EngineToken"s,"EngineConfigToken"s,"InputToken"s,"ReceiverToken"s,"Multicast"s,"Subscription"s,"Mode"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getEngineToken(), ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[1], value.getEngineConfigToken(), ser);
		TypeSerializer<std::vector<std::string>>::serialize(REMOTING__NAMES[2], value.getInputToken(), ser);
		TypeSerializer<std::vector<std::string>>::serialize(REMOTING__NAMES[3], value.getReceiverToken(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::MulticastConfiguration>>::serialize(REMOTING__NAMES[4], value.getMulticast(), ser);
		TypeSerializer<ONVIF::Config>::serialize(REMOTING__NAMES[5], value.getSubscription(), ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[6], value.getMode(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_AnalyticsEngineControl_INCLUDED

