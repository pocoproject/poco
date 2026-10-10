//
// SetPasswordHistoryConfigurationSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_Device_SetPasswordHistoryConfiguration_INCLUDED
#define TypeSerializer_ONVIF_Device_SetPasswordHistoryConfiguration_INCLUDED


#include "ONVIF/Device/SetPasswordHistoryConfiguration.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::Device::SetPasswordHistoryConfiguration>
{
public:
	static void serialize(const std::string& name, const ONVIF::Device::SetPasswordHistoryConfiguration& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/device/wsdl"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeImpl(const ONVIF::Device::SetPasswordHistoryConfiguration& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Enabled"s,"Length"s,""s};
		TypeSerializer<bool>::serialize(REMOTING__NAMES[0], value.getEnabled(), ser);
		TypeSerializer<int>::serialize(REMOTING__NAMES[1], value.getLength(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_Device_SetPasswordHistoryConfiguration_INCLUDED

