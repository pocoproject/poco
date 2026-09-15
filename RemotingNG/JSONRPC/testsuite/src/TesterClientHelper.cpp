//
// TesterClientHelper.cpp
//
// Package: Generated
// Module:  TesterClientHelper
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#include "TesterClientHelper.h"
#include "Poco/SingletonHolder.h"
#include "TesterProxyFactory.h"


namespace
{
	Poco::SingletonHolder<TesterClientHelper> shTesterClientHelper;
}


TesterClientHelper::TesterClientHelper():
	_pORB(0)
{
	_pORB = &Poco::RemotingNG::ORB::instance();
	_pORB->registerProxyFactory("Tester", new TesterProxyFactory);
}


TesterClientHelper::~TesterClientHelper()
{
}


ITester::Ptr TesterClientHelper::findImpl(const std::string& uri)
{
	Poco::RemotingNG::Identifiable::Ptr pInterface = _pORB->findObject(uri);
	return pInterface.cast<ITester>();
}


ITester::Ptr TesterClientHelper::findImpl(const std::string& uri, const std::string& protocol)
{
	Poco::RemotingNG::Identifiable::Ptr pInterface = _pORB->findObject(uri, ITester::remoting__typeId(), protocol);
	return pInterface.cast<ITester>();
}


TesterClientHelper& TesterClientHelper::instance()
{
	return *shTesterClientHelper.get();
}


