//
// ImagingOptions20Extension2Deserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_ImagingOptions20Extension2_INCLUDED
#define TypeDeserializer_ONVIF_ImagingOptions20Extension2_INCLUDED


#include "ONVIF/ImagingOptions20Extension2.h"
#include "ONVIF/ImagingOptions20Extension3Deserializer.h"
#include "ONVIF/ImagingOptions20Extension3Serializer.h"
#include "ONVIF/IrCutFilterAutoAdjustmentOptionsDeserializer.h"
#include "ONVIF/IrCutFilterAutoAdjustmentOptionsSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::ImagingOptions20Extension2>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::ImagingOptions20Extension2& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::ImagingOptions20Extension2& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"IrCutFilterAutoAdjustment"s,"Extension"s};
		bool ret = false;
		Poco::SharedPtr<ONVIF::IrCutFilterAutoAdjustmentOptions> gen_irCutFilterAutoAdjustment;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::IrCutFilterAutoAdjustmentOptions>>::deserialize(REMOTING__NAMES[0], false, deser, gen_irCutFilterAutoAdjustment);
		if (ret) value.setIrCutFilterAutoAdjustment(gen_irCutFilterAutoAdjustment);
		Poco::SharedPtr<ONVIF::ImagingOptions20Extension3> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::ImagingOptions20Extension3>>::deserialize(REMOTING__NAMES[1], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_ImagingOptions20Extension2_INCLUDED

