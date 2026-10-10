// 
// UpgradeSystemFirmwareResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_UpgradeSystemFirmwareResponse_INCLUDED
#define ONVIF_Device_UpgradeSystemFirmwareResponse_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#UpgradeSystemFirmwareResponse"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API UpgradeSystemFirmwareResponse
{
public:
	UpgradeSystemFirmwareResponse();

	UpgradeSystemFirmwareResponse(const Poco::Optional<std::string>& message);

	virtual ~UpgradeSystemFirmwareResponse();

	const Poco::Optional<std::string>& getMessage() const;

	void setMessage(const Poco::Optional<std::string>& val);

	void setMessage(Poco::Optional<std::string>&& val);

private:
	//@ mandatory=false
	//@ name=Message
	//@ order=0
	Poco::Optional<std::string> _message;

};


inline const Poco::Optional<std::string>& UpgradeSystemFirmwareResponse::getMessage() const
{
	return _message;
}


inline void UpgradeSystemFirmwareResponse::setMessage(const Poco::Optional<std::string>& val)
{
	_message = val;
}


inline void UpgradeSystemFirmwareResponse::setMessage(Poco::Optional<std::string>&& val)
{
	_message = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_UpgradeSystemFirmwareResponse_INCLUDED
