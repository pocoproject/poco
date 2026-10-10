//
// ImagingSettingsExtension202Deserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_ImagingSettingsExtension202_INCLUDED
#define TypeDeserializer_ONVIF_ImagingSettingsExtension202_INCLUDED


#include "ONVIF/ImagingSettingsExtension202.h"
#include "ONVIF/ImagingSettingsExtension203Deserializer.h"
#include "ONVIF/ImagingSettingsExtension203Serializer.h"
#include "ONVIF/IrCutFilterAutoAdjustmentDeserializer.h"
#include "ONVIF/IrCutFilterAutoAdjustmentSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::ImagingSettingsExtension202>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::ImagingSettingsExtension202& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::ImagingSettingsExtension202& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"IrCutFilterAutoAdjustment"s,"Extension"s};
		bool ret = false;
		std::vector<ONVIF::IrCutFilterAutoAdjustment> gen_irCutFilterAutoAdjustment;
		ret = TypeDeserializer<std::vector<ONVIF::IrCutFilterAutoAdjustment>>::deserialize(REMOTING__NAMES[0], false, deser, gen_irCutFilterAutoAdjustment);
		if (ret) value.setIrCutFilterAutoAdjustment(gen_irCutFilterAutoAdjustment);
		Poco::SharedPtr<ONVIF::ImagingSettingsExtension203> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::ImagingSettingsExtension203>>::deserialize(REMOTING__NAMES[1], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_ImagingSettingsExtension202_INCLUDED

