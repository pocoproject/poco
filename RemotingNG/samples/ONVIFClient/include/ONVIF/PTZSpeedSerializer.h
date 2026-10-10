//
// PTZSpeedSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_PTZSpeed_INCLUDED
#define TypeSerializer_ONVIF_PTZSpeed_INCLUDED


#include "ONVIF/PTZSpeed.h"
#include "ONVIF/Vector1DDeserializer.h"
#include "ONVIF/Vector1DSerializer.h"
#include "ONVIF/Vector2DDeserializer.h"
#include "ONVIF/Vector2DSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::PTZSpeed>
{
public:
	static void serialize(const std::string& name, const ONVIF::PTZSpeed& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::PTZSpeed& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"PanTilt"s,"Zoom"s,""s};
		TypeSerializer<Poco::SharedPtr<ONVIF::Vector2D>>::serialize(REMOTING__NAMES[0], value.getPanTilt(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::Vector1D>>::serialize(REMOTING__NAMES[1], value.getZoom(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_PTZSpeed_INCLUDED

