//
// WakeUpEventDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2006-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef TypeDeserializer_Services_WakeUpEvent_INCLUDED
#define TypeDeserializer_Services_WakeUpEvent_INCLUDED


#include "Poco/RemotingNG/TypeDeserializer.h"
#include "TimeService.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<Services::WakeUpEvent>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, Services::WakeUpEvent& value)
	{
		using namespace std::string_literals;
		
		bool ret = deser.deserializeStructBegin(name, isMandatory);
		if (ret)
		{
			deserializeImpl(deser, value);
			deser.deserializeStructEnd(name);
		}
		return ret;
	}

	static void deserializeImpl(Deserializer& deser, Services::WakeUpEvent& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"message"s};
		TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, value.message);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_Services_WakeUpEvent_INCLUDED

