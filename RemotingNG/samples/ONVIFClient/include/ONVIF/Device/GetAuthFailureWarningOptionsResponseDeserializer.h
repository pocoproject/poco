//
// GetAuthFailureWarningOptionsResponseDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Device_GetAuthFailureWarningOptionsResponse_INCLUDED
#define TypeDeserializer_ONVIF_Device_GetAuthFailureWarningOptionsResponse_INCLUDED


#include "ONVIF/Device/GetAuthFailureWarningOptionsResponse.h"
#include "ONVIF/IntRangeDeserializer.h"
#include "ONVIF/IntRangeSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Device::GetAuthFailureWarningOptionsResponse>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Device::GetAuthFailureWarningOptionsResponse& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Device::GetAuthFailureWarningOptionsResponse& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"MonitorPeriodRange"s,"AuthFailureRange"s};
		bool ret = false;
		ONVIF::IntRange gen_monitorPeriodRange;
		ret = TypeDeserializer<ONVIF::IntRange>::deserialize(REMOTING__NAMES[0], true, deser, gen_monitorPeriodRange);
		if (ret) value.setMonitorPeriodRange(gen_monitorPeriodRange);
		ONVIF::IntRange gen_authFailureRange;
		ret = TypeDeserializer<ONVIF::IntRange>::deserialize(REMOTING__NAMES[1], true, deser, gen_authFailureRange);
		if (ret) value.setAuthFailureRange(gen_authFailureRange);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Device_GetAuthFailureWarningOptionsResponse_INCLUDED

