//
// Authenticator.h
//
// Library: RemotingNG
// Package: Transport
// Module:  Authenticator
//
// Definition of the Authenticator class.
//
// Copyright (c) 2017, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_Authenticator_INCLUDED
#define RemotingNG_Authenticator_INCLUDED


#include "Poco/RemotingNG/RemotingNG.h"
#include "Poco/RemotingNG/Credentials.h"
#include "Poco/RefCountedObject.h"
#include "Poco/AutoPtr.h"


namespace Poco {
namespace RemotingNG {


class RemotingNG_API AuthenticateResult
	/// The result of an (intermediate) authenticate request.
{
public:
	enum State
	{
		AUTH_DONE     = 0x01,
			/// Authentication completed successfully.
			
		AUTH_CONTINUE = 0x02,
			/// Authentication step successful, continue with next step.

		AUTH_FAILED   = 0x80
			/// Authentication failed.
	};
	
	AuthenticateResult();
		/// Creates an empty AuthenticateResult with state AUTH_FAILED
		/// and conversation ID 0.
		
	explicit AuthenticateResult(State state);
		/// Creates an empty AuthenticateResult with the given state
		/// and conversation ID 0.
	
	AuthenticateResult(State state, const Credentials& credentials, Poco::UInt32 conversationID = 0);
		/// Creates an AuthenticateResult using the given state,
		/// credentials and conversationID.
		
	AuthenticateResult(const AuthenticateResult& other);
		/// Creates an AuthenticateResult by copying another one.
	
	~AuthenticateResult();
		/// Destroys the AuthenticateResult.
		
	AuthenticateResult& operator = (const AuthenticateResult& other);
		/// Assignment operator.
	
	State state() const;
		/// Returns the status.

	bool done() const;
		/// Returns true if authentication has completed successfully.
		
	bool cont() const;
		/// Returns true if the authentication step was successful and the
		/// next step must be performed to eventually complete authentication.
		
	bool failed() const;
		/// Returns true if authentication has failed.
		
	const Credentials& credentials() const;
		/// Returns the credentials.
		
	Poco::UInt32 conversationID() const;
		/// Returns the conversation ID.
		
private:	
	State _state;
	Credentials _credentials;
	Poco::UInt32 _conversationID;
};


class RemotingNG_API Authenticator: public Poco::RefCountedObject
	/// The Authenticator interface is used for authenticating credentials. 
	///
	/// Authenticators are specific to a transport and external authentication 
	/// mechanism and must be implemented specifically for an application.
	///
	/// The design of the Authentication class allows for multi-request
	/// authentication schemes.
	///
	/// For multi-step authentication mechanisms, the Authenticator must
	/// be able to handle multiple authentication conversations simultaneously.
{
public:
	using Ptr = Poco::AutoPtr<Authenticator>;

	Authenticator();
		/// Creates the Authenticator.

	virtual ~Authenticator();
		/// Destroys the Authenticator.
		
	virtual AuthenticateResult authenticate(const Credentials& credentials, Poco::UInt32 conversationID = 0) = 0;
		/// Verifies that the given credentials are valid.
		///
		/// Returns an AuthenticateResult encapsulating the state of the
		/// authentication exchange.
		///
		/// Depending on the specific authentication mechanism, multiple calls
		/// to authenticate() may be necessary. In the first call, conversationID
		/// should be 0. In subsequent calls, conversationID must contain the
		/// conversationID returned in AuthenticateResult by the previous call.
		///
		/// Authenticators supporting multi-step conversations must be able to
		/// handle multiple simultaneous conversations. For that purpose, the
		/// conversationID is used to associate calls to authenticate with
		/// existing conversations.
		///
		/// As long as the state is AUTH_CONTINUE, the Credentials in the AuthenticateResult
		/// will be returned back to the client. If the state is AUTH_DONE, the credentials
		/// (if set) will be passed on to the Context and will be available to the
		/// Authorizer. However, the exact semantics are up to the specific Transport
		/// implementation.
};


//
// inlines
//
inline AuthenticateResult::State AuthenticateResult::state() const
{
	return _state;
}


inline bool AuthenticateResult::done() const
{
	return _state == AUTH_DONE;
}

	
inline bool AuthenticateResult::cont() const
{
	return _state == AUTH_CONTINUE;
}

	
inline bool AuthenticateResult::failed() const
{
	return _state == AUTH_FAILED;
}

	
inline const Credentials& AuthenticateResult::credentials() const
{
	return _credentials;
}

	
inline Poco::UInt32 AuthenticateResult::conversationID() const
{
	return _conversationID;
}


} } // namespace Poco::RemotingNG


#endif // RemotingNG_Authenticator_INCLUDED
