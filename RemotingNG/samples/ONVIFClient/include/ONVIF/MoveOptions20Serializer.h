//
// MoveOptions20Serializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_MoveOptions20_INCLUDED
#define TypeSerializer_ONVIF_MoveOptions20_INCLUDED


#include "ONVIF/AbsoluteFocusOptionsDeserializer.h"
#include "ONVIF/AbsoluteFocusOptionsSerializer.h"
#include "ONVIF/ContinuousFocusOptionsDeserializer.h"
#include "ONVIF/ContinuousFocusOptionsSerializer.h"
#include "ONVIF/MoveOptions20.h"
#include "ONVIF/RelativeFocusOptions20Deserializer.h"
#include "ONVIF/RelativeFocusOptions20Serializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::MoveOptions20>
{
public:
	static void serialize(const std::string& name, const ONVIF::MoveOptions20& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::MoveOptions20& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Absolute"s,"Relative"s,"Continuous"s,""s};
		TypeSerializer<Poco::SharedPtr<ONVIF::AbsoluteFocusOptions>>::serialize(REMOTING__NAMES[0], value.getAbsolute(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::RelativeFocusOptions20>>::serialize(REMOTING__NAMES[1], value.getRelative(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::ContinuousFocusOptions>>::serialize(REMOTING__NAMES[2], value.getContinuous(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_MoveOptions20_INCLUDED

