//
// SetNTPDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Device_SetNTP_INCLUDED
#define TypeDeserializer_ONVIF_Device_SetNTP_INCLUDED


#include "ONVIF/Device/SetNTP.h"
#include "ONVIF/NetworkHostDeserializer.h"
#include "ONVIF/NetworkHostSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Device::SetNTP>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Device::SetNTP& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Device::SetNTP& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"FromDHCP"s,"NTPManual"s};
		bool ret = false;
		bool gen_fromDHCP;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[0], true, deser, gen_fromDHCP);
		if (ret) value.setFromDHCP(gen_fromDHCP);
		std::vector<ONVIF::NetworkHost> gen_nTPManual;
		ret = TypeDeserializer<std::vector<ONVIF::NetworkHost>>::deserialize(REMOTING__NAMES[1], false, deser, gen_nTPManual);
		if (ret) value.setNTPManual(gen_nTPManual);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Device_SetNTP_INCLUDED

