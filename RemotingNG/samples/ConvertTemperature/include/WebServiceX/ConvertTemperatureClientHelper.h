//
// ConvertTemperatureClientHelper.h
//
// Package: Generated
// Module:  ConvertTemperatureClientHelper
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef ConvertTemperatureClientHelper_INCLUDED
#define ConvertTemperatureClientHelper_INCLUDED


#include "Poco/RemotingNG/Identifiable.h"
#include "Poco/RemotingNG/ORB.h"
#include "WebServiceX/IConvertTemperature.h"


namespace WebServiceX {


class ConvertTemperatureClientHelper
{
public:
	ConvertTemperatureClientHelper();
		/// Creates a ConvertTemperatureClientHelper.

	~ConvertTemperatureClientHelper();
		/// Destroys the ConvertTemperatureClientHelper.

	static IConvertTemperature::Ptr find(const std::string& uri);
		/// Return an interface for the service object identified by the given URI.
		///
		/// Depending on whether the service object has been registered on the same ORB, or not,
		/// the ORB will either return a RemoteObject (with forwards calls locally, without the
		/// need for serialization/deserialization), or a Proxy.
		///
		/// The URI must have the following format: <scheme>://<authority>/<protocol>/<typeId>/<objectId>

	static IConvertTemperature::Ptr find(const std::string& uri, const std::string& protocol);
		/// Return a Proxy for the service object identified by the given URI.
		///
		/// The given protocol name is used to determine the Transport used by
		/// the Proxy. This is used for objects identified by URIs that do not
		/// follow the standard Remoting URI structure.

private:
	IConvertTemperature::Ptr findImpl(const std::string& uri);

	IConvertTemperature::Ptr findImpl(const std::string& uri, const std::string& protocol);

	static ConvertTemperatureClientHelper& instance();

	Poco::RemotingNG::ORB* _pORB;
};


inline IConvertTemperature::Ptr ConvertTemperatureClientHelper::find(const std::string& uri)
{
	return ConvertTemperatureClientHelper::instance().findImpl(uri);
}


inline IConvertTemperature::Ptr ConvertTemperatureClientHelper::find(const std::string& uri, const std::string& protocol)
{
	return ConvertTemperatureClientHelper::instance().findImpl(uri, protocol);
}


} // namespace WebServiceX


#endif // ConvertTemperatureClientHelper_INCLUDED

