//
// AnalyticsStateInformationDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_AnalyticsStateInformation_INCLUDED
#define TypeDeserializer_ONVIF_AnalyticsStateInformation_INCLUDED


#include "ONVIF/AnalyticsStateDeserializer.h"
#include "ONVIF/AnalyticsStateInformation.h"
#include "ONVIF/AnalyticsStateSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::AnalyticsStateInformation>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::AnalyticsStateInformation& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
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

	static void deserializeImpl(Deserializer& deser, ONVIF::AnalyticsStateInformation& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"AnalyticsEngineControlToken"s,"State"s};
		bool ret = false;
		std::string gen_analyticsEngineControlToken;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_analyticsEngineControlToken);
		if (ret) value.setAnalyticsEngineControlToken(gen_analyticsEngineControlToken);
		ONVIF::AnalyticsState gen_state;
		ret = TypeDeserializer<ONVIF::AnalyticsState>::deserialize(REMOTING__NAMES[1], true, deser, gen_state);
		if (ret) value.setState(gen_state);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_AnalyticsStateInformation_INCLUDED

