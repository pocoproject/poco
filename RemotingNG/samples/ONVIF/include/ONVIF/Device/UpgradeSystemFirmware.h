// 
// UpgradeSystemFirmware.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_UpgradeSystemFirmware_INCLUDED
#define ONVIF_Device_UpgradeSystemFirmware_INCLUDED


#include "ONVIF/AttachmentData.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#UpgradeSystemFirmware"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API UpgradeSystemFirmware
{
public:
	UpgradeSystemFirmware();

	UpgradeSystemFirmware(const ONVIF::AttachmentData& firmware);

	virtual ~UpgradeSystemFirmware();

	const ONVIF::AttachmentData& getFirmware() const;

	void setFirmware(const ONVIF::AttachmentData& val);

	void setFirmware(ONVIF::AttachmentData&& val);

private:
	//@ name=Firmware
	//@ order=0
	ONVIF::AttachmentData _firmware;

};


inline const ONVIF::AttachmentData& UpgradeSystemFirmware::getFirmware() const
{
	return _firmware;
}


inline void UpgradeSystemFirmware::setFirmware(const ONVIF::AttachmentData& val)
{
	_firmware = val;
}


inline void UpgradeSystemFirmware::setFirmware(ONVIF::AttachmentData&& val)
{
	_firmware = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_UpgradeSystemFirmware_INCLUDED
