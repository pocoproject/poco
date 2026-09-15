//
// MaximumNumberOfOSDsSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_MaximumNumberOfOSDs_INCLUDED
#define TypeSerializer_ONVIF_MaximumNumberOfOSDs_INCLUDED


#include "ONVIF/MaximumNumberOfOSDs.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::MaximumNumberOfOSDs>
{
public:
	static void prepareSerializeAttributes(Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Total"s,"Image"s,"PlainText"s,"Date"s,"Time"s,"DateAndTime"s,"http://www.onvif.org/ver10/schema"s};
		ser.pushAttribute(REMOTING__NAMES[6], REMOTING__NAMES[0]);
		ser.pushAttribute(REMOTING__NAMES[6], REMOTING__NAMES[1]);
		ser.pushAttribute(REMOTING__NAMES[6], REMOTING__NAMES[2]);
		ser.pushAttribute(REMOTING__NAMES[6], REMOTING__NAMES[3]);
		ser.pushAttribute(REMOTING__NAMES[6], REMOTING__NAMES[4]);
		ser.pushAttribute(REMOTING__NAMES[6], REMOTING__NAMES[5]);
	}

	static void serialize(const std::string& name, const ONVIF::MaximumNumberOfOSDs& value, Serializer& ser)
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

	static void serializeAttributes(const ONVIF::MaximumNumberOfOSDs& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Total"s,"Image"s,"PlainText"s,"Date"s,"Time"s,"DateAndTime"s};
		TypeSerializer<int>::serialize(REMOTING__NAMES[0], value.getTotal(), ser);
		TypeSerializer<Poco::Optional<int>>::serialize(REMOTING__NAMES[1], value.getImage(), ser);
		TypeSerializer<Poco::Optional<int>>::serialize(REMOTING__NAMES[2], value.getPlainText(), ser);
		TypeSerializer<Poco::Optional<int>>::serialize(REMOTING__NAMES[3], value.getDate(), ser);
		TypeSerializer<Poco::Optional<int>>::serialize(REMOTING__NAMES[4], value.getTime(), ser);
		TypeSerializer<Poco::Optional<int>>::serialize(REMOTING__NAMES[5], value.getDateAndTime(), ser);
	}

	static void serializeImpl(const ONVIF::MaximumNumberOfOSDs& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_MaximumNumberOfOSDs_INCLUDED

