//
// LensDescriptionSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_LensDescription_INCLUDED
#define TypeSerializer_ONVIF_LensDescription_INCLUDED


#include "ONVIF/LensDescription.h"
#include "ONVIF/LensOffsetDeserializer.h"
#include "ONVIF/LensOffsetSerializer.h"
#include "ONVIF/LensProjectionDeserializer.h"
#include "ONVIF/LensProjectionSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::LensDescription>
{
public:
	static void prepareSerializeAttributes(Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"FocalLength"s,"http://www.onvif.org/ver10/schema"s};
		ser.pushAttribute(REMOTING__NAMES[1], REMOTING__NAMES[0]);
	}

	static void serialize(const std::string& name, const ONVIF::LensDescription& value, Serializer& ser)
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

	static void serializeAttributes(const ONVIF::LensDescription& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"FocalLength"s};
		TypeSerializer<Poco::Optional<float>>::serialize(REMOTING__NAMES[0], value.getFocalLength(), ser);
	}

	static void serializeImpl(const ONVIF::LensDescription& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Offset"s,"Projection"s,"XFactor"s,""s};
		TypeSerializer<ONVIF::LensOffset>::serialize(REMOTING__NAMES[0], value.getOffset(), ser);
		TypeSerializer<std::vector<ONVIF::LensProjection>>::serialize(REMOTING__NAMES[1], value.getProjection(), ser);
		TypeSerializer<float>::serialize(REMOTING__NAMES[2], value.getXFactor(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_LensDescription_INCLUDED

