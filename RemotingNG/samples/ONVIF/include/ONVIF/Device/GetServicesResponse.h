// 
// GetServicesResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_GetServicesResponse_INCLUDED
#define ONVIF_Device_GetServicesResponse_INCLUDED


#include "ONVIF/Device/Service.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#GetServicesResponse"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API GetServicesResponse
{
public:
	GetServicesResponse();

	GetServicesResponse(const std::vector<Service>& service);

	virtual ~GetServicesResponse();

	const std::vector<Service>& getService() const;

	std::vector<Service>& getService();

	void setService(const std::vector<Service>& val);

	void setService(std::vector<Service>&& val);

private:
	//@ name=Service
	//@ order=0
	std::vector<Service> _service;

};


inline const std::vector<Service>& GetServicesResponse::getService() const
{
	return _service;
}


inline std::vector<Service>& GetServicesResponse::getService()
{
	return _service;
}


inline void GetServicesResponse::setService(const std::vector<Service>& val)
{
	_service = val;
}


inline void GetServicesResponse::setService(std::vector<Service>&& val)
{
	_service = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_GetServicesResponse_INCLUDED
