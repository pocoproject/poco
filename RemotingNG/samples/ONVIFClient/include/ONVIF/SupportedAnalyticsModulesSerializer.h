//
// SupportedAnalyticsModulesSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_SupportedAnalyticsModules_INCLUDED
#define TypeSerializer_ONVIF_SupportedAnalyticsModules_INCLUDED


#include "ONVIF/ConfigDescriptionDeserializer.h"
#include "ONVIF/ConfigDescriptionSerializer.h"
#include "ONVIF/SupportedAnalyticsModules.h"
#include "ONVIF/SupportedAnalyticsModulesExtensionDeserializer.h"
#include "ONVIF/SupportedAnalyticsModulesExtensionSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::SupportedAnalyticsModules>
{
public:
	static void prepareSerializeAttributes(Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Limit"s,"http://www.onvif.org/ver10/schema"s};
		ser.pushAttribute(REMOTING__NAMES[1], REMOTING__NAMES[0]);
	}

	static void serialize(const std::string& name, const ONVIF::SupportedAnalyticsModules& value, Serializer& ser)
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

	static void serializeAttributes(const ONVIF::SupportedAnalyticsModules& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Limit"s};
		TypeSerializer<Poco::Optional<int>>::serialize(REMOTING__NAMES[0], value.getLimit(), ser);
	}

	static void serializeImpl(const ONVIF::SupportedAnalyticsModules& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"AnalyticsModuleContentSchemaLocation"s,"AnalyticsModuleDescription"s,"Extension"s,""s};
		TypeSerializer<std::vector<Poco::URI>>::serialize(REMOTING__NAMES[0], value.getAnalyticsModuleContentSchemaLocation(), ser);
		TypeSerializer<std::vector<ONVIF::ConfigDescription>>::serialize(REMOTING__NAMES[1], value.getAnalyticsModuleDescription(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::SupportedAnalyticsModulesExtension>>::serialize(REMOTING__NAMES[2], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_SupportedAnalyticsModules_INCLUDED

