//
// GetDigitalInputConfigurationOptionsResponseSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_DeviceIO_GetDigitalInputConfigurationOptionsResponse_INCLUDED
#define TypeSerializer_ONVIF_DeviceIO_GetDigitalInputConfigurationOptionsResponse_INCLUDED


#include "ONVIF/DeviceIO/DigitalInputConfigurationOptionsDeserializer.h"
#include "ONVIF/DeviceIO/DigitalInputConfigurationOptionsSerializer.h"
#include "ONVIF/DeviceIO/GetDigitalInputConfigurationOptionsResponse.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::DeviceIO::GetDigitalInputConfigurationOptionsResponse>
{
public:
	static void serialize(const std::string& name, const ONVIF::DeviceIO::GetDigitalInputConfigurationOptionsResponse& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::DeviceIO::GetDigitalInputConfigurationOptionsResponse& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"DigitalInputOptions"s,""s};
		TypeSerializer<ONVIF::DeviceIO::DigitalInputConfigurationOptions>::serialize(REMOTING__NAMES[0], value.getDigitalInputOptions(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_DeviceIO_GetDigitalInputConfigurationOptionsResponse_INCLUDED

