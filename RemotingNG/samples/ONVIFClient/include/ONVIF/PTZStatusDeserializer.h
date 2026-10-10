//
// PTZStatusDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_PTZStatus_INCLUDED
#define TypeDeserializer_ONVIF_PTZStatus_INCLUDED


#include "ONVIF/PTZMoveStatusDeserializer.h"
#include "ONVIF/PTZMoveStatusSerializer.h"
#include "ONVIF/PTZStatus.h"
#include "ONVIF/PTZVectorDeserializer.h"
#include "ONVIF/PTZVectorSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::PTZStatus>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::PTZStatus& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::PTZStatus& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Position"s,"MoveStatus"s,"Error"s,"UtcTime"s};
		bool ret = false;
		Poco::SharedPtr<ONVIF::PTZVector> gen_position;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::PTZVector>>::deserialize(REMOTING__NAMES[0], false, deser, gen_position);
		if (ret) value.setPosition(gen_position);
		Poco::SharedPtr<ONVIF::PTZMoveStatus> gen_moveStatus;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::PTZMoveStatus>>::deserialize(REMOTING__NAMES[1], false, deser, gen_moveStatus);
		if (ret) value.setMoveStatus(gen_moveStatus);
		Poco::Optional<std::string> gen_error;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[2], false, deser, gen_error);
		if (ret) value.setError(gen_error);
		deser.pushProperty(SerializerBase::PROP_XSDTYPE, "dateTime"s);
		Poco::DateTime gen_utcTime;
		ret = TypeDeserializer<Poco::DateTime>::deserialize(REMOTING__NAMES[3], true, deser, gen_utcTime);
		if (ret) value.setUtcTime(gen_utcTime);
		deser.popProperty(SerializerBase::PROP_XSDTYPE);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_PTZStatus_INCLUDED

