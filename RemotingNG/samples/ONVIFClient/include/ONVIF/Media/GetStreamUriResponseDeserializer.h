//
// GetStreamUriResponseDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Media_GetStreamUriResponse_INCLUDED
#define TypeDeserializer_ONVIF_Media_GetStreamUriResponse_INCLUDED


#include "ONVIF/Media/GetStreamUriResponse.h"
#include "ONVIF/MediaUriDeserializer.h"
#include "ONVIF/MediaUriSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Media::GetStreamUriResponse>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Media::GetStreamUriResponse& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Media::GetStreamUriResponse& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"MediaUri"s};
		bool ret = false;
		ONVIF::MediaUri gen_mediaUri;
		ret = TypeDeserializer<ONVIF::MediaUri>::deserialize(REMOTING__NAMES[0], true, deser, gen_mediaUri);
		if (ret) value.setMediaUri(gen_mediaUri);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Media_GetStreamUriResponse_INCLUDED

