// 
// GetDeviceInformationResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_GetDeviceInformationResponse_INCLUDED
#define ONVIF_Device_GetDeviceInformationResponse_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#GetDeviceInformationResponse"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API GetDeviceInformationResponse
{
public:
	GetDeviceInformationResponse();

	GetDeviceInformationResponse(
		const std::string& manufacturer, 
		const std::string& model, 
		const std::string& firmwareVersion, 
		const std::string& serialNumber, 
		const std::string& hardwareId);

	virtual ~GetDeviceInformationResponse();

	const std::string& getFirmwareVersion() const;

	const std::string& getHardwareId() const;

	const std::string& getManufacturer() const;

	const std::string& getModel() const;

	const std::string& getSerialNumber() const;

	void setFirmwareVersion(const std::string& val);

	void setFirmwareVersion(std::string&& val);

	void setHardwareId(const std::string& val);

	void setHardwareId(std::string&& val);

	void setManufacturer(const std::string& val);

	void setManufacturer(std::string&& val);

	void setModel(const std::string& val);

	void setModel(std::string&& val);

	void setSerialNumber(const std::string& val);

	void setSerialNumber(std::string&& val);

private:
	//@ name=Manufacturer
	//@ order=0
	std::string _manufacturer;

	//@ name=Model
	//@ order=1
	std::string _model;

	//@ name=FirmwareVersion
	//@ order=2
	std::string _firmwareVersion;

	//@ name=SerialNumber
	//@ order=3
	std::string _serialNumber;

	//@ name=HardwareId
	//@ order=4
	std::string _hardwareId;

};


inline const std::string& GetDeviceInformationResponse::getFirmwareVersion() const
{
	return _firmwareVersion;
}


inline const std::string& GetDeviceInformationResponse::getHardwareId() const
{
	return _hardwareId;
}


inline const std::string& GetDeviceInformationResponse::getManufacturer() const
{
	return _manufacturer;
}


inline const std::string& GetDeviceInformationResponse::getModel() const
{
	return _model;
}


inline const std::string& GetDeviceInformationResponse::getSerialNumber() const
{
	return _serialNumber;
}


inline void GetDeviceInformationResponse::setFirmwareVersion(const std::string& val)
{
	_firmwareVersion = val;
}


inline void GetDeviceInformationResponse::setFirmwareVersion(std::string&& val)
{
	_firmwareVersion = std::move(val);
}


inline void GetDeviceInformationResponse::setHardwareId(const std::string& val)
{
	_hardwareId = val;
}


inline void GetDeviceInformationResponse::setHardwareId(std::string&& val)
{
	_hardwareId = std::move(val);
}


inline void GetDeviceInformationResponse::setManufacturer(const std::string& val)
{
	_manufacturer = val;
}


inline void GetDeviceInformationResponse::setManufacturer(std::string&& val)
{
	_manufacturer = std::move(val);
}


inline void GetDeviceInformationResponse::setModel(const std::string& val)
{
	_model = val;
}


inline void GetDeviceInformationResponse::setModel(std::string&& val)
{
	_model = std::move(val);
}


inline void GetDeviceInformationResponse::setSerialNumber(const std::string& val)
{
	_serialNumber = val;
}


inline void GetDeviceInformationResponse::setSerialNumber(std::string&& val)
{
	_serialNumber = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_GetDeviceInformationResponse_INCLUDED
