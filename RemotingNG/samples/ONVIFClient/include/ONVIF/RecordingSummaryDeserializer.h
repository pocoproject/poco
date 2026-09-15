//
// RecordingSummaryDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_RecordingSummary_INCLUDED
#define TypeDeserializer_ONVIF_RecordingSummary_INCLUDED


#include "ONVIF/RecordingSummary.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::RecordingSummary>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::RecordingSummary& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::RecordingSummary& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"DataFrom"s,"DataUntil"s,"NumberRecordings"s};
		bool ret = false;
		deser.pushProperty(SerializerBase::PROP_XSDTYPE, "dateTime"s);
		Poco::DateTime gen_dataFrom;
		ret = TypeDeserializer<Poco::DateTime>::deserialize(REMOTING__NAMES[0], true, deser, gen_dataFrom);
		if (ret) value.setDataFrom(gen_dataFrom);
		deser.popProperty(SerializerBase::PROP_XSDTYPE);
		deser.pushProperty(SerializerBase::PROP_XSDTYPE, "dateTime"s);
		Poco::DateTime gen_dataUntil;
		ret = TypeDeserializer<Poco::DateTime>::deserialize(REMOTING__NAMES[1], true, deser, gen_dataUntil);
		if (ret) value.setDataUntil(gen_dataUntil);
		deser.popProperty(SerializerBase::PROP_XSDTYPE);
		int gen_numberRecordings;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES[2], true, deser, gen_numberRecordings);
		if (ret) value.setNumberRecordings(gen_numberRecordings);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_RecordingSummary_INCLUDED

