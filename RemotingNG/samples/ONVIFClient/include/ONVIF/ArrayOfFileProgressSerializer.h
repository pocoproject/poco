//
// ArrayOfFileProgressSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_ArrayOfFileProgress_INCLUDED
#define TypeSerializer_ONVIF_ArrayOfFileProgress_INCLUDED


#include "ONVIF/ArrayOfFileProgress.h"
#include "ONVIF/ArrayOfFileProgressExtensionDeserializer.h"
#include "ONVIF/ArrayOfFileProgressExtensionSerializer.h"
#include "ONVIF/FileProgressDeserializer.h"
#include "ONVIF/FileProgressSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::ArrayOfFileProgress>
{
public:
	static void serialize(const std::string& name, const ONVIF::ArrayOfFileProgress& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::ArrayOfFileProgress& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"FileProgress"s,"Extension"s,""s};
		TypeSerializer<std::vector<ONVIF::FileProgress>>::serialize(REMOTING__NAMES[0], value.getFileProgress(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::ArrayOfFileProgressExtension>>::serialize(REMOTING__NAMES[1], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_ArrayOfFileProgress_INCLUDED

