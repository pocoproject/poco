//
// ImagingSettingsExtension203Deserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_ImagingSettingsExtension203_INCLUDED
#define TypeDeserializer_ONVIF_ImagingSettingsExtension203_INCLUDED


#include "ONVIF/DefoggingDeserializer.h"
#include "ONVIF/DefoggingSerializer.h"
#include "ONVIF/ImagingSettingsExtension203.h"
#include "ONVIF/ImagingSettingsExtension204Deserializer.h"
#include "ONVIF/ImagingSettingsExtension204Serializer.h"
#include "ONVIF/NoiseReductionDeserializer.h"
#include "ONVIF/NoiseReductionSerializer.h"
#include "ONVIF/ToneCompensationDeserializer.h"
#include "ONVIF/ToneCompensationSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::ImagingSettingsExtension203>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::ImagingSettingsExtension203& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::ImagingSettingsExtension203& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"ToneCompensation"s,"Defogging"s,"NoiseReduction"s,"Extension"s};
		bool ret = false;
		Poco::SharedPtr<ONVIF::ToneCompensation> gen_toneCompensation;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::ToneCompensation>>::deserialize(REMOTING__NAMES[0], false, deser, gen_toneCompensation);
		if (ret) value.setToneCompensation(gen_toneCompensation);
		Poco::SharedPtr<ONVIF::Defogging> gen_defogging;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::Defogging>>::deserialize(REMOTING__NAMES[1], false, deser, gen_defogging);
		if (ret) value.setDefogging(gen_defogging);
		Poco::SharedPtr<ONVIF::NoiseReduction> gen_noiseReduction;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::NoiseReduction>>::deserialize(REMOTING__NAMES[2], false, deser, gen_noiseReduction);
		if (ret) value.setNoiseReduction(gen_noiseReduction);
		Poco::SharedPtr<ONVIF::ImagingSettingsExtension204> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::ImagingSettingsExtension204>>::deserialize(REMOTING__NAMES[3], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_ImagingSettingsExtension203_INCLUDED

