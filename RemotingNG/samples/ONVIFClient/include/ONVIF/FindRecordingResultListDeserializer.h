//
// FindRecordingResultListDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_FindRecordingResultList_INCLUDED
#define TypeDeserializer_ONVIF_FindRecordingResultList_INCLUDED


#include "ONVIF/FindRecordingResultList.h"
#include "ONVIF/RecordingInformationDeserializer.h"
#include "ONVIF/RecordingInformationSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::FindRecordingResultList>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::FindRecordingResultList& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::FindRecordingResultList& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"SearchState"s,"RecordingInformation"s};
		bool ret = false;
		std::string gen_searchState;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_searchState);
		if (ret) value.setSearchState(gen_searchState);
		std::vector<ONVIF::RecordingInformation> gen_recordingInformation;
		ret = TypeDeserializer<std::vector<ONVIF::RecordingInformation>>::deserialize(REMOTING__NAMES[1], false, deser, gen_recordingInformation);
		if (ret) value.setRecordingInformation(gen_recordingInformation);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_FindRecordingResultList_INCLUDED

