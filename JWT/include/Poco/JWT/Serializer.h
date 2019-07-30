//
// Serializer.h
//
// Library: JWT
// Package: JWT
// Module:  Serializer
//
// Definition of the Serializer class.
//
// Copyright (c) 2019, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef JWT_Serializer_INCLUDED
#define JWT_Serializer_INCLUDED


#include "Poco/JWT/JWT.h"
#include "Poco/JSON/Object.h"


namespace Poco {
namespace JWT {


class JWT_API Serializer
	/// A helper class for serializing and deserializing JWTs.
{
public:
	static std::string serialize(const Poco::JSON::Object& object);
		/// Serializes and base64-encodes a JSON object.

	static void serialize(const Poco::JSON::Object& object, std::ostream& stream);
		/// Serializes and base64-encodes a JSON object.

	static Poco::JSON::Object::Ptr deserialize(const std::string& serialized);
		/// Attempts to deserialize a base64-encoded serialized JSON object.

	static Poco::JSON::Object::Ptr deserialize(std::istream& stream);
		/// Attempts to deserialize a base64-encoded serialized JSON object.

	static std::vector<std::string> split(const std::string& token);
		/// Splits a serialized JWT into its components.
};


} } // namespace Poco::JWT


#endif // JWT_Serializer_INCLUDED
