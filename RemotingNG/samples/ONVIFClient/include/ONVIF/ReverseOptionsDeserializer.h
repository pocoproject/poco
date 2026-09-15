//
// ReverseOptionsDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_ReverseOptions_INCLUDED
#define TypeDeserializer_ONVIF_ReverseOptions_INCLUDED


#include "ONVIF/ReverseOptions.h"
#include "ONVIF/ReverseOptionsExtensionDeserializer.h"
#include "ONVIF/ReverseOptionsExtensionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::ReverseOptions>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::ReverseOptions& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::ReverseOptions& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Mode"s,"Extension"s};
		bool ret = false;
		std::vector<std::string> gen_mode;
		ret = TypeDeserializer<std::vector<std::string>>::deserialize(REMOTING__NAMES[0], false, deser, gen_mode);
		if (ret) value.setMode(gen_mode);
		Poco::SharedPtr<ONVIF::ReverseOptionsExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::ReverseOptionsExtension>>::deserialize(REMOTING__NAMES[1], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_ReverseOptions_INCLUDED

