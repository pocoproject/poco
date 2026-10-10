//
// Serializer.cpp
//
// Library: RemotingNG/REST
// Package: REST
// Module:  Serializer
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RemotingNG/REST/Serializer.h"
#include "Poco/RemotingNG/RemotingException.h"
#include "Poco/Net/NetException.h"
#include "Poco/Base64Encoder.h"


using namespace std::string_literals;


namespace Poco {
namespace RemotingNG {
namespace REST {


const std::string Serializer::LOCATION_PATH("path");
const std::string Serializer::LOCATION_QUERY("query");
const std::string Serializer::LOCATION_HEADER("header");
const std::string Serializer::LOCATION_BODY("body");
const std::string Serializer::LOCATION_FORM("form");
const std::string Serializer::FORMAT_RAW("raw");
const std::string Serializer::FORMAT_BINARY("binary");
const std::string Serializer::FORMAT_JSON("json");


Serializer::Serializer():
	_pStream(0),
	_pRequest(0),
	_pResponse(0)
{
}


Serializer::~Serializer()
{
}


void Serializer::serializeMessageBegin(const std::string&, Poco::RemotingNG::SerializerBase::MessageType type)
{
	poco_assert_dbg(_pStream);

	switch (type)
	{
	case Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST:
	case Poco::RemotingNG::SerializerBase::MESSAGE_REPLY:
		break;

	default:
		poco_bugcheck();
	}
}


void Serializer::serializeMessageEnd(const std::string&, Poco::RemotingNG::SerializerBase::MessageType type)
{
	poco_assert_dbg(_pStream);

	switch (type)
	{
	case Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST:
		if (_pPathSerializer)
			_pPathSerializer->close();
		if (_pQuerySerializer)
			_pQuerySerializer->close();
		// fallthrough

	case Poco::RemotingNG::SerializerBase::MESSAGE_REPLY:
		if (_pFormSerializer)
			_pFormSerializer->close();
		_pStream->flush();
		_pStream = 0;
		break;

	default:
		poco_bugcheck();
	}
}


void Serializer::serializeFaultMessage(const std::string& methodName, const Poco::Exception& e)
{
	poco_assert_dbg(_pStream);
	poco_assert_dbg(_pResponse);

	_pResponse->setContentType("application/json"s);
	try
	{
		e.rethrow();
	}
	catch (Poco::RemotingNG::MethodNotFoundException&)
	{
		_pResponse->setStatusAndReason(Poco::Net::HTTPResponse::HTTP_METHOD_NOT_ALLOWED);
	}
	catch (Poco::Net::HTTPException&)
	{
		_pResponse->setStatusAndReason(static_cast<Poco::Net::HTTPResponse::HTTPStatus>(e.code()));
	}
	catch (Poco::RemotingNG::AuthenticationFailedException&)
	{
		_pResponse->setStatusAndReason(Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED);
	}
	catch (Poco::RemotingNG::NoPermissionException&)
	{
		_pResponse->setStatusAndReason(Poco::Net::HTTPResponse::HTTP_FORBIDDEN);
	}
	catch (Poco::NoPermissionException&)
	{
		_pResponse->setStatusAndReason(Poco::Net::HTTPResponse::HTTP_FORBIDDEN);
	}
	catch (Poco::NotFoundException&)
	{
		_pResponse->setStatusAndReason(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
	}
	catch (Poco::ExistsException&)
	{
		_pResponse->setStatusAndReason(Poco::Net::HTTPResponse::HTTP_CONFLICT);
	}
	catch (Poco::InvalidArgumentException&)
	{
		_pResponse->setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
	}
	catch (Poco::NotImplementedException&)
	{
		_pResponse->setStatusAndReason(Poco::Net::HTTPResponse::HTTP_NOT_IMPLEMENTED);
	}
	catch (Poco::Exception&)
	{
		_pResponse->setStatusAndReason(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
	}
	JSONSerializer serializer(*_pStream);
	serializer.serializeFaultMessage(methodName, e);
	_pStream->flush();
}


void Serializer::serializeStructBegin(const std::string& name)
{
	subSerializer()->serializeStructBegin(name);
}


void Serializer::serializeStructEnd(const std::string& name)
{
	subSerializer()->serializeStructEnd(name);
}


void Serializer::serializeSequenceBegin(const std::string& name, Poco::UInt32 numElems)
{
	subSerializer()->serializeSequenceBegin(name, numElems);
}


void Serializer::serializeSequenceEnd(const std::string& name)
{
	subSerializer()->serializeSequenceEnd(name);
}


void Serializer::serializeNullableBegin(const std::string& name, bool isNull)
{
	subSerializer()->serializeNullableBegin(name, isNull);
}


void Serializer::serializeNullableEnd(const std::string& name)
{
	subSerializer()->serializeNullableEnd(name);
}


void Serializer::serializeOptionalBegin(const std::string& name, bool isSpecified)
{
	subSerializer()->serializeOptionalBegin(name, isSpecified);
}


void Serializer::serializeOptionalEnd(const std::string& name)
{
	subSerializer()->serializeOptionalEnd(name);
}


void Serializer::serialize(const std::string& name, Poco::Int8 val)
{
	subSerializer()->serialize(name, val);
}


void Serializer::serialize(const std::string& name, Poco::UInt8 val)
{
	subSerializer()->serialize(name, val);
}


void Serializer::serialize(const std::string& name, Poco::Int16 val)
{
	subSerializer()->serialize(name, val);
}


void Serializer::serialize(const std::string& name, Poco::UInt16 val)
{
	subSerializer()->serialize(name, val);
}


void Serializer::serialize(const std::string& name, Poco::Int32 val)
{
	subSerializer()->serialize(name, val);
}


void Serializer::serialize(const std::string& name, Poco::UInt32 val)
{
	subSerializer()->serialize(name, val);
}


void Serializer::serialize(const std::string& name, long val)
{
	subSerializer()->serialize(name, val);
}


void Serializer::serialize(const std::string& name, unsigned long val)
{
	subSerializer()->serialize(name, val);
}


#ifndef POCO_INT64_IS_LONG
void Serializer::serialize(const std::string& name, Poco::Int64 val)
{
	subSerializer()->serialize(name, val);
}


void Serializer::serialize(const std::string& name, Poco::UInt64 val)
{
	subSerializer()->serialize(name, val);
}
#endif


void Serializer::serialize(const std::string& name, float val)
{
	subSerializer()->serialize(name, val);
}


void Serializer::serialize(const std::string& name, double val)
{
	subSerializer()->serialize(name, val);
}


void Serializer::serialize(const std::string& name, bool val)
{
	subSerializer()->serialize(name, val);
}


void Serializer::serialize(const std::string& name, char val)
{
	subSerializer()->serialize(name, val);
}


void Serializer::serialize(const std::string& name, const std::vector<char>& val)
{
	subSerializer()->serialize(name, val);
}


void Serializer::serialize(const std::string& name, const std::string& val)
{
	subSerializer()->serialize(name, val);
}


void Serializer::setupImpl(std::ostream& stream)
{
	resetImpl();
	_pStream = &stream;
}


void Serializer::resetImpl()
{
	_pRequest = 0;
	_pResponse = 0;
	_pPathSerializer = 0;
	_pQuerySerializer = 0;
	_pHeaderSerializer = 0;
	_pRawSerializer = 0;
	_pJSONSerializer = 0;
	_pFormSerializer = 0;
}


Poco::SharedPtr<Poco::RemotingNG::Serializer> Serializer::subSerializer()
{
	std::string location = getProperty(PROP_LOCATION, LOCATION_BODY);
	std::string format = getProperty(PROP_FORMAT, FORMAT_JSON);
	if (location == LOCATION_PATH)
	{
		if (!_pPathSerializer)
		{
			if (!_pRequest) throw Poco::RemotingNG::SerializerException("Location 'path' can be used in request only");
			_pPathSerializer = new PathSerializer(getProperty(PROP_PATH, ""s), *_pRequest);
		}
		return _pPathSerializer;
	}
	else if (location == LOCATION_QUERY)
	{
		if (!_pQuerySerializer)
		{
			if (!_pRequest) throw Poco::RemotingNG::SerializerException("Location 'query' can be used in request only");
			_pQuerySerializer = new FormSerializer(*_pRequest);
		}
		return _pQuerySerializer;
	}
	else if (location == LOCATION_HEADER)
	{
		if (!_pHeaderSerializer)
		{
			if (_pRequest)
				_pHeaderSerializer = new HeaderSerializer(*_pRequest);
			else if (_pResponse)
				_pHeaderSerializer = new HeaderSerializer(*_pResponse);
			else
				throw Poco::IllegalStateException("No request or response object available");
		}
		return _pHeaderSerializer;
	}
	else if (location == LOCATION_BODY)
	{
		if (format == FORMAT_RAW || format == FORMAT_BINARY)
		{
			if (_pJSONSerializer) throw Poco::RemotingNG::SerializerException("Cannot use body formats 'raw' and 'json' simultaneously");
			if (_pFormSerializer) throw Poco::RemotingNG::SerializerException("Cannot use locations 'form' and 'body' simultaneously");
			if (!_pRawSerializer)
			{
				_pRawSerializer = new RawSerializer(*_pStream);
			}
			return _pRawSerializer;
		}
		else if (format == FORMAT_JSON)
		{
			if (_pRawSerializer) throw Poco::RemotingNG::SerializerException("Cannot use body formats 'raw' and 'json' simultaneously");
			if (_pFormSerializer) throw Poco::RemotingNG::SerializerException("Cannot use locations 'form' and 'body' simultaneously");
			if (!_pJSONSerializer)
			{
				_pJSONSerializer = new JSONSerializer(*_pStream);
			}
			return _pJSONSerializer;
		}
		else throw Poco::RemotingNG::SerializerException("Unsupported format", format);
	}
	else if (location == LOCATION_FORM)
	{
		if (_pRawSerializer || _pJSONSerializer) throw Poco::RemotingNG::SerializerException("Cannot use locations 'form' and 'body' simultaneously");
		if (!_pFormSerializer)
		{
			if (_pRequest)
				_pFormSerializer = new FormSerializer(*_pRequest, *_pStream);
			else if (_pResponse)
				_pFormSerializer = new FormSerializer(*_pResponse, *_pStream);
			else
				throw Poco::IllegalStateException("No request or response object available");
		}
		return _pFormSerializer;
	}
	else throw Poco::RemotingNG::SerializerException("Invalid location", location);
}


} } } // namespace Poco::RemotingNG::REST
