//
// AuthTesterRemoteObject.cpp
//
// Package: Generated
// Module:  AuthTesterRemoteObject
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#include "AuthTesterRemoteObject.h"


AuthTesterRemoteObject::AuthTesterRemoteObject(const Poco::RemotingNG::Identifiable::ObjectId& oid, Poco::SharedPtr<AuthTester> pServiceObject):
	IAuthTester(),
	Poco::RemotingNG::RemoteObject(oid),
	_pServiceObject(pServiceObject)
{
}


AuthTesterRemoteObject::~AuthTesterRemoteObject()
{
	try
	{
	}
	catch (...)
	{
		poco_unexpected();
	}
}


