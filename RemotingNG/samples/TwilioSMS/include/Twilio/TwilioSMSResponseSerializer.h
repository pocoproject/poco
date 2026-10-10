//
// TwilioSMSResponseSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_Twilio_TwilioSMSResponse_INCLUDED
#define TypeSerializer_Twilio_TwilioSMSResponse_INCLUDED


#include "Poco/RemotingNG/TypeSerializer.h"
#include "Twilio/TwilioSMSEndpoint.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<Twilio::TwilioSMSResponse>
{
public:
	static void serialize(const std::string& name, const Twilio::TwilioSMSResponse& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		ser.serializeStructBegin(name);
		serializeImpl(value, ser);
		ser.serializeStructEnd(name);
	}

	static void serializeImpl(const Twilio::TwilioSMSResponse& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"sid"s,"date_created"s,"date_updated"s,"date_sent"s,"account_sid"s,"to"s,"from"s,"body"s,"status"s,"direction"s,"api_version"s,"price"s,"price_unit"s,"uri"s,"num_segments"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.sid, ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[1], value.dateCreated, ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[2], value.dateUpdated, ser);
		TypeSerializer<Poco::Nullable<std::string>>::serialize(REMOTING__NAMES[3], value.dateSent, ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[4], value.accountSID, ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[5], value.to, ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[6], value.from, ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[7], value.body, ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[8], value.status, ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[9], value.direction, ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[10], value.apiVersion, ser);
		TypeSerializer<Poco::Nullable<std::string>>::serialize(REMOTING__NAMES[11], value.price, ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[12], value.price_unit, ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[13], value.uri, ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[14], value.num_segments, ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_Twilio_TwilioSMSResponse_INCLUDED

