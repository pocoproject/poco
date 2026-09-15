//
// ImagingStatus20Deserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_ImagingStatus20_INCLUDED
#define TypeDeserializer_ONVIF_ImagingStatus20_INCLUDED


#include "ONVIF/FocusStatus20Deserializer.h"
#include "ONVIF/FocusStatus20Serializer.h"
#include "ONVIF/ImagingStatus20.h"
#include "ONVIF/ImagingStatus20ExtensionDeserializer.h"
#include "ONVIF/ImagingStatus20ExtensionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::ImagingStatus20>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::ImagingStatus20& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::ImagingStatus20& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"FocusStatus20"s,"Extension"s};
		bool ret = false;
		Poco::SharedPtr<ONVIF::FocusStatus20> gen_focusStatus20;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::FocusStatus20>>::deserialize(REMOTING__NAMES[0], false, deser, gen_focusStatus20);
		if (ret) value.setFocusStatus20(gen_focusStatus20);
		Poco::SharedPtr<ONVIF::ImagingStatus20Extension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::ImagingStatus20Extension>>::deserialize(REMOTING__NAMES[1], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_ImagingStatus20_INCLUDED

