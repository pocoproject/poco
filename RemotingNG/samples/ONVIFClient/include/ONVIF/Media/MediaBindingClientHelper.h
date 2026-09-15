//
// MediaBindingClientHelper.h
//
// Package: Generated
// Module:  MediaBindingClientHelper
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef MediaBindingClientHelper_INCLUDED
#define MediaBindingClientHelper_INCLUDED


#include "ONVIF/Media/IMediaBinding.h"
#include "Poco/RemotingNG/Identifiable.h"
#include "Poco/RemotingNG/ORB.h"


namespace ONVIF {
namespace Media {


class MediaBindingClientHelper
{
public:
	MediaBindingClientHelper();
		/// Creates a MediaBindingClientHelper.

	~MediaBindingClientHelper();
		/// Destroys the MediaBindingClientHelper.

	static IMediaBinding::Ptr find(const std::string& uri);
		/// Return an interface for the service object identified by the given URI.
		///
		/// Depending on whether the service object has been registered on the same ORB, or not,
		/// the ORB will either return a RemoteObject (with forwards calls locally, without the
		/// need for serialization/deserialization), or a Proxy.
		///
		/// The URI must have the following format: <scheme>://<authority>/<protocol>/<typeId>/<objectId>

	static IMediaBinding::Ptr find(const std::string& uri, const std::string& protocol);
		/// Return a Proxy for the service object identified by the given URI.
		///
		/// The given protocol name is used to determine the Transport used by
		/// the Proxy. This is used for objects identified by URIs that do not
		/// follow the standard Remoting URI structure.

private:
	IMediaBinding::Ptr findImpl(const std::string& uri);

	IMediaBinding::Ptr findImpl(const std::string& uri, const std::string& protocol);

	static MediaBindingClientHelper& instance();

	Poco::RemotingNG::ORB* _pORB;
};


inline IMediaBinding::Ptr MediaBindingClientHelper::find(const std::string& uri)
{
	return MediaBindingClientHelper::instance().findImpl(uri);
}


inline IMediaBinding::Ptr MediaBindingClientHelper::find(const std::string& uri, const std::string& protocol)
{
	return MediaBindingClientHelper::instance().findImpl(uri, protocol);
}


} // namespace Media
} // namespace ONVIF


#endif // MediaBindingClientHelper_INCLUDED

