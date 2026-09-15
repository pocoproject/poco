//
// MaximumNumberOfOSDsDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_MaximumNumberOfOSDs_INCLUDED
#define TypeDeserializer_ONVIF_MaximumNumberOfOSDs_INCLUDED


#include "ONVIF/MaximumNumberOfOSDs.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::MaximumNumberOfOSDs>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::MaximumNumberOfOSDs& value)
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

	static void deserializeAttributes(Deserializer& deser, ONVIF::MaximumNumberOfOSDs& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Total"s,"Image"s,"PlainText"s,"Date"s,"Time"s,"DateAndTime"s};
		bool ret = false;
		int gen_total;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES[0], true, deser, gen_total);
		if (ret) value.setTotal(gen_total);
		Poco::Optional<int> gen_image;
		ret = TypeDeserializer<Poco::Optional<int>>::deserialize(REMOTING__NAMES[1], false, deser, gen_image);
		if (ret) value.setImage(gen_image);
		Poco::Optional<int> gen_plainText;
		ret = TypeDeserializer<Poco::Optional<int>>::deserialize(REMOTING__NAMES[2], false, deser, gen_plainText);
		if (ret) value.setPlainText(gen_plainText);
		Poco::Optional<int> gen_date;
		ret = TypeDeserializer<Poco::Optional<int>>::deserialize(REMOTING__NAMES[3], false, deser, gen_date);
		if (ret) value.setDate(gen_date);
		Poco::Optional<int> gen_time;
		ret = TypeDeserializer<Poco::Optional<int>>::deserialize(REMOTING__NAMES[4], false, deser, gen_time);
		if (ret) value.setTime(gen_time);
		Poco::Optional<int> gen_dateAndTime;
		ret = TypeDeserializer<Poco::Optional<int>>::deserialize(REMOTING__NAMES[5], false, deser, gen_dateAndTime);
		if (ret) value.setDateAndTime(gen_dateAndTime);
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::MaximumNumberOfOSDs& value)
	{
		using namespace std::string_literals;
		
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Total"s,"Image"s,"PlainText"s,"Date"s,"Time"s,"DateAndTime"s,"http://www.onvif.org/ver10/schema"s};
		deser.pushAttribute(REMOTING__NAMES[6], REMOTING__NAMES[0], true);
		deser.pushAttribute(REMOTING__NAMES[6], REMOTING__NAMES[1], false);
		deser.pushAttribute(REMOTING__NAMES[6], REMOTING__NAMES[2], false);
		deser.pushAttribute(REMOTING__NAMES[6], REMOTING__NAMES[3], false);
		deser.pushAttribute(REMOTING__NAMES[6], REMOTING__NAMES[4], false);
		deser.pushAttribute(REMOTING__NAMES[6], REMOTING__NAMES[5], false);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_MaximumNumberOfOSDs_INCLUDED

