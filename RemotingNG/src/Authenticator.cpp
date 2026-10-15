//
// Authenticator.cpp
//
// Library: RemotingNG
// Package: Transport
// Module:  Authenticator
//
// Copyright (c) 2017, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RemotingNG/Authenticator.h"


namespace Poco::RemotingNG {


//
// AuthenticateResult
//


AuthenticateResult::AuthenticateResult():
	_state(AUTH_FAILED),
	_conversationID(0)
{
}


AuthenticateResult::AuthenticateResult(State state):
	_state(state),
	_conversationID(0)
{
}


AuthenticateResult::AuthenticateResult(State state, const Credentials& credentials, Poco::UInt32 conversationID):
	_state(state),
	_credentials(credentials),
	_conversationID(conversationID)
{
}

	
AuthenticateResult::AuthenticateResult(const AuthenticateResult& other) = default;


AuthenticateResult::~AuthenticateResult() = default;

	
AuthenticateResult& AuthenticateResult::operator = (const AuthenticateResult& other)
{
	if (&other != this)
	{
		_credentials = other._credentials;
		_state = other._state;
		_conversationID = other._conversationID;
	}
	return *this;
}


//
// Authenticator
//


Authenticator::Authenticator() = default;


Authenticator::~Authenticator() = default;


} // namespace Poco::RemotingNG
