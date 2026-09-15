//
// IOCapabilitiesExtensionDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_IOCapabilitiesExtension_INCLUDED
#define TypeDeserializer_ONVIF_IOCapabilitiesExtension_INCLUDED


#include "ONVIF/IOCapabilitiesExtension.h"
#include "ONVIF/IOCapabilitiesExtension2Deserializer.h"
#include "ONVIF/IOCapabilitiesExtension2Serializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::IOCapabilitiesExtension>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::IOCapabilitiesExtension& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::IOCapabilitiesExtension& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Auxiliary"s,"AuxiliaryCommands"s,"Extension"s};
		bool ret = false;
		Poco::Optional<bool> gen_auxiliary;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[0], false, deser, gen_auxiliary);
		if (ret) value.setAuxiliary(gen_auxiliary);
		std::vector<std::string> gen_auxiliaryCommands;
		ret = TypeDeserializer<std::vector<std::string>>::deserialize(REMOTING__NAMES[1], false, deser, gen_auxiliaryCommands);
		if (ret) value.setAuxiliaryCommands(gen_auxiliaryCommands);
		ONVIF::IOCapabilitiesExtension2 gen_extension;
		ret = TypeDeserializer<ONVIF::IOCapabilitiesExtension2>::deserialize(REMOTING__NAMES[2], true, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_IOCapabilitiesExtension_INCLUDED

