//
// PTZPresetSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_PTZPreset_INCLUDED
#define TypeSerializer_ONVIF_PTZPreset_INCLUDED


#include "ONVIF/PTZPreset.h"
#include "ONVIF/PTZVectorDeserializer.h"
#include "ONVIF/PTZVectorSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::PTZPreset>
{
public:
	static void prepareSerializeAttributes(Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"token"s,"http://www.onvif.org/ver10/schema"s};
		ser.pushAttribute(REMOTING__NAMES[1], REMOTING__NAMES[0]);
	}

	static void serialize(const std::string& name, const ONVIF::PTZPreset& value, Serializer& ser)
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

	static void serializeAttributes(const ONVIF::PTZPreset& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"token"s};
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[0], value.getToken(), ser);
	}

	static void serializeImpl(const ONVIF::PTZPreset& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Name"s,"PTZPosition"s,""s};
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[0], value.getName(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::PTZVector>>::serialize(REMOTING__NAMES[1], value.getPTZPosition(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_PTZPreset_INCLUDED

