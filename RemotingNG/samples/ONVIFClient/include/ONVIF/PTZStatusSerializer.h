//
// PTZStatusSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_PTZStatus_INCLUDED
#define TypeSerializer_ONVIF_PTZStatus_INCLUDED


#include "ONVIF/PTZMoveStatusDeserializer.h"
#include "ONVIF/PTZMoveStatusSerializer.h"
#include "ONVIF/PTZStatus.h"
#include "ONVIF/PTZVectorDeserializer.h"
#include "ONVIF/PTZVectorSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::PTZStatus>
{
public:
	static void serialize(const std::string& name, const ONVIF::PTZStatus& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeImpl(const ONVIF::PTZStatus& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Position"s,"MoveStatus"s,"Error"s,"UtcTime"s,""s};
		TypeSerializer<Poco::SharedPtr<ONVIF::PTZVector>>::serialize(REMOTING__NAMES[0], value.getPosition(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::PTZMoveStatus>>::serialize(REMOTING__NAMES[1], value.getMoveStatus(), ser);
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[2], value.getError(), ser);
		ser.pushProperty(SerializerBase::PROP_XSDTYPE, "dateTime"s);
		TypeSerializer<Poco::DateTime>::serialize(REMOTING__NAMES[3], value.getUtcTime(), ser);
		ser.popProperty(SerializerBase::PROP_XSDTYPE);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_PTZStatus_INCLUDED

