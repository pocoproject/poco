//
// FocusConfiguration20Serializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_FocusConfiguration20_INCLUDED
#define TypeSerializer_ONVIF_FocusConfiguration20_INCLUDED


#include "ONVIF/FocusConfiguration20.h"
#include "ONVIF/FocusConfiguration20ExtensionDeserializer.h"
#include "ONVIF/FocusConfiguration20ExtensionSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::FocusConfiguration20>
{
public:
	static void prepareSerializeAttributes(Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"AFMode"s,"http://www.onvif.org/ver10/schema"s};
		ser.pushAttribute(REMOTING__NAMES[1], REMOTING__NAMES[0]);
	}

	static void serialize(const std::string& name, const ONVIF::FocusConfiguration20& value, Serializer& ser)
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

	static void serializeAttributes(const ONVIF::FocusConfiguration20& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"AFMode"s};
		TypeSerializer<std::vector<std::vector<std::string>>>::serialize(REMOTING__NAMES[0], value.getAFMode(), ser);
	}

	static void serializeImpl(const ONVIF::FocusConfiguration20& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"AutoFocusMode"s,"DefaultSpeed"s,"NearLimit"s,"FarLimit"s,"Extension"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getAutoFocusMode(), ser);
		TypeSerializer<Poco::Optional<float>>::serialize(REMOTING__NAMES[1], value.getDefaultSpeed(), ser);
		TypeSerializer<Poco::Optional<float>>::serialize(REMOTING__NAMES[2], value.getNearLimit(), ser);
		TypeSerializer<Poco::Optional<float>>::serialize(REMOTING__NAMES[3], value.getFarLimit(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::FocusConfiguration20Extension>>::serialize(REMOTING__NAMES[4], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_FocusConfiguration20_INCLUDED

