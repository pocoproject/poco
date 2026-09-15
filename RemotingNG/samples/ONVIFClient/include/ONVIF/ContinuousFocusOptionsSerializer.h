//
// ContinuousFocusOptionsSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_ContinuousFocusOptions_INCLUDED
#define TypeSerializer_ONVIF_ContinuousFocusOptions_INCLUDED


#include "ONVIF/ContinuousFocusOptions.h"
#include "ONVIF/FloatRangeDeserializer.h"
#include "ONVIF/FloatRangeSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::ContinuousFocusOptions>
{
public:
	static void serialize(const std::string& name, const ONVIF::ContinuousFocusOptions& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::ContinuousFocusOptions& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Speed"s,""s};
		TypeSerializer<ONVIF::FloatRange>::serialize(REMOTING__NAMES[0], value.getSpeed(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_ContinuousFocusOptions_INCLUDED

