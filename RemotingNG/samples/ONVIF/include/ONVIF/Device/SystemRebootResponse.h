// 
// SystemRebootResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_SystemRebootResponse_INCLUDED
#define ONVIF_Device_SystemRebootResponse_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#SystemRebootResponse"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API SystemRebootResponse
{
public:
	SystemRebootResponse();

	SystemRebootResponse(const std::string& message);

	virtual ~SystemRebootResponse();

	const std::string& getMessage() const;

	void setMessage(const std::string& val);

	void setMessage(std::string&& val);

private:
	//@ name=Message
	//@ order=0
	std::string _message;

};


inline const std::string& SystemRebootResponse::getMessage() const
{
	return _message;
}


inline void SystemRebootResponse::setMessage(const std::string& val)
{
	_message = val;
}


inline void SystemRebootResponse::setMessage(std::string&& val)
{
	_message = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_SystemRebootResponse_INCLUDED
