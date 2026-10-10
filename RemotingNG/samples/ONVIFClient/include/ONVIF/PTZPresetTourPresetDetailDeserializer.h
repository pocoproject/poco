//
// PTZPresetTourPresetDetailDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_PTZPresetTourPresetDetail_INCLUDED
#define TypeDeserializer_ONVIF_PTZPresetTourPresetDetail_INCLUDED


#include "ONVIF/PTZPresetTourPresetDetail.h"
#include "ONVIF/PTZPresetTourTypeExtensionDeserializer.h"
#include "ONVIF/PTZPresetTourTypeExtensionSerializer.h"
#include "ONVIF/PTZVectorDeserializer.h"
#include "ONVIF/PTZVectorSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::PTZPresetTourPresetDetail>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::PTZPresetTourPresetDetail& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::PTZPresetTourPresetDetail& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"PresetToken"s,"Home"s,"PTZPosition"s,"TypeExtension"s};
		bool ret = false;
		Poco::Optional<std::string> gen_presetToken;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[0], false, deser, gen_presetToken);
		if (ret) value.setPresetToken(gen_presetToken);
		Poco::Optional<bool> gen_home;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[1], false, deser, gen_home);
		if (ret) value.setHome(gen_home);
		Poco::SharedPtr<ONVIF::PTZVector> gen_pTZPosition;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::PTZVector>>::deserialize(REMOTING__NAMES[2], false, deser, gen_pTZPosition);
		if (ret) value.setPTZPosition(gen_pTZPosition);
		Poco::SharedPtr<ONVIF::PTZPresetTourTypeExtension> gen_typeExtension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::PTZPresetTourTypeExtension>>::deserialize(REMOTING__NAMES[3], false, deser, gen_typeExtension);
		if (ret) value.setTypeExtension(gen_typeExtension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_PTZPresetTourPresetDetail_INCLUDED

