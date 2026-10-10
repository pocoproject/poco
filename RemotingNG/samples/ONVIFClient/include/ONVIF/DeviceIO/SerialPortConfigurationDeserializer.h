//
// SerialPortConfigurationDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_DeviceIO_SerialPortConfiguration_INCLUDED
#define TypeDeserializer_ONVIF_DeviceIO_SerialPortConfiguration_INCLUDED


#include "ONVIF/DeviceIO/SerialPortConfiguration.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::DeviceIO::SerialPortConfiguration>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::DeviceIO::SerialPortConfiguration& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/deviceIO/wsdl"s);
		prepareDeserializeAttributes(deser);
		bool ret = deser.deserializeStructBegin(name, isMandatory);
		deser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		if (ret)
		{
			deserializeAttributes(deser, value);
			deserializeImpl(deser, value);
			deser.popProperty(SerializerBase::PROP_NAMESPACE);
			deser.deserializeStructEnd(name);
		}
		else deser.popProperty(SerializerBase::PROP_NAMESPACE);
		return ret;
	}

	static void deserializeAttributes(Deserializer& deser, ONVIF::DeviceIO::SerialPortConfiguration& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"token"s,"type"s};
		bool ret = false;
		std::string gen_token;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_token);
		if (ret) value.setToken(gen_token);
		std::string gen_type;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[1], true, deser, gen_type);
		if (ret) value.setType(gen_type);
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::DeviceIO::SerialPortConfiguration& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"BaudRate"s,"ParityBit"s,"CharacterLength"s,"StopBit"s};
		bool ret = false;
		int gen_baudRate;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES[0], true, deser, gen_baudRate);
		if (ret) value.setBaudRate(gen_baudRate);
		std::string gen_parityBit;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[1], true, deser, gen_parityBit);
		if (ret) value.setParityBit(gen_parityBit);
		int gen_characterLength;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES[2], true, deser, gen_characterLength);
		if (ret) value.setCharacterLength(gen_characterLength);
		float gen_stopBit;
		ret = TypeDeserializer<float>::deserialize(REMOTING__NAMES[3], true, deser, gen_stopBit);
		if (ret) value.setStopBit(gen_stopBit);
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"token"s,"type"s,"http://www.onvif.org/ver10/deviceIO/wsdl"s};
		deser.pushAttribute(REMOTING__NAMES[2], REMOTING__NAMES[0], true);
		deser.pushAttribute(REMOTING__NAMES[2], REMOTING__NAMES[1], true);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_DeviceIO_SerialPortConfiguration_INCLUDED

