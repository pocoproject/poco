//
// MetadataAttributesDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_MetadataAttributes_INCLUDED
#define TypeDeserializer_ONVIF_MetadataAttributes_INCLUDED


#include "ONVIF/MetadataAttributes.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::MetadataAttributes>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::MetadataAttributes& value)
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

	static void deserializeAttributes(Deserializer& deser, ONVIF::MetadataAttributes& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"PtzSpaces"s};
		bool ret = false;
		std::vector<std::vector<std::string>> gen_ptzSpaces;
		ret = TypeDeserializer<std::vector<std::vector<std::string>>>::deserialize(REMOTING__NAMES[0], false, deser, gen_ptzSpaces);
		if (ret) value.setPtzSpaces(gen_ptzSpaces);
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::MetadataAttributes& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"CanContainPTZ"s,"CanContainAnalytics"s,"CanContainNotifications"s};
		bool ret = false;
		bool gen_canContainPTZ;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[0], true, deser, gen_canContainPTZ);
		if (ret) value.setCanContainPTZ(gen_canContainPTZ);
		bool gen_canContainAnalytics;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[1], true, deser, gen_canContainAnalytics);
		if (ret) value.setCanContainAnalytics(gen_canContainAnalytics);
		bool gen_canContainNotifications;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[2], true, deser, gen_canContainNotifications);
		if (ret) value.setCanContainNotifications(gen_canContainNotifications);
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"PtzSpaces"s,"http://www.onvif.org/ver10/schema"s};
		deser.pushAttribute(REMOTING__NAMES[1], REMOTING__NAMES[0], false);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_MetadataAttributes_INCLUDED

