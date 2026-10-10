//
// SupportedAnalyticsModulesDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_SupportedAnalyticsModules_INCLUDED
#define TypeDeserializer_ONVIF_SupportedAnalyticsModules_INCLUDED


#include "ONVIF/ConfigDescriptionDeserializer.h"
#include "ONVIF/ConfigDescriptionSerializer.h"
#include "ONVIF/SupportedAnalyticsModules.h"
#include "ONVIF/SupportedAnalyticsModulesExtensionDeserializer.h"
#include "ONVIF/SupportedAnalyticsModulesExtensionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::SupportedAnalyticsModules>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::SupportedAnalyticsModules& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
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

	static void deserializeAttributes(Deserializer& deser, ONVIF::SupportedAnalyticsModules& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Limit"s};
		bool ret = false;
		Poco::Optional<int> gen_limit;
		ret = TypeDeserializer<Poco::Optional<int>>::deserialize(REMOTING__NAMES[0], false, deser, gen_limit);
		if (ret) value.setLimit(gen_limit);
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::SupportedAnalyticsModules& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"AnalyticsModuleContentSchemaLocation"s,"AnalyticsModuleDescription"s,"Extension"s};
		bool ret = false;
		std::vector<Poco::URI> gen_analyticsModuleContentSchemaLocation;
		ret = TypeDeserializer<std::vector<Poco::URI>>::deserialize(REMOTING__NAMES[0], false, deser, gen_analyticsModuleContentSchemaLocation);
		if (ret) value.setAnalyticsModuleContentSchemaLocation(gen_analyticsModuleContentSchemaLocation);
		std::vector<ONVIF::ConfigDescription> gen_analyticsModuleDescription;
		ret = TypeDeserializer<std::vector<ONVIF::ConfigDescription>>::deserialize(REMOTING__NAMES[1], false, deser, gen_analyticsModuleDescription);
		if (ret) value.setAnalyticsModuleDescription(gen_analyticsModuleDescription);
		Poco::SharedPtr<ONVIF::SupportedAnalyticsModulesExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::SupportedAnalyticsModulesExtension>>::deserialize(REMOTING__NAMES[2], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Limit"s,"http://www.onvif.org/ver10/schema"s};
		deser.pushAttribute(REMOTING__NAMES[1], REMOTING__NAMES[0], false);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_SupportedAnalyticsModules_INCLUDED

