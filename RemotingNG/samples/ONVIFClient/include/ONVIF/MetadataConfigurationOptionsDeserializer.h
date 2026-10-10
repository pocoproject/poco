//
// MetadataConfigurationOptionsDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_MetadataConfigurationOptions_INCLUDED
#define TypeDeserializer_ONVIF_MetadataConfigurationOptions_INCLUDED


#include "ONVIF/MetadataConfigurationOptions.h"
#include "ONVIF/MetadataConfigurationOptionsExtensionDeserializer.h"
#include "ONVIF/MetadataConfigurationOptionsExtensionSerializer.h"
#include "ONVIF/PTZStatusFilterOptionsDeserializer.h"
#include "ONVIF/PTZStatusFilterOptionsSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::MetadataConfigurationOptions>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::MetadataConfigurationOptions& value)
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

	static void deserializeAttributes(Deserializer& deser, ONVIF::MetadataConfigurationOptions& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"GeoLocation"s,"MaxContentFilterSize"s};
		bool ret = false;
		Poco::Optional<bool> gen_geoLocation;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[0], false, deser, gen_geoLocation);
		if (ret) value.setGeoLocation(gen_geoLocation);
		Poco::Optional<int> gen_maxContentFilterSize;
		ret = TypeDeserializer<Poco::Optional<int>>::deserialize(REMOTING__NAMES[1], false, deser, gen_maxContentFilterSize);
		if (ret) value.setMaxContentFilterSize(gen_maxContentFilterSize);
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::MetadataConfigurationOptions& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"PTZStatusFilterOptions"s,"Extension"s};
		bool ret = false;
		ONVIF::PTZStatusFilterOptions gen_pTZStatusFilterOptions;
		ret = TypeDeserializer<ONVIF::PTZStatusFilterOptions>::deserialize(REMOTING__NAMES[0], true, deser, gen_pTZStatusFilterOptions);
		if (ret) value.setPTZStatusFilterOptions(gen_pTZStatusFilterOptions);
		Poco::SharedPtr<ONVIF::MetadataConfigurationOptionsExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::MetadataConfigurationOptionsExtension>>::deserialize(REMOTING__NAMES[1], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"GeoLocation"s,"MaxContentFilterSize"s,"http://www.onvif.org/ver10/schema"s};
		deser.pushAttribute(REMOTING__NAMES[2], REMOTING__NAMES[0], false);
		deser.pushAttribute(REMOTING__NAMES[2], REMOTING__NAMES[1], false);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_MetadataConfigurationOptions_INCLUDED

