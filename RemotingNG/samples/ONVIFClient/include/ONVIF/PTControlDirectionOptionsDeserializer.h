//
// PTControlDirectionOptionsDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_PTControlDirectionOptions_INCLUDED
#define TypeDeserializer_ONVIF_PTControlDirectionOptions_INCLUDED


#include "ONVIF/EFlipOptionsDeserializer.h"
#include "ONVIF/EFlipOptionsSerializer.h"
#include "ONVIF/PTControlDirectionOptions.h"
#include "ONVIF/PTControlDirectionOptionsExtensionDeserializer.h"
#include "ONVIF/PTControlDirectionOptionsExtensionSerializer.h"
#include "ONVIF/ReverseOptionsDeserializer.h"
#include "ONVIF/ReverseOptionsSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::PTControlDirectionOptions>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::PTControlDirectionOptions& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::PTControlDirectionOptions& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"EFlip"s,"Reverse"s,"Extension"s};
		bool ret = false;
		Poco::SharedPtr<ONVIF::EFlipOptions> gen_eFlip;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::EFlipOptions>>::deserialize(REMOTING__NAMES[0], false, deser, gen_eFlip);
		if (ret) value.setEFlip(gen_eFlip);
		Poco::SharedPtr<ONVIF::ReverseOptions> gen_reverse;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::ReverseOptions>>::deserialize(REMOTING__NAMES[1], false, deser, gen_reverse);
		if (ret) value.setReverse(gen_reverse);
		Poco::SharedPtr<ONVIF::PTControlDirectionOptionsExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::PTControlDirectionOptionsExtension>>::deserialize(REMOTING__NAMES[2], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_PTControlDirectionOptions_INCLUDED

