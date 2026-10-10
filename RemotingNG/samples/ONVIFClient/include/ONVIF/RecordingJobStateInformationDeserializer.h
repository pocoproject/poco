//
// RecordingJobStateInformationDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_RecordingJobStateInformation_INCLUDED
#define TypeDeserializer_ONVIF_RecordingJobStateInformation_INCLUDED


#include "ONVIF/RecordingJobStateInformation.h"
#include "ONVIF/RecordingJobStateInformationExtensionDeserializer.h"
#include "ONVIF/RecordingJobStateInformationExtensionSerializer.h"
#include "ONVIF/RecordingJobStateSourceDeserializer.h"
#include "ONVIF/RecordingJobStateSourceSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::RecordingJobStateInformation>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::RecordingJobStateInformation& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::RecordingJobStateInformation& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"RecordingToken"s,"State"s,"Sources"s,"Extension"s};
		bool ret = false;
		std::string gen_recordingToken;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_recordingToken);
		if (ret) value.setRecordingToken(gen_recordingToken);
		std::string gen_state;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[1], true, deser, gen_state);
		if (ret) value.setState(gen_state);
		std::vector<ONVIF::RecordingJobStateSource> gen_sources;
		ret = TypeDeserializer<std::vector<ONVIF::RecordingJobStateSource>>::deserialize(REMOTING__NAMES[2], false, deser, gen_sources);
		if (ret) value.setSources(gen_sources);
		Poco::SharedPtr<ONVIF::RecordingJobStateInformationExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::RecordingJobStateInformationExtension>>::deserialize(REMOTING__NAMES[3], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_RecordingJobStateInformation_INCLUDED

