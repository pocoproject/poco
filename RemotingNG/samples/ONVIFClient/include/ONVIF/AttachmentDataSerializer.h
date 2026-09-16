//
// AttachmentDataSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_AttachmentData_INCLUDED
#define TypeSerializer_ONVIF_AttachmentData_INCLUDED


#include "ONVIF/AttachmentData.h"
#include "Poco/RemotingNG/TypeSerializer.h"
#include "W3/XOP/IncludeDeserializer.h"
#include "W3/XOP/IncludeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::AttachmentData>
{
public:
	static void prepareSerializeAttributes(Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"contentType"s,"http://www.onvif.org/ver10/schema"s,"http://www.w3.org/2005/05/xmlmime"s};
		ser.pushAttribute(REMOTING__NAMES[2], REMOTING__NAMES[0]);
	}

	static void serialize(const std::string& name, const ONVIF::AttachmentData& value, Serializer& ser)
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

	static void serializeAttributes(const ONVIF::AttachmentData& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"contentType"s};
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[0], value.getContentType(), ser);
	}

	static void serializeImpl(const ONVIF::AttachmentData& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Include"s,""s,"http://www.w3.org/2004/08/xop/include"s};
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMES[2]);
		TypeSerializer<W3::XOP::Include>::serialize(REMOTING__NAMES[0], value.getInclude(), ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_AttachmentData_INCLUDED

