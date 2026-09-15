//
// SetOSDDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Media_SetOSD_INCLUDED
#define TypeDeserializer_ONVIF_Media_SetOSD_INCLUDED


#include "ONVIF/Media/SetOSD.h"
#include "ONVIF/OSDConfigurationDeserializer.h"
#include "ONVIF/OSDConfigurationSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Media::SetOSD>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Media::SetOSD& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Media::SetOSD& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"OSD"s};
		bool ret = false;
		ONVIF::OSDConfiguration gen_oSD;
		ret = TypeDeserializer<ONVIF::OSDConfiguration>::deserialize(REMOTING__NAMES[0], true, deser, gen_oSD);
		if (ret) value.setOSD(gen_oSD);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Media_SetOSD_INCLUDED

