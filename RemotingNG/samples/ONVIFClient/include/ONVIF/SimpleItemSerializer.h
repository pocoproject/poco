//
// SimpleItemSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_SimpleItem_INCLUDED
#define TypeSerializer_ONVIF_SimpleItem_INCLUDED


#include "ONVIF/SimpleItem.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::SimpleItem>
{
public:
	static void prepareSerializeAttributes(Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Name"s,"Value"s,"http://www.onvif.org/ver10/schema"s};
		ser.pushAttribute(REMOTING__NAMES[2], REMOTING__NAMES[0]);
		ser.pushAttribute(REMOTING__NAMES[2], REMOTING__NAMES[1]);
	}

	static void serialize(const std::string& name, const ONVIF::SimpleItem& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		prepareSerializeAttributes(ser);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeAttributes(value, ser);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeAttributes(const ONVIF::SimpleItem& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Name"s,"Value"s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getName(), ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[1], value.getValue(), ser);
	}

	static void serializeImpl(const ONVIF::SimpleItem& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_SimpleItem_INCLUDED

