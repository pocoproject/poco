//
// PTZNodeSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_PTZNode_INCLUDED
#define TypeSerializer_ONVIF_PTZNode_INCLUDED


#include "ONVIF/PTZNode.h"
#include "ONVIF/PTZNodeExtensionDeserializer.h"
#include "ONVIF/PTZNodeExtensionSerializer.h"
#include "ONVIF/PTZSpacesDeserializer.h"
#include "ONVIF/PTZSpacesSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::PTZNode>
{
public:
	static void prepareSerializeAttributes(Serializer& ser)
	{
		// ONVIF::DeviceEntity
		{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES__ONVIF__DEVICEENTITY[] = {"token"s,"http://www.onvif.org/ver10/schema"s};
		ser.pushAttribute(REMOTING__NAMES__ONVIF__DEVICEENTITY[1], REMOTING__NAMES__ONVIF__DEVICEENTITY[0]);
		}
		
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"FixedHomePosition"s,"GeoMove"s,"http://www.onvif.org/ver10/schema"s};
		ser.pushAttribute(REMOTING__NAMES[2], REMOTING__NAMES[0]);
		ser.pushAttribute(REMOTING__NAMES[2], REMOTING__NAMES[1]);
	}

	static void serialize(const std::string& name, const ONVIF::PTZNode& value, Serializer& ser)
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

	static void serializeAttributes(const ONVIF::PTZNode& value, Serializer& ser)
	{
		// ONVIF::DeviceEntity
		{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES__ONVIF__DEVICEENTITY[] = {"token"s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES__ONVIF__DEVICEENTITY[0], value.getToken(), ser);
		}
		
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"FixedHomePosition"s,"GeoMove"s};
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[0], value.getFixedHomePosition(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[1], value.getGeoMove(), ser);
	}

	static void serializeImpl(const ONVIF::PTZNode& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		// ONVIF::DeviceEntity
		{
		}
		
		static const std::string REMOTING__NAMES[] = {"Name"s,"SupportedPTZSpaces"s,"MaximumNumberOfPresets"s,"HomeSupported"s,"AuxiliaryCommands"s,"Extension"s,""s};
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[0], value.getName(), ser);
		TypeSerializer<ONVIF::PTZSpaces>::serialize(REMOTING__NAMES[1], value.getSupportedPTZSpaces(), ser);
		TypeSerializer<int>::serialize(REMOTING__NAMES[2], value.getMaximumNumberOfPresets(), ser);
		TypeSerializer<bool>::serialize(REMOTING__NAMES[3], value.getHomeSupported(), ser);
		TypeSerializer<std::vector<std::string>>::serialize(REMOTING__NAMES[4], value.getAuxiliaryCommands(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::PTZNodeExtension>>::serialize(REMOTING__NAMES[5], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_PTZNode_INCLUDED

