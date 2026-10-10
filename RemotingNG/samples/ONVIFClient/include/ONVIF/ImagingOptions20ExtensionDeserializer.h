//
// ImagingOptions20ExtensionDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_ImagingOptions20Extension_INCLUDED
#define TypeDeserializer_ONVIF_ImagingOptions20Extension_INCLUDED


#include "ONVIF/ImageStabilizationOptionsDeserializer.h"
#include "ONVIF/ImageStabilizationOptionsSerializer.h"
#include "ONVIF/ImagingOptions20Extension.h"
#include "ONVIF/ImagingOptions20Extension2Deserializer.h"
#include "ONVIF/ImagingOptions20Extension2Serializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::ImagingOptions20Extension>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::ImagingOptions20Extension& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::ImagingOptions20Extension& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"ImageStabilization"s,"Extension"s};
		bool ret = false;
		Poco::SharedPtr<ONVIF::ImageStabilizationOptions> gen_imageStabilization;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::ImageStabilizationOptions>>::deserialize(REMOTING__NAMES[0], false, deser, gen_imageStabilization);
		if (ret) value.setImageStabilization(gen_imageStabilization);
		Poco::SharedPtr<ONVIF::ImagingOptions20Extension2> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::ImagingOptions20Extension2>>::deserialize(REMOTING__NAMES[1], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_ImagingOptions20Extension_INCLUDED

