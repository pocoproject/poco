//
// ImagingOptions20Extension3Deserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_ImagingOptions20Extension3_INCLUDED
#define TypeDeserializer_ONVIF_ImagingOptions20Extension3_INCLUDED


#include "ONVIF/DefoggingOptionsDeserializer.h"
#include "ONVIF/DefoggingOptionsSerializer.h"
#include "ONVIF/ImagingOptions20Extension3.h"
#include "ONVIF/ImagingOptions20Extension4Deserializer.h"
#include "ONVIF/ImagingOptions20Extension4Serializer.h"
#include "ONVIF/NoiseReductionOptionsDeserializer.h"
#include "ONVIF/NoiseReductionOptionsSerializer.h"
#include "ONVIF/ToneCompensationOptionsDeserializer.h"
#include "ONVIF/ToneCompensationOptionsSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::ImagingOptions20Extension3>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::ImagingOptions20Extension3& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::ImagingOptions20Extension3& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"ToneCompensationOptions"s,"DefoggingOptions"s,"NoiseReductionOptions"s,"Extension"s};
		bool ret = false;
		Poco::SharedPtr<ONVIF::ToneCompensationOptions> gen_toneCompensationOptions;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::ToneCompensationOptions>>::deserialize(REMOTING__NAMES[0], false, deser, gen_toneCompensationOptions);
		if (ret) value.setToneCompensationOptions(gen_toneCompensationOptions);
		Poco::SharedPtr<ONVIF::DefoggingOptions> gen_defoggingOptions;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::DefoggingOptions>>::deserialize(REMOTING__NAMES[1], false, deser, gen_defoggingOptions);
		if (ret) value.setDefoggingOptions(gen_defoggingOptions);
		Poco::SharedPtr<ONVIF::NoiseReductionOptions> gen_noiseReductionOptions;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::NoiseReductionOptions>>::deserialize(REMOTING__NAMES[2], false, deser, gen_noiseReductionOptions);
		if (ret) value.setNoiseReductionOptions(gen_noiseReductionOptions);
		Poco::SharedPtr<ONVIF::ImagingOptions20Extension4> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::ImagingOptions20Extension4>>::deserialize(REMOTING__NAMES[3], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_ImagingOptions20Extension3_INCLUDED

