//
// Token.h
//
// Library: JWT
// Package: JWT
// Module:  Token
//
// Definition of the Token class.
//
// Copyright (c) 2019, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef JWT_Token_INCLUDED
#define JWT_Token_INCLUDED


#include "Poco/JWT/JWT.h"
#include "Poco/JSON/Object.h"
#include "Poco/Timestamp.h"


namespace Poco {
namespace JWT {


class JWT_API Token
	/// This class represents a JSON Web Token (JWT) according to RFC 7519.
	///
	/// To create and sign a JWT (using the Signer class):
	///
	///     Token token;
	///     token.setType("JWT");
	///     token.setSubject("1234567890");
	///     token.payload().set("name", std::string("John Doe"));
	///     token.setIssuedAt(Poco::Timestamp()));
	///
	///     Signer signer("0123456789ABCDEF0123456789ABCDEF");
	///     std::string jwt = signer.sign(token, Signer::ALGO_HS256);
	///
	/// To verify a signed token:
	///
	///     std::string jwt(
	///     	"eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9."
	///     	"eyJpYXQiOjE1MTYyMzkwMjIsIm5hbWUiOiJKb2huIERvZSIsInN1YiI6IjEyMzQ1Njc4OTAifQ."
	///     	"qn9G7NwFEOjIh-7hfCUDZA1aJeQmf7I7YvzCBcdenGw");
	///
	///     Signer signer("0123456789ABCDEF0123456789ABCDEF");
	///     Token token = signer.verify(jwt);
{
public:
	Token();
		/// Creates an empty JSON Web Token.

	explicit Token(const std::string& token);
		/// Creates a JSON Web Token from its serialized string representation.

	Token(const Token& token);
		/// Creates a JSON Web Token by copying another one.

	Token(Token&& token) noexcept;
		/// Creates a JSON Web Token by moving another one.

	~Token();
		/// Destroys the Token.

	Token& operator = (const Token& token);
		/// Assignment operator.

	Token& operator = (Token&& token) noexcept;
		/// Move assignment operator.

	Token& operator = (const std::string& token);
		/// Parses and assigns serialized JWT.

	std::string toString() const;
		/// Returns the serialized string representation of the JSON Web Token.

	const Poco::JSON::Object& header() const;
		/// Returns the header JSON object.

	Poco::JSON::Object& header();
		/// Returns the header JSON object.

	const Poco::JSON::Object& payload() const;
		/// Returns the payload JSON object.

	Poco::JSON::Object& payload();
		/// Returns the payload JSON object.

	const std::string& signature() const;
		/// Returns the signature string.

	void setIssuer(const std::string& issuer);
		/// Sets the issuer ("iss" claim in payload).

	std::string getIssuer() const;
		/// Returns the issuer, or an empty string if no issuer has been specified.

	void setSubject(const std::string& subject);
		/// Sets the subject ("sub" claim in paylod).

	std::string getSubject() const;
		/// Returns the subject, or an empty string if no issuer has been specified.

	void setAudience(const std::string& audience);
		/// Sets the audience ("aud" claim in payload) to a single value.

	void setAudience(const std::vector<std::string>& audience);
		/// Sets the audience ("aud" claim in payload).

	std::vector<std::string> getAudience() const;
		/// Returns the audience.

	void setExpiration(const Poco::Timestamp& expiration);
		/// Sets the expiration ("exp" claim in payload).

	Poco::Timestamp getExpiration() const;
		/// Returns the expiration, or a zero Poco::Timestamp if no expiration has been specified.

	void setNotBefore(const Poco::Timestamp& notBefore);
		/// Sets the not-before time ("nbf" claim in payload).

	Poco::Timestamp getNotBefore() const;
		/// Returns the not-before time, or a zero Poco::Timestamp if no not-before time has been specified.

	void setIssuedAt(const Poco::Timestamp& issuedAt);
		/// Sets the issued-at time ("iat" claim in payload).

	Poco::Timestamp getIssuedAt() const;
		/// Returns the issued-at time, or a zero Poco::Timestamp if no issued-at time has been specified.

	void setId(const std::string& id);
		/// Sets the JWT ID ("jti" claim in payload).

	std::string getId() const;
		/// Returns the JWT ID, or an empty string if no JWT ID has been specified.

	void setType(const std::string& type);
		/// Sets the JWT type ("typ" claim in payload).

	std::string getType() const;
		/// Returns the JWT type or an empty string if no type has been specified.

	void setAlgorithm(const std::string& algorithm);
		/// Sets the JWT signature algorithm ("alg" claim in header).

	std::string getAlgorithm() const;
		/// Returns the JWT signature algorithm, or an empty string if no algorithm has been specified.

	void setContentType(const std::string& contentType);
		/// Sets the JWT content type ("cty" claim in header").

	std::string getContentType() const;
		/// Returns the JWT content type, or an empty string if no content type has been specified.

	static const std::string CLAIM_ISSUER;
	static const std::string CLAIM_SUBJECT;
	static const std::string CLAIM_AUDIENCE;
	static const std::string CLAIM_EXPIRATION;
	static const std::string CLAIM_NOT_BEFORE;
	static const std::string CLAIM_ISSUED_AT;
	static const std::string CLAIM_JWT_ID;
	static const std::string CLAIM_TYPE;
	static const std::string CLAIM_ALGORITHM;
	static const std::string CLAIM_CONTENT_TYPE;

protected:
	Token(const std::string& header, const std::string& payload, const std::string& signature);

	void sign(const std::string& signature);
	void setTimestamp(const std::string& claim, const Poco::Timestamp& ts);
	Poco::Timestamp getTimestamp(const std::string& claim) const;
	void assign(const std::string& header, const std::string& payload, const std::string& signature);

private:
	Poco::JSON::Object::Ptr _pHeader;
	Poco::JSON::Object::Ptr _pPayload;
	std::string _signature;

	static const std::string EMPTY;

	friend class Signer;
};


//
// inlines
//


inline const Poco::JSON::Object& Token::header() const
{
	return *_pHeader;
}


inline Poco::JSON::Object& Token::header()
{
	return *_pHeader;
}


inline const Poco::JSON::Object& Token::payload() const
{
	return *_pPayload;
}


inline Poco::JSON::Object& Token::payload()
{
	return *_pPayload;
}


inline const std::string& Token::signature() const
{
	return _signature;
}


inline void Token::setIssuer(const std::string& issuer)
{
	_pPayload->set(CLAIM_ISSUER, issuer);
}


inline std::string Token::getIssuer() const
{
	return _pPayload->optValue(CLAIM_ISSUER, EMPTY);
}


inline void Token::setSubject(const std::string& subject)
{
	_pPayload->set(CLAIM_SUBJECT, subject);
}


inline std::string Token::getSubject() const
{
	return _pPayload->optValue(CLAIM_SUBJECT, EMPTY);
}


inline void Token::setAudience(const std::string& audience)
{
	_pPayload->set(CLAIM_AUDIENCE, audience);
}


inline void Token::setExpiration(const Poco::Timestamp& expiration)
{
	setTimestamp(CLAIM_EXPIRATION, expiration);
}


inline Poco::Timestamp Token::getExpiration() const
{
	return getTimestamp(CLAIM_EXPIRATION);
}


inline void Token::setNotBefore(const Poco::Timestamp& notBefore)
{
	setTimestamp(CLAIM_NOT_BEFORE, notBefore);
}


inline Poco::Timestamp Token::getNotBefore() const
{
	return getTimestamp(CLAIM_NOT_BEFORE);
}


inline void Token::setIssuedAt(const Poco::Timestamp& issuedAt)
{
	setTimestamp(CLAIM_ISSUED_AT, issuedAt);
}


inline Poco::Timestamp Token::getIssuedAt() const
{
	return getTimestamp(CLAIM_ISSUED_AT);
}


inline void Token::setId(const std::string& id)
{
	_pPayload->set(CLAIM_JWT_ID, id);
}


inline std::string Token::getId() const
{
	return _pPayload->optValue(CLAIM_JWT_ID, EMPTY);
}


inline void Token::setType(const std::string& type)
{
	_pHeader->set(CLAIM_TYPE, type);
}


inline std::string Token::getType() const
{
	return _pHeader->optValue(CLAIM_TYPE, EMPTY);
}


inline void Token::setAlgorithm(const std::string& algorithm)
{
	_pHeader->set(CLAIM_ALGORITHM, algorithm);
}


inline std::string Token::getAlgorithm() const
{
	return _pHeader->optValue(CLAIM_ALGORITHM, EMPTY);
}


inline void Token::setContentType(const std::string& contentType)
{
	_pHeader->set(CLAIM_CONTENT_TYPE, contentType);
}


inline std::string Token::getContentType() const
{
	return _pHeader->optValue(CLAIM_CONTENT_TYPE, EMPTY);
}


} } // namespace Poco::JWT


#endif // JWT_Token_INCLUDED
