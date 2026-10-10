//
// AuthTesterClientHelper.h
//
// Package: Generated
// Module:  AuthTesterClientHelper
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef AuthTesterClientHelper_INCLUDED
#define AuthTesterClientHelper_INCLUDED


#include "IAuthTester.h"
#include "Poco/RemotingNG/Identifiable.h"
#include "Poco/RemotingNG/ORB.h"


class AuthTesterClientHelper
{
public:
	AuthTesterClientHelper();
		/// Creates a AuthTesterClientHelper.

	~AuthTesterClientHelper();
		/// Destroys the AuthTesterClientHelper.

	static IAuthTester::Ptr find(const std::string& uri);
		/// Return an interface for the service object identified by the given URI.
		///
		/// Depending on whether the service object has been registered on the same ORB, or not,
		/// the ORB will either return a RemoteObject (with forwards calls locally, without the
		/// need for serialization/deserialization), or a Proxy.
		///
		/// The URI must have the following format: <scheme>://<authority>/<protocol>/<typeId>/<objectId>

	static IAuthTester::Ptr find(const std::string& uri, const std::string& protocol);
		/// Return a Proxy for the service object identified by the given URI.
		///
		/// The given protocol name is used to determine the Transport used by
		/// the Proxy. This is used for objects identified by URIs that do not
		/// follow the standard Remoting URI structure.

private:
	IAuthTester::Ptr findImpl(const std::string& uri);

	IAuthTester::Ptr findImpl(const std::string& uri, const std::string& protocol);

	static AuthTesterClientHelper& instance();

	Poco::RemotingNG::ORB* _pORB;
};


inline IAuthTester::Ptr AuthTesterClientHelper::find(const std::string& uri)
{
	return AuthTesterClientHelper::instance().findImpl(uri);
}


inline IAuthTester::Ptr AuthTesterClientHelper::find(const std::string& uri, const std::string& protocol)
{
	return AuthTesterClientHelper::instance().findImpl(uri, protocol);
}




#endif // AuthTesterClientHelper_INCLUDED

