//
// TesterRemoteObject.cpp
//
// Package: Generated
// Module:  TesterRemoteObject
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#include "TesterRemoteObject.h"


TesterRemoteObject::TesterRemoteObject(const Poco::RemotingNG::Identifiable::ObjectId& oid, Poco::SharedPtr<Tester> pServiceObject):
	ITester(),
	Poco::RemotingNG::RemoteObject(oid),
	_pServiceObject(pServiceObject)
{
}


TesterRemoteObject::~TesterRemoteObject()
{
	try
	{
	}
	catch (...)
	{
		poco_unexpected();
	}
}


