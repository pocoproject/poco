//
// PTControlDirectionDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_PTControlDirection_INCLUDED
#define TypeDeserializer_ONVIF_PTControlDirection_INCLUDED


#include "ONVIF/EFlipDeserializer.h"
#include "ONVIF/EFlipSerializer.h"
#include "ONVIF/PTControlDirection.h"
#include "ONVIF/PTControlDirectionExtensionDeserializer.h"
#include "ONVIF/PTControlDirectionExtensionSerializer.h"
#include "ONVIF/ReverseDeserializer.h"
#include "ONVIF/ReverseSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::PTControlDirection>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::PTControlDirection& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::PTControlDirection& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"EFlip"s,"Reverse"s,"Extension"s};
		bool ret = false;
		Poco::SharedPtr<ONVIF::EFlip> gen_eFlip;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::EFlip>>::deserialize(REMOTING__NAMES[0], false, deser, gen_eFlip);
		if (ret) value.setEFlip(gen_eFlip);
		Poco::SharedPtr<ONVIF::Reverse> gen_reverse;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::Reverse>>::deserialize(REMOTING__NAMES[1], false, deser, gen_reverse);
		if (ret) value.setReverse(gen_reverse);
		Poco::SharedPtr<ONVIF::PTControlDirectionExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::PTControlDirectionExtension>>::deserialize(REMOTING__NAMES[2], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_PTControlDirection_INCLUDED

