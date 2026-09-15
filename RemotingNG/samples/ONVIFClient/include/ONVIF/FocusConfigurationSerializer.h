//
// FocusConfigurationSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_FocusConfiguration_INCLUDED
#define TypeSerializer_ONVIF_FocusConfiguration_INCLUDED


#include "ONVIF/FocusConfiguration.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::FocusConfiguration>
{
public:
	static void serialize(const std::string& name, const ONVIF::FocusConfiguration& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::FocusConfiguration& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"AutoFocusMode"s,"DefaultSpeed"s,"NearLimit"s,"FarLimit"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getAutoFocusMode(), ser);
		TypeSerializer<float>::serialize(REMOTING__NAMES[1], value.getDefaultSpeed(), ser);
		TypeSerializer<float>::serialize(REMOTING__NAMES[2], value.getNearLimit(), ser);
		TypeSerializer<float>::serialize(REMOTING__NAMES[3], value.getFarLimit(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_FocusConfiguration_INCLUDED

