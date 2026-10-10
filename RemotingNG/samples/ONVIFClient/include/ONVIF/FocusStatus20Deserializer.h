//
// FocusStatus20Deserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_FocusStatus20_INCLUDED
#define TypeDeserializer_ONVIF_FocusStatus20_INCLUDED


#include "ONVIF/FocusStatus20.h"
#include "ONVIF/FocusStatus20ExtensionDeserializer.h"
#include "ONVIF/FocusStatus20ExtensionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::FocusStatus20>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::FocusStatus20& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::FocusStatus20& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Position"s,"MoveStatus"s,"Error"s,"Extension"s};
		bool ret = false;
		float gen_position;
		ret = TypeDeserializer<float>::deserialize(REMOTING__NAMES[0], true, deser, gen_position);
		if (ret) value.setPosition(gen_position);
		std::string gen_moveStatus;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[1], true, deser, gen_moveStatus);
		if (ret) value.setMoveStatus(gen_moveStatus);
		Poco::Optional<std::string> gen_error;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[2], false, deser, gen_error);
		if (ret) value.setError(gen_error);
		Poco::SharedPtr<ONVIF::FocusStatus20Extension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::FocusStatus20Extension>>::deserialize(REMOTING__NAMES[3], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_FocusStatus20_INCLUDED

