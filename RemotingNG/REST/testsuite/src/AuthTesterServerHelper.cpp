//
// AuthTesterServerHelper.cpp
//
// Package: Generated
// Module:  AuthTesterServerHelper
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#include "AuthTesterServerHelper.h"
#include "AuthTesterSkeleton.h"
#include "Poco/RemotingNG/URIUtility.h"
#include "Poco/SingletonHolder.h"


namespace
{
	Poco::SingletonHolder<AuthTesterServerHelper> shAuthTesterServerHelper;
}


AuthTesterServerHelper::AuthTesterServerHelper():
	_pORB(0)
{
	_pORB = &Poco::RemotingNG::ORB::instance();
	registerSkeleton();
}


AuthTesterServerHelper::~AuthTesterServerHelper()
{
}


void AuthTesterServerHelper::shutdown()
{
	AuthTesterServerHelper::instance().unregisterSkeleton();
	shAuthTesterServerHelper.reset();
}


Poco::AutoPtr<AuthTesterRemoteObject> AuthTesterServerHelper::createRemoteObjectImpl(Poco::SharedPtr<AuthTester> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid)
{
	Poco::AutoPtr<AuthTesterRemoteObject > pRemoteObject = new AuthTesterRemoteObject(oid, pServiceObject);
	pRemoteObject->remoting__setURI(Poco::URI("/authtester/{id}"));
	return pRemoteObject;
}


AuthTesterServerHelper& AuthTesterServerHelper::instance()
{
	return *shAuthTesterServerHelper.get();
}


std::string AuthTesterServerHelper::registerObjectImpl(Poco::AutoPtr<AuthTesterRemoteObject> pRemoteObject, const std::string& listenerId)
{
	return _pORB->registerObject(pRemoteObject, listenerId);
}


void AuthTesterServerHelper::registerSkeleton()
{
	_pORB->registerSkeleton("AuthTester", new AuthTesterSkeleton);
}


void AuthTesterServerHelper::unregisterObjectImpl(const std::string& uri)
{
	_pORB->unregisterObject(uri);
}


void AuthTesterServerHelper::unregisterSkeleton()
{
	_pORB->unregisterSkeleton("AuthTester", true);
}


