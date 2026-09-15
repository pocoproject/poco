//
// EventSubscriptionSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_EventSubscription_INCLUDED
#define TypeSerializer_ONVIF_EventSubscription_INCLUDED


#include "OASIS/WSN/B2/FilterDeserializer.h"
#include "OASIS/WSN/B2/FilterSerializer.h"
#include "ONVIF/EventSubscription.h"
#include "ONVIF/SubscriptionPolicyDeserializer.h"
#include "ONVIF/SubscriptionPolicySerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::EventSubscription>
{
public:
	static void serialize(const std::string& name, const ONVIF::EventSubscription& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::EventSubscription& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Filter"s,"SubscriptionPolicy"s,""s};
		TypeSerializer<Poco::SharedPtr<OASIS::WSN::B2::Filter>>::serialize(REMOTING__NAMES[0], value.getFilter(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::SubscriptionPolicy>>::serialize(REMOTING__NAMES[1], value.getSubscriptionPolicy(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_EventSubscription_INCLUDED

