//
// GetGuaranteedNumberOfVideoEncoderInstancesResponseSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_Media_GetGuaranteedNumberOfVideoEncoderInstancesResponse_INCLUDED
#define TypeSerializer_ONVIF_Media_GetGuaranteedNumberOfVideoEncoderInstancesResponse_INCLUDED


#include "ONVIF/Media/GetGuaranteedNumberOfVideoEncoderInstancesResponse.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::Media::GetGuaranteedNumberOfVideoEncoderInstancesResponse>
{
public:
	static void serialize(const std::string& name, const ONVIF::Media::GetGuaranteedNumberOfVideoEncoderInstancesResponse& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/media/wsdl"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeImpl(const ONVIF::Media::GetGuaranteedNumberOfVideoEncoderInstancesResponse& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"TotalNumber"s,"JPEG"s,"H264"s,"MPEG4"s,""s};
		TypeSerializer<int>::serialize(REMOTING__NAMES[0], value.getTotalNumber(), ser);
		TypeSerializer<Poco::Optional<int>>::serialize(REMOTING__NAMES[1], value.getJPEG(), ser);
		TypeSerializer<Poco::Optional<int>>::serialize(REMOTING__NAMES[2], value.getH264(), ser);
		TypeSerializer<Poco::Optional<int>>::serialize(REMOTING__NAMES[3], value.getMPEG4(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_Media_GetGuaranteedNumberOfVideoEncoderInstancesResponse_INCLUDED

