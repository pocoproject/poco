//
// OSDImgOptionsSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_OSDImgOptions_INCLUDED
#define TypeSerializer_ONVIF_OSDImgOptions_INCLUDED


#include "ONVIF/OSDImgOptions.h"
#include "ONVIF/OSDImgOptionsExtensionDeserializer.h"
#include "ONVIF/OSDImgOptionsExtensionSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::OSDImgOptions>
{
public:
	static void prepareSerializeAttributes(Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"FormatsSupported"s,"MaxSize"s,"MaxWidth"s,"MaxHeight"s,"http://www.onvif.org/ver10/schema"s};
		ser.pushAttribute(REMOTING__NAMES[4], REMOTING__NAMES[0]);
		ser.pushAttribute(REMOTING__NAMES[4], REMOTING__NAMES[1]);
		ser.pushAttribute(REMOTING__NAMES[4], REMOTING__NAMES[2]);
		ser.pushAttribute(REMOTING__NAMES[4], REMOTING__NAMES[3]);
	}

	static void serialize(const std::string& name, const ONVIF::OSDImgOptions& value, Serializer& ser)
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

	static void serializeAttributes(const ONVIF::OSDImgOptions& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"FormatsSupported"s,"MaxSize"s,"MaxWidth"s,"MaxHeight"s};
		TypeSerializer<std::vector<std::vector<std::string>>>::serialize(REMOTING__NAMES[0], value.getFormatsSupported(), ser);
		TypeSerializer<Poco::Optional<int>>::serialize(REMOTING__NAMES[1], value.getMaxSize(), ser);
		TypeSerializer<Poco::Optional<int>>::serialize(REMOTING__NAMES[2], value.getMaxWidth(), ser);
		TypeSerializer<Poco::Optional<int>>::serialize(REMOTING__NAMES[3], value.getMaxHeight(), ser);
	}

	static void serializeImpl(const ONVIF::OSDImgOptions& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"ImagePath"s,"Extension"s,""s};
		TypeSerializer<std::vector<Poco::URI>>::serialize(REMOTING__NAMES[0], value.getImagePath(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::OSDImgOptionsExtension>>::serialize(REMOTING__NAMES[1], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_OSDImgOptions_INCLUDED

