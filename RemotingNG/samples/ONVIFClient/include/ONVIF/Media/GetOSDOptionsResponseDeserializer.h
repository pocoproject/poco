//
// GetOSDOptionsResponseDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Media_GetOSDOptionsResponse_INCLUDED
#define TypeDeserializer_ONVIF_Media_GetOSDOptionsResponse_INCLUDED


#include "ONVIF/Media/GetOSDOptionsResponse.h"
#include "ONVIF/OSDConfigurationOptionsDeserializer.h"
#include "ONVIF/OSDConfigurationOptionsSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Media::GetOSDOptionsResponse>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Media::GetOSDOptionsResponse& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Media::GetOSDOptionsResponse& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"OSDOptions"s};
		bool ret = false;
		ONVIF::OSDConfigurationOptions gen_oSDOptions;
		ret = TypeDeserializer<ONVIF::OSDConfigurationOptions>::deserialize(REMOTING__NAMES[0], true, deser, gen_oSDOptions);
		if (ret) value.setOSDOptions(gen_oSDOptions);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Media_GetOSDOptionsResponse_INCLUDED

