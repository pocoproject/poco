//
// CheckVatServiceClientHelper.cpp
//
// Package: Generated
// Module:  CheckVatServiceClientHelper
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#include "CheckVat/CheckVatServiceClientHelper.h"
#include "CheckVat/CheckVatServiceProxyFactory.h"
#include "Poco/SingletonHolder.h"


namespace CheckVat {


namespace
{
	Poco::SingletonHolder<CheckVatServiceClientHelper> shCheckVatServiceClientHelper;
}


CheckVatServiceClientHelper::CheckVatServiceClientHelper():
	_pORB(0)
{
	_pORB = &Poco::RemotingNG::ORB::instance();
	_pORB->registerProxyFactory("CheckVat.CheckVatService", new CheckVatServiceProxyFactory);
}


CheckVatServiceClientHelper::~CheckVatServiceClientHelper()
{
}


ICheckVatService::Ptr CheckVatServiceClientHelper::findImpl(const std::string& uri)
{
	Poco::RemotingNG::Identifiable::Ptr pInterface = _pORB->findObject(uri);
	return pInterface.cast<ICheckVatService>();
}


ICheckVatService::Ptr CheckVatServiceClientHelper::findImpl(const std::string& uri, const std::string& protocol)
{
	Poco::RemotingNG::Identifiable::Ptr pInterface = _pORB->findObject(uri, ICheckVatService::remoting__typeId(), protocol);
	return pInterface.cast<ICheckVatService>();
}


CheckVatServiceClientHelper& CheckVatServiceClientHelper::instance()
{
	return *shCheckVatServiceClientHelper.get();
}


} // namespace CheckVat

