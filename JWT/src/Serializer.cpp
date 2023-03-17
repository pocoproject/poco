//
// Serializer.cpp
//
// Library: JWT
// Package: JWT
// Module:  Serializer
//
// Copyright (c) 2019, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/JWT/Serializer.h"
#include "Poco/JWT/JWTException.h"
#include "Poco/JSON/Parser.h"
#include "Poco/Base64Encoder.h"
#include "Poco/Base64Decoder.h"
#include "Poco/MemoryStream.h"
#include "Poco/StringTokenizer.h"
#include <sstream>


namespace Poco {
namespace JWT {


std::string Serializer::serialize(const Poco::JSON::Object& object)
{
	std::ostringstream stream;
	serialize(object, stream);
	return stream.str();
}


void Serializer::serialize(const Poco::JSON::Object& object, std::ostream& stream)
{
	Poco::Base64Encoder encoder(stream, Poco::BASE64_URL_ENCODING | Poco::BASE64_NO_PADDING);
	object.stringify(encoder);
	encoder.close();
}


Poco::JSON::Object::Ptr Serializer::deserialize(const std::string& serialized)
{
	Poco::MemoryInputStream stream(serialized.data(), serialized.size());
	return deserialize(stream);
}


Poco::JSON::Object::Ptr Serializer::deserialize(std::istream& stream)
{
	Poco::Base64Decoder decoder(stream, Poco::BASE64_URL_ENCODING | Poco::BASE64_NO_PADDING);
	Poco::JSON::Parser parser;
	try
	{
		Poco::Dynamic::Var json = parser.parse(decoder);
		Poco::JSON::Object::Ptr pObject = json.extract<Poco::JSON::Object::Ptr>();
		return pObject;
	}
	catch (Poco::BadCastException&)
	{
		throw ParseException("String does not deserialize to a JSON object");
	}
	catch (Poco::Exception& exc)
	{
		throw ParseException("Failed to deserialize JWT component", exc.displayText());
	}
}


std::vector<std::string> Serializer::split(const std::string& token)
{
	Poco::StringTokenizer tokenizer(token, ".");
	std::vector<std::string> result(tokenizer.begin(), tokenizer.end());
	return result;
}


} } // namespace Poco::JWT
