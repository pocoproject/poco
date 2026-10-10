//
// ImagingSettingsExtension20Deserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_ImagingSettingsExtension20_INCLUDED
#define TypeDeserializer_ONVIF_ImagingSettingsExtension20_INCLUDED


#include "ONVIF/ImageStabilizationDeserializer.h"
#include "ONVIF/ImageStabilizationSerializer.h"
#include "ONVIF/ImagingSettingsExtension20.h"
#include "ONVIF/ImagingSettingsExtension202Deserializer.h"
#include "ONVIF/ImagingSettingsExtension202Serializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::ImagingSettingsExtension20>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::ImagingSettingsExtension20& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::ImagingSettingsExtension20& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"ImageStabilization"s,"Extension"s};
		bool ret = false;
		Poco::SharedPtr<ONVIF::ImageStabilization> gen_imageStabilization;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::ImageStabilization>>::deserialize(REMOTING__NAMES[0], false, deser, gen_imageStabilization);
		if (ret) value.setImageStabilization(gen_imageStabilization);
		Poco::SharedPtr<ONVIF::ImagingSettingsExtension202> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::ImagingSettingsExtension202>>::deserialize(REMOTING__NAMES[1], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_ImagingSettingsExtension20_INCLUDED

