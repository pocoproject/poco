//
// PTZPositionFilterSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_PTZPositionFilter_INCLUDED
#define TypeSerializer_ONVIF_PTZPositionFilter_INCLUDED


#include "ONVIF/PTZPositionFilter.h"
#include "ONVIF/PTZVectorDeserializer.h"
#include "ONVIF/PTZVectorSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::PTZPositionFilter>
{
public:
	static void serialize(const std::string& name, const ONVIF::PTZPositionFilter& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::PTZPositionFilter& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"MinPosition"s,"MaxPosition"s,"EnterOrExit"s,""s};
		TypeSerializer<ONVIF::PTZVector>::serialize(REMOTING__NAMES[0], value.getMinPosition(), ser);
		TypeSerializer<ONVIF::PTZVector>::serialize(REMOTING__NAMES[1], value.getMaxPosition(), ser);
		TypeSerializer<bool>::serialize(REMOTING__NAMES[2], value.getEnterOrExit(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_PTZPositionFilter_INCLUDED

