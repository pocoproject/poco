//
// OptionalConsumer.cpp
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RemotingNG/Serializer.h"
#include "Poco/RemotingNG/Deserializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"
#include <optional>
#include <string>


// Compiles only if this file sees the std::optional specializations of
// TypeSerializer and TypeDeserializer, which need C++17.
void serializeOptional(const std::optional<std::string>& value, Poco::RemotingNG::Serializer& ser)
{
	Poco::RemotingNG::TypeSerializer<std::optional<std::string>>::serialize("value", value, ser);
}


bool deserializeOptional(Poco::RemotingNG::Deserializer& deser, std::optional<std::string>& value)
{
	return Poco::RemotingNG::TypeDeserializer<std::optional<std::string>>::deserialize("value", false, deser, value);
}
