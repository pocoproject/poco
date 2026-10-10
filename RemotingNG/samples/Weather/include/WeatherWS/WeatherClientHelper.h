//
// WeatherClientHelper.h
//
// Package: Generated
// Module:  WeatherClientHelper
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef WeatherClientHelper_INCLUDED
#define WeatherClientHelper_INCLUDED


#include "Poco/RemotingNG/Identifiable.h"
#include "Poco/RemotingNG/ORB.h"
#include "WeatherWS/IWeather.h"


namespace WeatherWS {


class WeatherClientHelper
{
public:
	WeatherClientHelper();
		/// Creates a WeatherClientHelper.

	~WeatherClientHelper();
		/// Destroys the WeatherClientHelper.

	static IWeather::Ptr find(const std::string& uri);
		/// Return an interface for the service object identified by the given URI.
		///
		/// Depending on whether the service object has been registered on the same ORB, or not,
		/// the ORB will either return a RemoteObject (with forwards calls locally, without the
		/// need for serialization/deserialization), or a Proxy.
		///
		/// The URI must have the following format: <scheme>://<authority>/<protocol>/<typeId>/<objectId>

	static IWeather::Ptr find(const std::string& uri, const std::string& protocol);
		/// Return a Proxy for the service object identified by the given URI.
		///
		/// The given protocol name is used to determine the Transport used by
		/// the Proxy. This is used for objects identified by URIs that do not
		/// follow the standard Remoting URI structure.

private:
	IWeather::Ptr findImpl(const std::string& uri);

	IWeather::Ptr findImpl(const std::string& uri, const std::string& protocol);

	static WeatherClientHelper& instance();

	Poco::RemotingNG::ORB* _pORB;
};


inline IWeather::Ptr WeatherClientHelper::find(const std::string& uri)
{
	return WeatherClientHelper::instance().findImpl(uri);
}


inline IWeather::Ptr WeatherClientHelper::find(const std::string& uri, const std::string& protocol)
{
	return WeatherClientHelper::instance().findImpl(uri, protocol);
}


} // namespace WeatherWS


#endif // WeatherClientHelper_INCLUDED

