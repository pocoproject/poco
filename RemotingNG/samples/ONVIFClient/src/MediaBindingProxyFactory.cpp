//
// MediaBindingProxyFactory.cpp
//
// Package: Generated
// Module:  MediaBindingProxyFactory
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#include "ONVIF/Media/MediaBindingProxyFactory.h"


namespace ONVIF {
namespace Media {


MediaBindingProxyFactory::MediaBindingProxyFactory():
	Poco::RemotingNG::ProxyFactory()

{
}


MediaBindingProxyFactory::~MediaBindingProxyFactory()
{
}


Poco::RemotingNG::Proxy* MediaBindingProxyFactory::createProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid) const
{
	return new ONVIF::Media::MediaBindingProxy(oid);
}


} // namespace Media
} // namespace ONVIF

