//
// FocusMoveSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_FocusMove_INCLUDED
#define TypeSerializer_ONVIF_FocusMove_INCLUDED


#include "ONVIF/AbsoluteFocusDeserializer.h"
#include "ONVIF/AbsoluteFocusSerializer.h"
#include "ONVIF/ContinuousFocusDeserializer.h"
#include "ONVIF/ContinuousFocusSerializer.h"
#include "ONVIF/FocusMove.h"
#include "ONVIF/RelativeFocusDeserializer.h"
#include "ONVIF/RelativeFocusSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::FocusMove>
{
public:
	static void serialize(const std::string& name, const ONVIF::FocusMove& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::FocusMove& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Absolute"s,"Relative"s,"Continuous"s,""s};
		TypeSerializer<Poco::SharedPtr<ONVIF::AbsoluteFocus>>::serialize(REMOTING__NAMES[0], value.getAbsolute(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::RelativeFocus>>::serialize(REMOTING__NAMES[1], value.getRelative(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::ContinuousFocus>>::serialize(REMOTING__NAMES[2], value.getContinuous(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_FocusMove_INCLUDED

