//
// Token.cpp
//
// Library: JWT
// Package: JWT
// Module:  Token
//
// Copyright (c) 2019, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/JWT/Token.h"
#include "Poco/JWT/Serializer.h"
#include "Poco/JWT/JWTException.h"


namespace Poco {
namespace JWT {


const std::string Token::CLAIM_ISSUER("iss");
const std::string Token::CLAIM_SUBJECT("sub");
const std::string Token::CLAIM_AUDIENCE("aud");
const std::string Token::CLAIM_EXPIRATION("exp");
const std::string Token::CLAIM_NOT_BEFORE("nbf");
const std::string Token::CLAIM_ISSUED_AT("iat");
const std::string Token::CLAIM_JWT_ID("jti");
const std::string Token::CLAIM_TYPE("typ");
const std::string Token::CLAIM_ALGORITHM("alg");
const std::string Token::CLAIM_CONTENT_TYPE("cty");
const std::string Token::EMPTY;


Token::Token():
	_pHeader(new Poco::JSON::Object),
	_pPayload(new Poco::JSON::Object)
{
}


Token::Token(const std::string& token)
{
	std::vector<std::string> parts = Serializer::split(token);
	if (parts.size() < 3) throw ParseException("Not a valid JWT", token);
	_pHeader = Serializer::deserialize(parts[0]);
	_pPayload = Serializer::deserialize(parts[1]);
	_signature = parts[2];
}


Token::Token(const Token& token):
	_pHeader(new Poco::JSON::Object(*token._pHeader)),
	_pPayload(new Poco::JSON::Object(*token._pPayload)),
	_signature(token._signature)
{
}


Token::Token(Token&& token) noexcept:
	_pHeader(std::move(token._pHeader)),
	_pPayload(std::move(token._pPayload)),
	_signature(std::move(token._signature))
{
}


Token::Token(const std::string& header, const std::string& payload, const std::string& signature)
{
	assign(header, payload, signature);
}


Token::~Token()
{
}


Token& Token::operator = (const Token& token)
{
	if (&token != this)
	{
		Poco::JSON::Object::Ptr pHeader  = new Poco::JSON::Object(*token._pHeader);
		Poco::JSON::Object::Ptr pPayload = new Poco::JSON::Object(*token._pPayload);
		std::string signature = token._signature;

		std::swap(_pHeader, pHeader);
		std::swap(_pPayload, pPayload);
		std::swap(_signature, signature);
	}
	return *this;
}


Token& Token::operator = (Token&& token) noexcept
{
	_pHeader = std::move(token._pHeader);
	_pPayload = std::move(token._pPayload);
	_signature = std::move(token._signature);

	return *this;
}


Token& Token::operator = (const std::string& token)
{
	std::vector<std::string> parts = Serializer::split(token);
	if (parts.size() < 3) throw ParseException("Not a valid JWT", token);
	assign(parts[0], parts[1], parts[2]);

	return *this;
}


void Token::assign(const std::string& header, const std::string& payload, const std::string& signature)
{
	Poco::JSON::Object::Ptr pHeader  = Serializer::deserialize(header);
	Poco::JSON::Object::Ptr pPayload = Serializer::deserialize(payload);
	std::string aSignature = signature;

	std::swap(_pHeader, pHeader);
	std::swap(_pPayload, pPayload);
	std::swap(_signature, aSignature);
}


std::string Token::toString() const
{
	std::ostringstream stream;
	Serializer::serialize(*_pHeader, stream);
	stream << '.';
	Serializer::serialize(*_pPayload, stream);
	stream << '.';
	stream << _signature;
	return stream.str();
}


void Token::setAudience(const std::vector<std::string>& audience)
{
	Poco::JSON::Array::Ptr pArray = new Poco::JSON::Array;
	for (std::vector<std::string>::const_iterator it = audience.begin(); it != audience.end(); ++it)
	{
		pArray->add(*it);
	}
	_pPayload->set(CLAIM_AUDIENCE, pArray);
}


std::vector<std::string> Token::getAudience() const
{
	std::vector<std::string> result;
	if (_pPayload->has(CLAIM_AUDIENCE))
	{
		if (_pPayload->isArray(CLAIM_AUDIENCE))
		{
			Poco::JSON::Array::Ptr pArray = _pPayload->getArray(CLAIM_AUDIENCE);
			if (pArray)
			{
				for (unsigned i = 0; i < pArray->size(); i++)
				{
					result.push_back(pArray->getElement<std::string>(i));
				}
			}
		}
		else
		{
			result.push_back(_pPayload->getValue<std::string>(CLAIM_AUDIENCE));
		}
	}
	return result;
}


void Token::setTimestamp(const std::string& claim, const Poco::Timestamp& ts)
{
	double epochSeconds = static_cast<double>(ts.epochMicroseconds())/Poco::Timestamp::resolution();
	_pPayload->set(claim, epochSeconds);
}


Poco::Timestamp Token::getTimestamp(const std::string& claim) const
{
	double epochSeconds = _pPayload->optValue(claim, 0.0);
	Poco::Timestamp::TimeVal tv = static_cast<Poco::Timestamp::TimeVal>(epochSeconds*Poco::Timestamp::resolution());
	return Poco::Timestamp(tv);
}


void Token::sign(const std::string& signature)
{
	_signature = signature;
}


} } // namespace Poco::JWT
