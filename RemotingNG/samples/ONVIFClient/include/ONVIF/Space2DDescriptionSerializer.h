//
// Space2DDescriptionSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_Space2DDescription_INCLUDED
#define TypeSerializer_ONVIF_Space2DDescription_INCLUDED


#include "ONVIF/FloatRangeDeserializer.h"
#include "ONVIF/FloatRangeSerializer.h"
#include "ONVIF/Space2DDescription.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::Space2DDescription>
{
public:
	static void serialize(const std::string& name, const ONVIF::Space2DDescription& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::Space2DDescription& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"URI"s,"XRange"s,"YRange"s,""s};
		TypeSerializer<Poco::URI>::serialize(REMOTING__NAMES[0], value.getURI(), ser);
		TypeSerializer<ONVIF::FloatRange>::serialize(REMOTING__NAMES[1], value.getXRange(), ser);
		TypeSerializer<ONVIF::FloatRange>::serialize(REMOTING__NAMES[2], value.getYRange(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_Space2DDescription_INCLUDED

