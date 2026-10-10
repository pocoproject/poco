//
// WhiteBalanceOptions20Serializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_WhiteBalanceOptions20_INCLUDED
#define TypeSerializer_ONVIF_WhiteBalanceOptions20_INCLUDED


#include "ONVIF/FloatRangeDeserializer.h"
#include "ONVIF/FloatRangeSerializer.h"
#include "ONVIF/WhiteBalanceOptions20.h"
#include "ONVIF/WhiteBalanceOptions20ExtensionDeserializer.h"
#include "ONVIF/WhiteBalanceOptions20ExtensionSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::WhiteBalanceOptions20>
{
public:
	static void serialize(const std::string& name, const ONVIF::WhiteBalanceOptions20& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::WhiteBalanceOptions20& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Mode"s,"YrGain"s,"YbGain"s,"Extension"s,""s};
		TypeSerializer<std::vector<std::string>>::serialize(REMOTING__NAMES[0], value.getMode(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::FloatRange>>::serialize(REMOTING__NAMES[1], value.getYrGain(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::FloatRange>>::serialize(REMOTING__NAMES[2], value.getYbGain(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::WhiteBalanceOptions20Extension>>::serialize(REMOTING__NAMES[3], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_WhiteBalanceOptions20_INCLUDED

