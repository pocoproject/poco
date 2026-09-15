//
// Credentials.h
//
// Library: RemotingNG
// Package: Transport
// Module:  Credentials
//
// Definition of the Credentials class.
//
// Copyright (c) 2017, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#ifndef RemotingNG_Credentials_INCLUDED
#define RemotingNG_Credentials_INCLUDED


#include "Poco/RemotingNG/AttributedObject.h"


namespace Poco {
namespace RemotingNG {


class RemotingNG_API Credentials: public AttributedObject
	/// This class contains credentials for authentication,
	/// in the form of key-value pairs (attributes).
	///
	/// The content of a Credentials object depends upon a
	/// specific authentication mechanism and is implementation 
	/// specific.
	///
	/// In case of username/password credentials, a Credentials
	/// object would contain a username (ATTR_USERNAME, "n") and 
	/// a password (ATTR_PASSWORD, "p") attribute.
{
public:
	Credentials();
		/// Creates an empty Credentials object.

	Credentials(const Credentials& other);
		/// Creates a Credentials object by copying the attributes from another one.

	Credentials(Credentials&& other) noexcept;
		/// Creates a Credentials object by moving the attributes from another one.
		
	Credentials& operator = (const Credentials& other);
		/// Assigns the attributes from another Credentials object.

	Credentials& operator = (Credentials&& other) noexcept;
		/// Assigns the attributes from another Credentials object.
		
	~Credentials();
		/// Destroys the Credentials object.
		
	static const std::string ATTR_USERNAME;
		/// Attribute named "n" containing a username.
	static const std::string ATTR_PASSWORD;
		/// Attribute named "p" containing a password or proof.
	static const std::string ATTR_MECHANISM;
		/// Attribute named "#mechanism" containing the authentication mechanism.
	static const std::string ATTR_TOKEN;
		/// Attribute named "t" containing a bearer token.
	static const std::string ATTR_NONCE;
		/// Attribute named "r" containing a nonce.
	static const std::string ATTR_SALT;
		/// Attribute named "s" containing a password salt.
	static const std::string ATTR_ITERATIONS;
		/// Attribute named "i" containing the number of
		/// iterations for PBKDF2.
	static const std::string ATTR_SIGNATURE;
		/// Attribute named "v" containing a signature.
	static const std::string ATTR_CREATED;
		/// Attribute named "c" containing a credentials creation date.
};


} } // namespace Poco::RemotingNG


#endif // RemotingNG_Credentials_INCLUDED
