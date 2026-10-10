//
// TesterServerHelper.cpp
//
// Package: Generated
// Module:  TesterServerHelper
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#include "TesterServerHelper.h"
#include "Poco/RemotingNG/URIUtility.h"
#include "Poco/SingletonHolder.h"
#include "TesterSkeleton.h"


namespace
{
	Poco::SingletonHolder<TesterServerHelper> shTesterServerHelper;
}


TesterServerHelper::TesterServerHelper():
	_pORB(0)
{
	_pORB = &Poco::RemotingNG::ORB::instance();
	registerSkeleton();
}


TesterServerHelper::~TesterServerHelper()
{
}


void TesterServerHelper::shutdown()
{
	TesterServerHelper::instance().unregisterSkeleton();
	shTesterServerHelper.reset();
}


Poco::AutoPtr<TesterRemoteObject> TesterServerHelper::createRemoteObjectImpl(Poco::SharedPtr<Tester> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid)
{
	Poco::AutoPtr<TesterRemoteObject > pRemoteObject = new TesterRemoteObject(oid, pServiceObject);
	pRemoteObject->remoting__setURI(Poco::URI("/tester/{id}"));
	return pRemoteObject;
}


TesterServerHelper& TesterServerHelper::instance()
{
	return *shTesterServerHelper.get();
}


std::string TesterServerHelper::registerObjectImpl(Poco::AutoPtr<TesterRemoteObject> pRemoteObject, const std::string& listenerId)
{
	return _pORB->registerObject(pRemoteObject, listenerId);
}


void TesterServerHelper::registerSkeleton()
{
	_pORB->registerSkeleton("Tester", new TesterSkeleton);
}


void TesterServerHelper::unregisterObjectImpl(const std::string& uri)
{
	_pORB->unregisterObject(uri);
}


void TesterServerHelper::unregisterSkeleton()
{
	_pORB->unregisterSkeleton("Tester", true);
}


