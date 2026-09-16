//
// Deserializer.cpp
//
// Library: RemotingNG/REST
// Package: REST
// Module:  Deserializer
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RemotingNG/REST/Deserializer.h"
#include "Poco/RemotingNG/RemotingException.h"
#include "Poco/String.h"


namespace Poco {
namespace RemotingNG {
namespace REST {


Deserializer::Deserializer()
{
}


Deserializer::~Deserializer()
{
}


Poco::RemotingNG::SerializerBase::MessageType Deserializer::findMessage(std::string& name)
{
	poco_check_ptr (_pRequest);

	name = _pRequest->getMethod();
	if (name == Poco::Net::HTTPRequest::HTTP_POST)
	{
		name = _pRequest->get("X-HTTP-Method-Override"s, name);
	}

	Poco::toLowerInPlace(name);
	if (name == "delete") name += "_";
	return Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST;
}


void Deserializer::deserializeMessageBegin(const std::string&, SerializerBase::MessageType)
{
}


void Deserializer::deserializeMessageEnd(const std::string& name, SerializerBase::MessageType type)
{
	// consume extra response content
	int ch = _pStream->get();
	while (ch != -1) ch = _pStream->get();

}


bool Deserializer::deserializeStructBegin(const std::string& name, bool isMandatory)
{
	return subDeserializer()->deserializeStructBegin(name, isMandatory);
}


void Deserializer::deserializeStructEnd(const std::string& name)
{
	subDeserializer()->deserializeStructEnd(name);
}


bool Deserializer::deserializeSequenceBegin(const std::string& name, bool isMandatory, Poco::UInt32& sizeHint)
{
	return subDeserializer()->deserializeSequenceBegin(name, isMandatory, sizeHint);
}


void Deserializer::deserializeSequenceEnd(const std::string& name)
{
	subDeserializer()->deserializeSequenceEnd(name);
}


bool Deserializer::deserializeNullableBegin(const std::string& name, bool isMandatory, bool& isNull)
{
	return subDeserializer()->deserializeNullableBegin(name, isMandatory, isNull);
}


void Deserializer::deserializeNullableEnd(const std::string& name)
{
	subDeserializer()->deserializeNullableEnd(name);
}


bool Deserializer::deserializeOptionalBegin(const std::string& name, bool isMandatory, bool& isSpecified)
{
	return subDeserializer()->deserializeOptionalBegin(name, isMandatory, isSpecified);
}


void Deserializer::deserializeOptionalEnd(const std::string& name)
{
	subDeserializer()->deserializeOptionalEnd(name);
}


bool Deserializer::deserialize(const std::string& name, bool isMandatory, Poco::Int8& val)
{
	return subDeserializer()->deserialize(name, isMandatory, val);
}


bool Deserializer::deserialize(const std::string& name, bool isMandatory, Poco::UInt8& val)
{
	return subDeserializer()->deserialize(name, isMandatory, val);
}


bool Deserializer::deserialize(const std::string& name, bool isMandatory, Poco::Int16& val)
{
	return subDeserializer()->deserialize(name, isMandatory, val);
}


bool Deserializer::deserialize(const std::string& name, bool isMandatory, Poco::UInt16& val)
{
	return subDeserializer()->deserialize(name, isMandatory, val);
}


bool Deserializer::deserialize(const std::string& name, bool isMandatory, Poco::Int32& val)
{
	return subDeserializer()->deserialize(name, isMandatory, val);
}


bool Deserializer::deserialize(const std::string& name, bool isMandatory, Poco::UInt32& val)
{
	return subDeserializer()->deserialize(name, isMandatory, val);
}


bool Deserializer::deserialize(const std::string& name, bool isMandatory, long& val)
{
	return subDeserializer()->deserialize(name, isMandatory, val);
}


bool Deserializer::deserialize(const std::string& name, bool isMandatory, unsigned long& val)
{
	return subDeserializer()->deserialize(name, isMandatory, val);
}


#ifndef POCO_INT64_IS_LONG
bool Deserializer::deserialize(const std::string& name, bool isMandatory, Poco::Int64& val)
{
	return subDeserializer()->deserialize(name, isMandatory, val);
}


bool Deserializer::deserialize(const std::string& name, bool isMandatory, Poco::UInt64& val)
{
	return subDeserializer()->deserialize(name, isMandatory, val);
}
#endif


bool Deserializer::deserialize(const std::string& name, bool isMandatory, float& val)
{
	return subDeserializer()->deserialize(name, isMandatory, val);
}


bool Deserializer::deserialize(const std::string& name, bool isMandatory, double& val)
{
	return subDeserializer()->deserialize(name, isMandatory, val);
}


bool Deserializer::deserialize(const std::string& name, bool isMandatory, bool& val)
{
	return subDeserializer()->deserialize(name, isMandatory, val);
}


bool Deserializer::deserialize(const std::string& name, bool isMandatory, char& val)
{
	return subDeserializer()->deserialize(name, isMandatory, val);
}


bool Deserializer::deserialize(const std::string& name, bool isMandatory, std::string& val)
{
	return subDeserializer()->deserialize(name, isMandatory, val);
}


bool Deserializer::deserialize(const std::string& name, bool isMandatory, std::vector<char>& val)
{
	return subDeserializer()->deserialize(name, isMandatory, val);
}


void Deserializer::setupImpl(std::istream& stream)
{
	resetImpl();
	_pStream = &stream;
}


void Deserializer::resetImpl()
{
	_pStream = 0;
	_pRequest = 0;
	_pResponse = 0;
	_pPathDeserializer = 0;
	_pQueryDeserializer = 0;
	_pHeaderDeserializer = 0;
	_pRawDeserializer = 0;
	_pJSONDeserializer = 0;
	_pFormDeserializer = 0;
}


Poco::SharedPtr<Poco::RemotingNG::Deserializer> Deserializer::subDeserializer()
{
	std::string location = getProperty(PROP_LOCATION, Serializer::LOCATION_BODY);
	std::string format = getProperty(PROP_FORMAT, Serializer::FORMAT_JSON);
	if (location == Serializer::LOCATION_PATH)
	{
		if (!_pPathDeserializer)
		{
			if (!_pRequest) throw Poco::RemotingNG::DeserializerException("Location 'path' can be used in request only");
			_pPathDeserializer = new PathDeserializer(getProperty(PROP_PATH, ""), *_pRequest);
		}
		return _pPathDeserializer;
	}
	else if (location == Serializer::LOCATION_QUERY)
	{
		if (!_pQueryDeserializer)
		{
			if (!_pRequest) throw Poco::RemotingNG::SerializerException("Location 'query' can be used in request only");
			_pQueryDeserializer = new FormDeserializer(*_pRequest);
		}
		return _pQueryDeserializer;
	}
	else if (location == Serializer::LOCATION_HEADER)
	{
		if (!_pHeaderDeserializer)
		{
			if (_pRequest)
				_pHeaderDeserializer = new HeaderDeserializer(*_pRequest);
			else if (_pResponse)
				_pHeaderDeserializer = new HeaderDeserializer(*_pResponse);
			else
				throw Poco::IllegalStateException("No request or response object available");
		}
		return _pHeaderDeserializer;
	}
	else if (location == Serializer::LOCATION_BODY)
	{
		if (format == Serializer::FORMAT_RAW || format == Serializer::FORMAT_BINARY)
		{
			if (_pJSONDeserializer) throw Poco::RemotingNG::DeserializerException("Cannot use body formats 'raw' and 'json' simultaneously");
			if (_pFormDeserializer) throw Poco::RemotingNG::DeserializerException("Cannot use locations 'form' and 'body' simultaneously");
			if (!_pRawDeserializer)
			{
				_pRawDeserializer = new RawDeserializer(*_pStream);
			}
			return _pRawDeserializer;
		}
		else if (format == Serializer::FORMAT_JSON)
		{
			if (_pRawDeserializer) throw Poco::RemotingNG::DeserializerException("Cannot use body formats 'raw' and 'json' simultaneously");
			if (_pFormDeserializer) throw Poco::RemotingNG::DeserializerException("Cannot use locations 'form' and 'body' simultaneously");
			if (!_pJSONDeserializer)
			{
				_pJSONDeserializer = new JSONDeserializer(*_pStream);
			}
			return _pJSONDeserializer;
		}
		else throw Poco::RemotingNG::DeserializerException("Unsupported format", format);
	}
	else if (location == Serializer::LOCATION_FORM)
	{
		if (_pRawDeserializer || _pJSONDeserializer) throw Poco::RemotingNG::DeserializerException("Cannot use locations 'form' and 'body' simultaneously");
		if (!_pFormDeserializer)
		{
			if (_pRequest)
				_pFormDeserializer = new FormDeserializer(*_pRequest, *_pStream);
			else if (_pResponse)
				_pFormDeserializer = new FormDeserializer(*_pResponse, *_pStream);
			else
				throw Poco::IllegalStateException("No request or response object available");
		}
		return _pFormDeserializer;
	}
	else throw Poco::RemotingNG::SerializerException("Invalid location", location);
}


} } } // namespace Poco::RemotingNG::REST
