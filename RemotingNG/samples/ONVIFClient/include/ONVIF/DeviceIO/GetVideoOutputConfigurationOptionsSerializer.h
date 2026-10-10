//
// GetVideoOutputConfigurationOptionsSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_DeviceIO_GetVideoOutputConfigurationOptions_INCLUDED
#define TypeSerializer_ONVIF_DeviceIO_GetVideoOutputConfigurationOptions_INCLUDED


#include "ONVIF/DeviceIO/GetVideoOutputConfigurationOptions.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::DeviceIO::GetVideoOutputConfigurationOptions>
{
public:
	static void serialize(const std::string& name, const ONVIF::DeviceIO::GetVideoOutputConfigurationOptions& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/deviceIO/wsdl"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeImpl(const ONVIF::DeviceIO::GetVideoOutputConfigurationOptions& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"VideoOutputToken"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getVideoOutputToken(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_DeviceIO_GetVideoOutputConfigurationOptions_INCLUDED

