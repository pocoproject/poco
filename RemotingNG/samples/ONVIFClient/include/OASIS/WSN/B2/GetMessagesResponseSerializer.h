//
// GetMessagesResponseSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_OASIS_WSN_B2_GetMessagesResponse_INCLUDED
#define TypeSerializer_OASIS_WSN_B2_GetMessagesResponse_INCLUDED


#include "OASIS/WSN/B2/GetMessagesResponse.h"
#include "OASIS/WSN/B2/NotificationMessageHolderDeserializer.h"
#include "OASIS/WSN/B2/NotificationMessageHolderSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<OASIS::WSN::B2::GetMessagesResponse>
{
public:
	static void serialize(const std::string& name, const OASIS::WSN::B2::GetMessagesResponse& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://docs.oasis-open.org/wsn/b-2"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeImpl(const OASIS::WSN::B2::GetMessagesResponse& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"NotificationMessage"s,""s};
		TypeSerializer<std::vector<OASIS::WSN::B2::NotificationMessageHolder>>::serialize(REMOTING__NAMES[0], value.getNotificationMessage(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_OASIS_WSN_B2_GetMessagesResponse_INCLUDED

