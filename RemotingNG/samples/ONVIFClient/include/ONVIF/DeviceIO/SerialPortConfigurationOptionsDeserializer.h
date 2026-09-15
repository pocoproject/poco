//
// SerialPortConfigurationOptionsDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_DeviceIO_SerialPortConfigurationOptions_INCLUDED
#define TypeDeserializer_ONVIF_DeviceIO_SerialPortConfigurationOptions_INCLUDED


#include "ONVIF/DeviceIO/ParityBitListDeserializer.h"
#include "ONVIF/DeviceIO/ParityBitListSerializer.h"
#include "ONVIF/DeviceIO/SerialPortConfigurationOptions.h"
#include "ONVIF/FloatItemsDeserializer.h"
#include "ONVIF/FloatItemsSerializer.h"
#include "ONVIF/IntItemsDeserializer.h"
#include "ONVIF/IntItemsSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::DeviceIO::SerialPortConfigurationOptions>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::DeviceIO::SerialPortConfigurationOptions& value)
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

	static void deserializeAttributes(Deserializer& deser, ONVIF::DeviceIO::SerialPortConfigurationOptions& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"token"s};
		bool ret = false;
		std::string gen_token;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_token);
		if (ret) value.setToken(gen_token);
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::DeviceIO::SerialPortConfigurationOptions& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"BaudRateList"s,"ParityBitList"s,"CharacterLengthList"s,"StopBitList"s};
		bool ret = false;
		ONVIF::IntItems gen_baudRateList;
		ret = TypeDeserializer<ONVIF::IntItems>::deserialize(REMOTING__NAMES[0], true, deser, gen_baudRateList);
		if (ret) value.setBaudRateList(gen_baudRateList);
		ONVIF::DeviceIO::ParityBitList gen_parityBitList;
		ret = TypeDeserializer<ONVIF::DeviceIO::ParityBitList>::deserialize(REMOTING__NAMES[1], true, deser, gen_parityBitList);
		if (ret) value.setParityBitList(gen_parityBitList);
		ONVIF::IntItems gen_characterLengthList;
		ret = TypeDeserializer<ONVIF::IntItems>::deserialize(REMOTING__NAMES[2], true, deser, gen_characterLengthList);
		if (ret) value.setCharacterLengthList(gen_characterLengthList);
		ONVIF::FloatItems gen_stopBitList;
		ret = TypeDeserializer<ONVIF::FloatItems>::deserialize(REMOTING__NAMES[3], true, deser, gen_stopBitList);
		if (ret) value.setStopBitList(gen_stopBitList);
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"token"s,"http://www.onvif.org/ver10/deviceIO/wsdl"s};
		deser.pushAttribute(REMOTING__NAMES[1], REMOTING__NAMES[0], true);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_DeviceIO_SerialPortConfigurationOptions_INCLUDED

