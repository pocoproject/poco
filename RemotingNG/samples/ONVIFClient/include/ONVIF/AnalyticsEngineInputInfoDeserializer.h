//
// AnalyticsEngineInputInfoDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_AnalyticsEngineInputInfo_INCLUDED
#define TypeDeserializer_ONVIF_AnalyticsEngineInputInfo_INCLUDED


#include "ONVIF/AnalyticsEngineInputInfo.h"
#include "ONVIF/AnalyticsEngineInputInfoExtensionDeserializer.h"
#include "ONVIF/AnalyticsEngineInputInfoExtensionSerializer.h"
#include "ONVIF/ConfigDeserializer.h"
#include "ONVIF/ConfigSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::AnalyticsEngineInputInfo>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::AnalyticsEngineInputInfo& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::AnalyticsEngineInputInfo& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"InputInfo"s,"Extension"s};
		bool ret = false;
		Poco::SharedPtr<ONVIF::Config> gen_inputInfo;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::Config>>::deserialize(REMOTING__NAMES[0], false, deser, gen_inputInfo);
		if (ret) value.setInputInfo(gen_inputInfo);
		Poco::SharedPtr<ONVIF::AnalyticsEngineInputInfoExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::AnalyticsEngineInputInfoExtension>>::deserialize(REMOTING__NAMES[1], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_AnalyticsEngineInputInfo_INCLUDED

