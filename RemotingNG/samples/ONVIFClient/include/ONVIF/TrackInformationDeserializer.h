//
// TrackInformationDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_TrackInformation_INCLUDED
#define TypeDeserializer_ONVIF_TrackInformation_INCLUDED


#include "ONVIF/TrackInformation.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::TrackInformation>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::TrackInformation& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::TrackInformation& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"TrackToken"s,"TrackType"s,"Description"s,"DataFrom"s,"DataTo"s};
		bool ret = false;
		std::string gen_trackToken;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_trackToken);
		if (ret) value.setTrackToken(gen_trackToken);
		std::string gen_trackType;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[1], true, deser, gen_trackType);
		if (ret) value.setTrackType(gen_trackType);
		std::string gen_description;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[2], true, deser, gen_description);
		if (ret) value.setDescription(gen_description);
		deser.pushProperty(SerializerBase::PROP_XSDTYPE, "dateTime"s);
		Poco::DateTime gen_dataFrom;
		ret = TypeDeserializer<Poco::DateTime>::deserialize(REMOTING__NAMES[3], true, deser, gen_dataFrom);
		if (ret) value.setDataFrom(gen_dataFrom);
		deser.popProperty(SerializerBase::PROP_XSDTYPE);
		deser.pushProperty(SerializerBase::PROP_XSDTYPE, "dateTime"s);
		Poco::DateTime gen_dataTo;
		ret = TypeDeserializer<Poco::DateTime>::deserialize(REMOTING__NAMES[4], true, deser, gen_dataTo);
		if (ret) value.setDataTo(gen_dataTo);
		deser.popProperty(SerializerBase::PROP_XSDTYPE);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_TrackInformation_INCLUDED

