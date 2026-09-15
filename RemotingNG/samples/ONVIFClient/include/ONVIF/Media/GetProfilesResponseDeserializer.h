//
// GetProfilesResponseDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Media_GetProfilesResponse_INCLUDED
#define TypeDeserializer_ONVIF_Media_GetProfilesResponse_INCLUDED


#include "ONVIF/Media/GetProfilesResponse.h"
#include "ONVIF/ProfileDeserializer.h"
#include "ONVIF/ProfileSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Media::GetProfilesResponse>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Media::GetProfilesResponse& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/media/wsdl"s);
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Media::GetProfilesResponse& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Profiles"s};
		bool ret = false;
		std::vector<ONVIF::Profile> gen_profiles;
		ret = TypeDeserializer<std::vector<ONVIF::Profile>>::deserialize(REMOTING__NAMES[0], false, deser, gen_profiles);
		if (ret) value.setProfiles(gen_profiles);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Media_GetProfilesResponse_INCLUDED

