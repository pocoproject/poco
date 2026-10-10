//
// SendReceiveSerialCommandDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_DeviceIO_SendReceiveSerialCommand_INCLUDED
#define TypeDeserializer_ONVIF_DeviceIO_SendReceiveSerialCommand_INCLUDED


#include "ONVIF/DeviceIO/SendReceiveSerialCommand.h"
#include "ONVIF/DeviceIO/SerialDataDeserializer.h"
#include "ONVIF/DeviceIO/SerialDataSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::DeviceIO::SendReceiveSerialCommand>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::DeviceIO::SendReceiveSerialCommand& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/deviceIO/wsdl"s);
		bool ret = deser.deserializeStructBegin(name, isMandatory);
		deser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		if (ret)
		{
			deserializeImpl(deser, value);
			deser.popProperty(SerializerBase::PROP_NAMESPACE);
			deser.deserializeStructEnd(name);
		}
		else deser.popProperty(SerializerBase::PROP_NAMESPACE);
		return ret;
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::DeviceIO::SendReceiveSerialCommand& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Token"s,"SerialData"s,"TimeOut"s,"DataLength"s,"Delimiter"s};
		bool ret = false;
		Poco::Optional<std::string> gen_token;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[0], false, deser, gen_token);
		if (ret) value.setToken(gen_token);
		Poco::SharedPtr<ONVIF::DeviceIO::SerialData> gen_serialData;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::DeviceIO::SerialData>>::deserialize(REMOTING__NAMES[1], false, deser, gen_serialData);
		if (ret) value.setSerialData(gen_serialData);
		Poco::Optional<std::string> gen_timeOut;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[2], false, deser, gen_timeOut);
		if (ret) value.setTimeOut(gen_timeOut);
		Poco::Optional<int> gen_dataLength;
		ret = TypeDeserializer<Poco::Optional<int>>::deserialize(REMOTING__NAMES[3], false, deser, gen_dataLength);
		if (ret) value.setDataLength(gen_dataLength);
		Poco::Optional<std::string> gen_delimiter;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[4], false, deser, gen_delimiter);
		if (ret) value.setDelimiter(gen_delimiter);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_DeviceIO_SendReceiveSerialCommand_INCLUDED

