//
// VideoRateControlSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_VideoRateControl_INCLUDED
#define TypeSerializer_ONVIF_VideoRateControl_INCLUDED


#include "ONVIF/VideoRateControl.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::VideoRateControl>
{
public:
	static void serialize(const std::string& name, const ONVIF::VideoRateControl& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::VideoRateControl& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"FrameRateLimit"s,"EncodingInterval"s,"BitrateLimit"s,""s};
		TypeSerializer<int>::serialize(REMOTING__NAMES[0], value.getFrameRateLimit(), ser);
		TypeSerializer<int>::serialize(REMOTING__NAMES[1], value.getEncodingInterval(), ser);
		TypeSerializer<int>::serialize(REMOTING__NAMES[2], value.getBitrateLimit(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_VideoRateControl_INCLUDED

