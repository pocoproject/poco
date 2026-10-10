//
// SerialPortConfigurationOptionsSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_DeviceIO_SerialPortConfigurationOptions_INCLUDED
#define TypeSerializer_ONVIF_DeviceIO_SerialPortConfigurationOptions_INCLUDED


#include "ONVIF/DeviceIO/ParityBitListDeserializer.h"
#include "ONVIF/DeviceIO/ParityBitListSerializer.h"
#include "ONVIF/DeviceIO/SerialPortConfigurationOptions.h"
#include "ONVIF/FloatItemsDeserializer.h"
#include "ONVIF/FloatItemsSerializer.h"
#include "ONVIF/IntItemsDeserializer.h"
#include "ONVIF/IntItemsSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::DeviceIO::SerialPortConfigurationOptions>
{
public:
	static void prepareSerializeAttributes(Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"token"s,"http://www.onvif.org/ver10/deviceIO/wsdl"s};
		ser.pushAttribute(REMOTING__NAMES[1], REMOTING__NAMES[0]);
	}

	static void serialize(const std::string& name, const ONVIF::DeviceIO::SerialPortConfigurationOptions& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/deviceIO/wsdl"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		prepareSerializeAttributes(ser);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeAttributes(value, ser);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeAttributes(const ONVIF::DeviceIO::SerialPortConfigurationOptions& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"token"s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getToken(), ser);
	}

	static void serializeImpl(const ONVIF::DeviceIO::SerialPortConfigurationOptions& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"BaudRateList"s,"ParityBitList"s,"CharacterLengthList"s,"StopBitList"s,""s};
		TypeSerializer<ONVIF::IntItems>::serialize(REMOTING__NAMES[0], value.getBaudRateList(), ser);
		TypeSerializer<ONVIF::DeviceIO::ParityBitList>::serialize(REMOTING__NAMES[1], value.getParityBitList(), ser);
		TypeSerializer<ONVIF::IntItems>::serialize(REMOTING__NAMES[2], value.getCharacterLengthList(), ser);
		TypeSerializer<ONVIF::FloatItems>::serialize(REMOTING__NAMES[3], value.getStopBitList(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_DeviceIO_SerialPortConfigurationOptions_INCLUDED

