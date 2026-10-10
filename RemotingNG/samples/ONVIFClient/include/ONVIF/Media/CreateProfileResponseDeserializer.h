//
// CreateProfileResponseDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Media_CreateProfileResponse_INCLUDED
#define TypeDeserializer_ONVIF_Media_CreateProfileResponse_INCLUDED


#include "ONVIF/Media/CreateProfileResponse.h"
#include "ONVIF/ProfileDeserializer.h"
#include "ONVIF/ProfileSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Media::CreateProfileResponse>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Media::CreateProfileResponse& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Media::CreateProfileResponse& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Profile"s};
		bool ret = false;
		ONVIF::Profile gen_profile;
		ret = TypeDeserializer<ONVIF::Profile>::deserialize(REMOTING__NAMES[0], true, deser, gen_profile);
		if (ret) value.setProfile(gen_profile);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Media_CreateProfileResponse_INCLUDED

