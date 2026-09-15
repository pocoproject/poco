//
// GetVideoOutputsResponseDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_DeviceIO_GetVideoOutputsResponse_INCLUDED
#define TypeDeserializer_ONVIF_DeviceIO_GetVideoOutputsResponse_INCLUDED


#include "ONVIF/DeviceIO/GetVideoOutputsResponse.h"
#include "ONVIF/VideoOutputDeserializer.h"
#include "ONVIF/VideoOutputSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::DeviceIO::GetVideoOutputsResponse>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::DeviceIO::GetVideoOutputsResponse& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/deviceIO/wsdl"s);
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

	static void deserializeImpl(Deserializer& deser, ONVIF::DeviceIO::GetVideoOutputsResponse& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"VideoOutputs"s};
		bool ret = false;
		std::vector<ONVIF::VideoOutput> gen_videoOutputs;
		ret = TypeDeserializer<std::vector<ONVIF::VideoOutput>>::deserialize(REMOTING__NAMES[0], false, deser, gen_videoOutputs);
		if (ret) value.setVideoOutputs(gen_videoOutputs);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_DeviceIO_GetVideoOutputsResponse_INCLUDED

