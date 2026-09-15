//
// CheckVatServiceClientHelper.h
//
// Package: Generated
// Module:  CheckVatServiceClientHelper
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef CheckVatServiceClientHelper_INCLUDED
#define CheckVatServiceClientHelper_INCLUDED


#include "CheckVat/ICheckVatService.h"
#include "Poco/RemotingNG/Identifiable.h"
#include "Poco/RemotingNG/ORB.h"


namespace CheckVat {


class CheckVatServiceClientHelper
{
public:
	CheckVatServiceClientHelper();
		/// Creates a CheckVatServiceClientHelper.

	~CheckVatServiceClientHelper();
		/// Destroys the CheckVatServiceClientHelper.

	static ICheckVatService::Ptr find(const std::string& uri);
		/// Return an interface for the service object identified by the given URI.
		///
		/// Depending on whether the service object has been registered on the same ORB, or not,
		/// the ORB will either return a RemoteObject (with forwards calls locally, without the
		/// need for serialization/deserialization), or a Proxy.
		///
		/// The URI must have the following format: <scheme>://<authority>/<protocol>/<typeId>/<objectId>

	static ICheckVatService::Ptr find(const std::string& uri, const std::string& protocol);
		/// Return a Proxy for the service object identified by the given URI.
		///
		/// The given protocol name is used to determine the Transport used by
		/// the Proxy. This is used for objects identified by URIs that do not
		/// follow the standard Remoting URI structure.

private:
	ICheckVatService::Ptr findImpl(const std::string& uri);

	ICheckVatService::Ptr findImpl(const std::string& uri, const std::string& protocol);

	static CheckVatServiceClientHelper& instance();

	Poco::RemotingNG::ORB* _pORB;
};


inline ICheckVatService::Ptr CheckVatServiceClientHelper::find(const std::string& uri)
{
	return CheckVatServiceClientHelper::instance().findImpl(uri);
}


inline ICheckVatService::Ptr CheckVatServiceClientHelper::find(const std::string& uri, const std::string& protocol)
{
	return CheckVatServiceClientHelper::instance().findImpl(uri, protocol);
}


} // namespace CheckVat


#endif // CheckVatServiceClientHelper_INCLUDED

