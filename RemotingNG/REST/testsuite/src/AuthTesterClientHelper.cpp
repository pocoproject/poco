//
// AuthTesterClientHelper.cpp
//
// Package: Generated
// Module:  AuthTesterClientHelper
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#include "AuthTesterClientHelper.h"
#include "AuthTesterProxyFactory.h"
#include "Poco/SingletonHolder.h"


namespace
{
	Poco::SingletonHolder<AuthTesterClientHelper> shAuthTesterClientHelper;
}


AuthTesterClientHelper::AuthTesterClientHelper():
	_pORB(0)
{
	_pORB = &Poco::RemotingNG::ORB::instance();
	_pORB->registerProxyFactory("AuthTester", new AuthTesterProxyFactory);
}


AuthTesterClientHelper::~AuthTesterClientHelper()
{
}


IAuthTester::Ptr AuthTesterClientHelper::findImpl(const std::string& uri)
{
	Poco::RemotingNG::Identifiable::Ptr pInterface = _pORB->findObject(uri);
	return pInterface.cast<IAuthTester>();
}


IAuthTester::Ptr AuthTesterClientHelper::findImpl(const std::string& uri, const std::string& protocol)
{
	Poco::RemotingNG::Identifiable::Ptr pInterface = _pORB->findObject(uri, IAuthTester::remoting__typeId(), protocol);
	return pInterface.cast<IAuthTester>();
}


AuthTesterClientHelper& AuthTesterClientHelper::instance()
{
	return *shAuthTesterClientHelper.get();
}


