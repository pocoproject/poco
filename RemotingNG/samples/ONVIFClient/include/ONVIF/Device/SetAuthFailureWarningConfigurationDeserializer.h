//
// SetAuthFailureWarningConfigurationDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Device_SetAuthFailureWarningConfiguration_INCLUDED
#define TypeDeserializer_ONVIF_Device_SetAuthFailureWarningConfiguration_INCLUDED


#include "ONVIF/Device/SetAuthFailureWarningConfiguration.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Device::SetAuthFailureWarningConfiguration>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Device::SetAuthFailureWarningConfiguration& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Device::SetAuthFailureWarningConfiguration& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Enabled"s,"MonitorPeriod"s,"MaxAuthFailures"s};
		bool ret = false;
		bool gen_enabled;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[0], true, deser, gen_enabled);
		if (ret) value.setEnabled(gen_enabled);
		int gen_monitorPeriod;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES[1], true, deser, gen_monitorPeriod);
		if (ret) value.setMonitorPeriod(gen_monitorPeriod);
		int gen_maxAuthFailures;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES[2], true, deser, gen_maxAuthFailures);
		if (ret) value.setMaxAuthFailures(gen_maxAuthFailures);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Device_SetAuthFailureWarningConfiguration_INCLUDED

