//
// MediaBindingClientHelper.cpp
//
// Package: Generated
// Module:  MediaBindingClientHelper
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#include "ONVIF/Media/MediaBindingClientHelper.h"
#include "ONVIF/Media/MediaBindingProxyFactory.h"
#include "Poco/SingletonHolder.h"


namespace ONVIF {
namespace Media {


namespace
{
	Poco::SingletonHolder<MediaBindingClientHelper> shMediaBindingClientHelper;
}


MediaBindingClientHelper::MediaBindingClientHelper():
	_pORB(0)
{
	_pORB = &Poco::RemotingNG::ORB::instance();
	_pORB->registerProxyFactory("ONVIF.Media.MediaBinding", new MediaBindingProxyFactory);
}


MediaBindingClientHelper::~MediaBindingClientHelper()
{
}


IMediaBinding::Ptr MediaBindingClientHelper::findImpl(const std::string& uri)
{
	Poco::RemotingNG::Identifiable::Ptr pInterface = _pORB->findObject(uri);
	return pInterface.cast<IMediaBinding>();
}


IMediaBinding::Ptr MediaBindingClientHelper::findImpl(const std::string& uri, const std::string& protocol)
{
	Poco::RemotingNG::Identifiable::Ptr pInterface = _pORB->findObject(uri, IMediaBinding::remoting__typeId(), protocol);
	return pInterface.cast<IMediaBinding>();
}


MediaBindingClientHelper& MediaBindingClientHelper::instance()
{
	return *shMediaBindingClientHelper.get();
}


} // namespace Media
} // namespace ONVIF

