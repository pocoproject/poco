//
// ConvertTemperatureClientHelper.cpp
//
// Package: Generated
// Module:  ConvertTemperatureClientHelper
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#include "WebServiceX/ConvertTemperatureClientHelper.h"
#include "Poco/SingletonHolder.h"
#include "WebServiceX/ConvertTemperatureProxyFactory.h"


namespace WebServiceX {


namespace
{
	Poco::SingletonHolder<ConvertTemperatureClientHelper> shConvertTemperatureClientHelper;
}


ConvertTemperatureClientHelper::ConvertTemperatureClientHelper():
	_pORB(0)
{
	_pORB = &Poco::RemotingNG::ORB::instance();
	_pORB->registerProxyFactory("WebServiceX.ConvertTemperature", new ConvertTemperatureProxyFactory);
}


ConvertTemperatureClientHelper::~ConvertTemperatureClientHelper()
{
}


IConvertTemperature::Ptr ConvertTemperatureClientHelper::findImpl(const std::string& uri)
{
	Poco::RemotingNG::Identifiable::Ptr pInterface = _pORB->findObject(uri);
	return pInterface.cast<IConvertTemperature>();
}


IConvertTemperature::Ptr ConvertTemperatureClientHelper::findImpl(const std::string& uri, const std::string& protocol)
{
	Poco::RemotingNG::Identifiable::Ptr pInterface = _pORB->findObject(uri, IConvertTemperature::remoting__typeId(), protocol);
	return pInterface.cast<IConvertTemperature>();
}


ConvertTemperatureClientHelper& ConvertTemperatureClientHelper::instance()
{
	return *shConvertTemperatureClientHelper.get();
}


} // namespace WebServiceX

