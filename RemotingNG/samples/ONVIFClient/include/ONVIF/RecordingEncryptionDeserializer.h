//
// RecordingEncryptionDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_RecordingEncryption_INCLUDED
#define TypeDeserializer_ONVIF_RecordingEncryption_INCLUDED


#include "ONVIF/RecordingEncryption.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::RecordingEncryption>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::RecordingEncryption& value)
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

	static void deserializeAttributes(Deserializer& deser, ONVIF::RecordingEncryption& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Mode"s};
		bool ret = false;
		std::string gen_mode;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_mode);
		if (ret) value.setMode(gen_mode);
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::RecordingEncryption& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"KID"s,"Key"s,"Track"s};
		bool ret = false;
		std::string gen_kID;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_kID);
		if (ret) value.setKID(gen_kID);
		Poco::Optional<std::string> gen_key;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[1], false, deser, gen_key);
		if (ret) value.setKey(gen_key);
		std::vector<std::string> gen_track;
		ret = TypeDeserializer<std::vector<std::string>>::deserialize(REMOTING__NAMES[2], false, deser, gen_track);
		if (ret) value.setTrack(gen_track);
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Mode"s,"http://www.onvif.org/ver10/schema"s};
		deser.pushAttribute(REMOTING__NAMES[1], REMOTING__NAMES[0], true);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_RecordingEncryption_INCLUDED

