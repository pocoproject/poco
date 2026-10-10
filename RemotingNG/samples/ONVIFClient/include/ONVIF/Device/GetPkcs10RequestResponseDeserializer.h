//
// GetPkcs10RequestResponseDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Device_GetPkcs10RequestResponse_INCLUDED
#define TypeDeserializer_ONVIF_Device_GetPkcs10RequestResponse_INCLUDED


#include "ONVIF/BinaryDataDeserializer.h"
#include "ONVIF/BinaryDataSerializer.h"
#include "ONVIF/Device/GetPkcs10RequestResponse.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Device::GetPkcs10RequestResponse>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Device::GetPkcs10RequestResponse& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/device/wsdl"s);
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Device::GetPkcs10RequestResponse& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Pkcs10Request"s};
		bool ret = false;
		ONVIF::BinaryData gen_pkcs10Request;
		ret = TypeDeserializer<ONVIF::BinaryData>::deserialize(REMOTING__NAMES[0], true, deser, gen_pkcs10Request);
		if (ret) value.setPkcs10Request(gen_pkcs10Request);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Device_GetPkcs10RequestResponse_INCLUDED

