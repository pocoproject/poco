//
// VideoSourceConfigurationSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_VideoSourceConfiguration_INCLUDED
#define TypeSerializer_ONVIF_VideoSourceConfiguration_INCLUDED


#include "ONVIF/IntRectangleDeserializer.h"
#include "ONVIF/IntRectangleSerializer.h"
#include "ONVIF/VideoSourceConfiguration.h"
#include "ONVIF/VideoSourceConfigurationExtensionDeserializer.h"
#include "ONVIF/VideoSourceConfigurationExtensionSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::VideoSourceConfiguration>
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
		
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"ViewMode"s,"http://www.onvif.org/ver10/schema"s};
		ser.pushAttribute(REMOTING__NAMES[1], REMOTING__NAMES[0]);
	}

	static void serialize(const std::string& name, const ONVIF::VideoSourceConfiguration& value, Serializer& ser)
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

	static void serializeAttributes(const ONVIF::VideoSourceConfiguration& value, Serializer& ser)
	{
		// ONVIF::ConfigurationEntity
		{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[] = {"token"s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[0], value.getToken(), ser);
		}
		
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"ViewMode"s};
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[0], value.getViewMode(), ser);
	}

	static void serializeImpl(const ONVIF::VideoSourceConfiguration& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		// ONVIF::ConfigurationEntity
		{
		static const std::string REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[] = {"Name"s,"UseCount"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[0], value.getName(), ser);
		TypeSerializer<int>::serialize(REMOTING__NAMES__ONVIF__CONFIGURATIONENTITY[1], value.getUseCount(), ser);
		}
		
		static const std::string REMOTING__NAMES[] = {"SourceToken"s,"Bounds"s,"Extension"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getSourceToken(), ser);
		TypeSerializer<ONVIF::IntRectangle>::serialize(REMOTING__NAMES[1], value.getBounds(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::VideoSourceConfigurationExtension>>::serialize(REMOTING__NAMES[2], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_VideoSourceConfiguration_INCLUDED

