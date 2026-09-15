//
// TesterClientHelper.h
//
// Package: Generated
// Module:  TesterClientHelper
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TesterClientHelper_INCLUDED
#define TesterClientHelper_INCLUDED


#include "ITester.h"
#include "Poco/RemotingNG/Identifiable.h"
#include "Poco/RemotingNG/ORB.h"


class TesterClientHelper
{
public:
	TesterClientHelper();
		/// Creates a TesterClientHelper.

	~TesterClientHelper();
		/// Destroys the TesterClientHelper.

	static ITester::Ptr find(const std::string& uri);
		/// Return an interface for the service object identified by the given URI.
		///
		/// Depending on whether the service object has been registered on the same ORB, or not,
		/// the ORB will either return a RemoteObject (with forwards calls locally, without the
		/// need for serialization/deserialization), or a Proxy.
		///
		/// The URI must have the following format: <scheme>://<authority>/<protocol>/<typeId>/<objectId>

	static ITester::Ptr find(const std::string& uri, const std::string& protocol);
		/// Return a Proxy for the service object identified by the given URI.
		///
		/// The given protocol name is used to determine the Transport used by
		/// the Proxy. This is used for objects identified by URIs that do not
		/// follow the standard Remoting URI structure.

private:
	ITester::Ptr findImpl(const std::string& uri);

	ITester::Ptr findImpl(const std::string& uri, const std::string& protocol);

	static TesterClientHelper& instance();

	Poco::RemotingNG::ORB* _pORB;
};


inline ITester::Ptr TesterClientHelper::find(const std::string& uri)
{
	return TesterClientHelper::instance().findImpl(uri);
}


inline ITester::Ptr TesterClientHelper::find(const std::string& uri, const std::string& protocol)
{
	return TesterClientHelper::instance().findImpl(uri, protocol);
}




#endif // TesterClientHelper_INCLUDED

