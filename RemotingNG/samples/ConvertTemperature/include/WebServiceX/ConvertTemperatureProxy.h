//
// ConvertTemperatureProxy.h
//
// Package: Generated
// Module:  ConvertTemperatureProxy
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef ConvertTemperatureProxy_INCLUDED
#define ConvertTemperatureProxy_INCLUDED


#include "Poco/RemotingNG/Proxy.h"
#include "WebServiceX/IConvertTemperature.h"


namespace WebServiceX {


class ConvertTemperatureProxy: public WebServiceX::IConvertTemperature, public Poco::RemotingNG::Proxy
{
public:
	using Ptr = Poco::AutoPtr<ConvertTemperatureProxy>;

	ConvertTemperatureProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid);
		/// Creates a ConvertTemperatureProxy.

	virtual ~ConvertTemperatureProxy();
		/// Destroys the ConvertTemperatureProxy.

	virtual void convertTemp(double temperature, const std::string& fromUnit, const std::string& toUnit, double& convertTempResult);

	virtual const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId() const;

private:
	static const std::string DEFAULT_NS;
};


inline const Poco::RemotingNG::Identifiable::TypeId& ConvertTemperatureProxy::remoting__typeId() const
{
	return IConvertTemperature::remoting__typeId();
}


} // namespace WebServiceX


#endif // ConvertTemperatureProxy_INCLUDED

