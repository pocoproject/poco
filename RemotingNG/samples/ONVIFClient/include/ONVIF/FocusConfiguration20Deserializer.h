//
// FocusConfiguration20Deserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_FocusConfiguration20_INCLUDED
#define TypeDeserializer_ONVIF_FocusConfiguration20_INCLUDED


#include "ONVIF/FocusConfiguration20.h"
#include "ONVIF/FocusConfiguration20ExtensionDeserializer.h"
#include "ONVIF/FocusConfiguration20ExtensionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::FocusConfiguration20>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::FocusConfiguration20& value)
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

	static void deserializeAttributes(Deserializer& deser, ONVIF::FocusConfiguration20& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"AFMode"s};
		bool ret = false;
		std::vector<std::vector<std::string>> gen_aFMode;
		ret = TypeDeserializer<std::vector<std::vector<std::string>>>::deserialize(REMOTING__NAMES[0], false, deser, gen_aFMode);
		if (ret) value.setAFMode(gen_aFMode);
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::FocusConfiguration20& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"AutoFocusMode"s,"DefaultSpeed"s,"NearLimit"s,"FarLimit"s,"Extension"s};
		bool ret = false;
		std::string gen_autoFocusMode;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_autoFocusMode);
		if (ret) value.setAutoFocusMode(gen_autoFocusMode);
		Poco::Optional<float> gen_defaultSpeed;
		ret = TypeDeserializer<Poco::Optional<float>>::deserialize(REMOTING__NAMES[1], false, deser, gen_defaultSpeed);
		if (ret) value.setDefaultSpeed(gen_defaultSpeed);
		Poco::Optional<float> gen_nearLimit;
		ret = TypeDeserializer<Poco::Optional<float>>::deserialize(REMOTING__NAMES[2], false, deser, gen_nearLimit);
		if (ret) value.setNearLimit(gen_nearLimit);
		Poco::Optional<float> gen_farLimit;
		ret = TypeDeserializer<Poco::Optional<float>>::deserialize(REMOTING__NAMES[3], false, deser, gen_farLimit);
		if (ret) value.setFarLimit(gen_farLimit);
		Poco::SharedPtr<ONVIF::FocusConfiguration20Extension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::FocusConfiguration20Extension>>::deserialize(REMOTING__NAMES[4], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"AFMode"s,"http://www.onvif.org/ver10/schema"s};
		deser.pushAttribute(REMOTING__NAMES[1], REMOTING__NAMES[0], false);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_FocusConfiguration20_INCLUDED

