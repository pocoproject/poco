//
// PTZConfigurationOptionsDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_PTZConfigurationOptions_INCLUDED
#define TypeDeserializer_ONVIF_PTZConfigurationOptions_INCLUDED


#include "ONVIF/DurationRangeDeserializer.h"
#include "ONVIF/DurationRangeSerializer.h"
#include "ONVIF/PTControlDirectionOptionsDeserializer.h"
#include "ONVIF/PTControlDirectionOptionsSerializer.h"
#include "ONVIF/PTZConfigurationOptions.h"
#include "ONVIF/PTZConfigurationOptions2Deserializer.h"
#include "ONVIF/PTZConfigurationOptions2Serializer.h"
#include "ONVIF/PTZSpacesDeserializer.h"
#include "ONVIF/PTZSpacesSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::PTZConfigurationOptions>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::PTZConfigurationOptions& value)
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

	static void deserializeAttributes(Deserializer& deser, ONVIF::PTZConfigurationOptions& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"PTZRamps"s};
		bool ret = false;
		std::vector<std::vector<int>> gen_pTZRamps;
		ret = TypeDeserializer<std::vector<std::vector<int>>>::deserialize(REMOTING__NAMES[0], false, deser, gen_pTZRamps);
		if (ret) value.setPTZRamps(gen_pTZRamps);
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::PTZConfigurationOptions& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Spaces"s,"PTZTimeout"s,"PTControlDirection"s,"Extension"s};
		bool ret = false;
		ONVIF::PTZSpaces gen_spaces;
		ret = TypeDeserializer<ONVIF::PTZSpaces>::deserialize(REMOTING__NAMES[0], true, deser, gen_spaces);
		if (ret) value.setSpaces(gen_spaces);
		ONVIF::DurationRange gen_pTZTimeout;
		ret = TypeDeserializer<ONVIF::DurationRange>::deserialize(REMOTING__NAMES[1], true, deser, gen_pTZTimeout);
		if (ret) value.setPTZTimeout(gen_pTZTimeout);
		Poco::SharedPtr<ONVIF::PTControlDirectionOptions> gen_pTControlDirection;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::PTControlDirectionOptions>>::deserialize(REMOTING__NAMES[2], false, deser, gen_pTControlDirection);
		if (ret) value.setPTControlDirection(gen_pTControlDirection);
		Poco::SharedPtr<ONVIF::PTZConfigurationOptions2> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::PTZConfigurationOptions2>>::deserialize(REMOTING__NAMES[3], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"PTZRamps"s,"http://www.onvif.org/ver10/schema"s};
		deser.pushAttribute(REMOTING__NAMES[1], REMOTING__NAMES[0], false);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_PTZConfigurationOptions_INCLUDED

