//
// G711DecOptionsSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_G711DecOptions_INCLUDED
#define TypeSerializer_ONVIF_G711DecOptions_INCLUDED


#include "ONVIF/G711DecOptions.h"
#include "ONVIF/IntItemsDeserializer.h"
#include "ONVIF/IntItemsSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::G711DecOptions>
{
public:
	static void serialize(const std::string& name, const ONVIF::G711DecOptions& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::G711DecOptions& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Bitrate"s,"SampleRateRange"s,""s};
		TypeSerializer<ONVIF::IntItems>::serialize(REMOTING__NAMES[0], value.getBitrate(), ser);
		TypeSerializer<ONVIF::IntItems>::serialize(REMOTING__NAMES[1], value.getSampleRateRange(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_G711DecOptions_INCLUDED

