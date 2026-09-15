//
// IDeviceIOBinding.cpp
//
// Package: Generated
// Module:  IDeviceIOBinding
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#include "ONVIF/DeviceIO/IDeviceIOBinding.h"


namespace ONVIF {
namespace DeviceIO {


IDeviceIOBinding::IDeviceIOBinding():
	Poco::RefCountedObject()

{
}


IDeviceIOBinding::~IDeviceIOBinding()
{
}


const Poco::RemotingNG::Identifiable::TypeId& IDeviceIOBinding::remoting__typeId()
{
	static const std::string REMOTING__TYPE_ID("ONVIF.DeviceIO.DeviceIOBinding");
	return REMOTING__TYPE_ID;
}


} // namespace DeviceIO
} // namespace ONVIF

