//
// GetGuaranteedNumberOfVideoEncoderInstancesResponseDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Media_GetGuaranteedNumberOfVideoEncoderInstancesResponse_INCLUDED
#define TypeDeserializer_ONVIF_Media_GetGuaranteedNumberOfVideoEncoderInstancesResponse_INCLUDED


#include "ONVIF/Media/GetGuaranteedNumberOfVideoEncoderInstancesResponse.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Media::GetGuaranteedNumberOfVideoEncoderInstancesResponse>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Media::GetGuaranteedNumberOfVideoEncoderInstancesResponse& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Media::GetGuaranteedNumberOfVideoEncoderInstancesResponse& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"TotalNumber"s,"JPEG"s,"H264"s,"MPEG4"s};
		bool ret = false;
		int gen_totalNumber;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES[0], true, deser, gen_totalNumber);
		if (ret) value.setTotalNumber(gen_totalNumber);
		Poco::Optional<int> gen_jPEG;
		ret = TypeDeserializer<Poco::Optional<int>>::deserialize(REMOTING__NAMES[1], false, deser, gen_jPEG);
		if (ret) value.setJPEG(gen_jPEG);
		Poco::Optional<int> gen_h264;
		ret = TypeDeserializer<Poco::Optional<int>>::deserialize(REMOTING__NAMES[2], false, deser, gen_h264);
		if (ret) value.setH264(gen_h264);
		Poco::Optional<int> gen_mPEG4;
		ret = TypeDeserializer<Poco::Optional<int>>::deserialize(REMOTING__NAMES[3], false, deser, gen_mPEG4);
		if (ret) value.setMPEG4(gen_mPEG4);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Media_GetGuaranteedNumberOfVideoEncoderInstancesResponse_INCLUDED

