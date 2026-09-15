//
// SearchScopeDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_SearchScope_INCLUDED
#define TypeDeserializer_ONVIF_SearchScope_INCLUDED


#include "ONVIF/SearchScope.h"
#include "ONVIF/SearchScopeExtensionDeserializer.h"
#include "ONVIF/SearchScopeExtensionSerializer.h"
#include "ONVIF/SourceReferenceDeserializer.h"
#include "ONVIF/SourceReferenceSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::SearchScope>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::SearchScope& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::SearchScope& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"IncludedSources"s,"IncludedRecordings"s,"RecordingInformationFilter"s,"Extension"s};
		bool ret = false;
		std::vector<ONVIF::SourceReference> gen_includedSources;
		ret = TypeDeserializer<std::vector<ONVIF::SourceReference>>::deserialize(REMOTING__NAMES[0], false, deser, gen_includedSources);
		if (ret) value.setIncludedSources(gen_includedSources);
		std::vector<std::string> gen_includedRecordings;
		ret = TypeDeserializer<std::vector<std::string>>::deserialize(REMOTING__NAMES[1], false, deser, gen_includedRecordings);
		if (ret) value.setIncludedRecordings(gen_includedRecordings);
		Poco::Optional<std::string> gen_recordingInformationFilter;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[2], false, deser, gen_recordingInformationFilter);
		if (ret) value.setRecordingInformationFilter(gen_recordingInformationFilter);
		Poco::SharedPtr<ONVIF::SearchScopeExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::SearchScopeExtension>>::deserialize(REMOTING__NAMES[3], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_SearchScope_INCLUDED

