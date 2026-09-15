//
// MediaBindingProxyFactory.h
//
// Package: Generated
// Module:  MediaBindingProxyFactory
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef MediaBindingProxyFactory_INCLUDED
#define MediaBindingProxyFactory_INCLUDED


#include "ONVIF/Media/MediaBindingProxy.h"
#include "Poco/RemotingNG/ProxyFactory.h"


namespace ONVIF {
namespace Media {


class MediaBindingProxyFactory: public Poco::RemotingNG::ProxyFactory
{
public:
	MediaBindingProxyFactory();
		/// Creates a MediaBindingProxyFactory.

	~MediaBindingProxyFactory();
		/// Destroys the MediaBindingProxyFactory.

	Poco::RemotingNG::Proxy* createProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid) const;
		/// Creates and returns a new Proxy object.

};


} // namespace Media
} // namespace ONVIF


#endif // MediaBindingProxyFactory_INCLUDED

