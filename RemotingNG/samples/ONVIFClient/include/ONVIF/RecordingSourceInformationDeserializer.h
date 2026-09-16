//
// RecordingSourceInformationDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_RecordingSourceInformation_INCLUDED
#define TypeDeserializer_ONVIF_RecordingSourceInformation_INCLUDED


#include "ONVIF/RecordingSourceInformation.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::RecordingSourceInformation>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::RecordingSourceInformation& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::RecordingSourceInformation& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"SourceId"s,"Name"s,"Location"s,"Description"s,"Address"s};
		bool ret = false;
		Poco::URI gen_sourceId;
		ret = TypeDeserializer<Poco::URI>::deserialize(REMOTING__NAMES[0], true, deser, gen_sourceId);
		if (ret) value.setSourceId(gen_sourceId);
		std::string gen_name;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[1], true, deser, gen_name);
		if (ret) value.setName(gen_name);
		std::string gen_location;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[2], true, deser, gen_location);
		if (ret) value.setLocation(gen_location);
		std::string gen_description;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[3], true, deser, gen_description);
		if (ret) value.setDescription(gen_description);
		Poco::URI gen_address;
		ret = TypeDeserializer<Poco::URI>::deserialize(REMOTING__NAMES[4], true, deser, gen_address);
		if (ret) value.setAddress(gen_address);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_RecordingSourceInformation_INCLUDED

