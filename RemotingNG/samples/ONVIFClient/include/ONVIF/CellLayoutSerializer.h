//
// CellLayoutSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_CellLayout_INCLUDED
#define TypeSerializer_ONVIF_CellLayout_INCLUDED


#include "ONVIF/CellLayout.h"
#include "ONVIF/TransformationDeserializer.h"
#include "ONVIF/TransformationSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::CellLayout>
{
public:
	static void prepareSerializeAttributes(Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Columns"s,"Rows"s,"http://www.onvif.org/ver10/schema"s};
		ser.pushAttribute(REMOTING__NAMES[2], REMOTING__NAMES[0]);
		ser.pushAttribute(REMOTING__NAMES[2], REMOTING__NAMES[1]);
	}

	static void serialize(const std::string& name, const ONVIF::CellLayout& value, Serializer& ser)
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

	static void serializeAttributes(const ONVIF::CellLayout& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Columns"s,"Rows"s};
		TypeSerializer<int>::serialize(REMOTING__NAMES[0], value.getColumns(), ser);
		TypeSerializer<int>::serialize(REMOTING__NAMES[1], value.getRows(), ser);
	}

	static void serializeImpl(const ONVIF::CellLayout& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Transformation"s,""s};
		TypeSerializer<ONVIF::Transformation>::serialize(REMOTING__NAMES[0], value.getTransformation(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_CellLayout_INCLUDED

