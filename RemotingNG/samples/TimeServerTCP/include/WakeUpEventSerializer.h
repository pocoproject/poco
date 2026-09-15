//
// WakeUpEventSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2006-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef TypeSerializer_Services_WakeUpEvent_INCLUDED
#define TypeSerializer_Services_WakeUpEvent_INCLUDED


#include "Poco/RemotingNG/TypeSerializer.h"
#include "TimeService.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<Services::WakeUpEvent>
{
public:
	static void serialize(const std::string& name, const Services::WakeUpEvent& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		ser.serializeStructBegin(name);
		serializeImpl(value, ser);
		ser.serializeStructEnd(name);
	}

	static void serializeImpl(const Services::WakeUpEvent& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"message"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.message, ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_Services_WakeUpEvent_INCLUDED

