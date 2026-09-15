//
// IMediaBinding.cpp
//
// Package: Generated
// Module:  IMediaBinding
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#include "ONVIF/Media/IMediaBinding.h"


namespace ONVIF {
namespace Media {


IMediaBinding::IMediaBinding():
	Poco::RefCountedObject()

{
}


IMediaBinding::~IMediaBinding()
{
}


const Poco::RemotingNG::Identifiable::TypeId& IMediaBinding::remoting__typeId()
{
	static const std::string REMOTING__TYPE_ID("ONVIF.Media.MediaBinding");
	return REMOTING__TYPE_ID;
}


} // namespace Media
} // namespace ONVIF

