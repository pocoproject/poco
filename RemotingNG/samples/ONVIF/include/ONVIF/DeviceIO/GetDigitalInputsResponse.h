// 
// GetDigitalInputsResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DeviceIO_GetDigitalInputsResponse_INCLUDED
#define ONVIF_DeviceIO_GetDigitalInputsResponse_INCLUDED


#include "ONVIF/DigitalInput.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace DeviceIO {


//@ name="#GetDigitalInputsResponse"
//@ namespace="http://www.onvif.org/ver10/deviceIO/wsdl"
//@ serialize
class ONVIF_API GetDigitalInputsResponse
{
public:
	GetDigitalInputsResponse();

	GetDigitalInputsResponse(const std::vector<ONVIF::DigitalInput>& digitalInputs);

	virtual ~GetDigitalInputsResponse();

	const std::vector<ONVIF::DigitalInput>& getDigitalInputs() const;

	std::vector<ONVIF::DigitalInput>& getDigitalInputs();

	void setDigitalInputs(const std::vector<ONVIF::DigitalInput>& val);

	void setDigitalInputs(std::vector<ONVIF::DigitalInput>&& val);

private:
	//@ mandatory=false
	//@ name=DigitalInputs
	//@ order=0
	std::vector<ONVIF::DigitalInput> _digitalInputs;

};


inline const std::vector<ONVIF::DigitalInput>& GetDigitalInputsResponse::getDigitalInputs() const
{
	return _digitalInputs;
}


inline std::vector<ONVIF::DigitalInput>& GetDigitalInputsResponse::getDigitalInputs()
{
	return _digitalInputs;
}


inline void GetDigitalInputsResponse::setDigitalInputs(const std::vector<ONVIF::DigitalInput>& val)
{
	_digitalInputs = val;
}


inline void GetDigitalInputsResponse::setDigitalInputs(std::vector<ONVIF::DigitalInput>&& val)
{
	_digitalInputs = std::move(val);
}


} } // ONVIF::DeviceIO


#endif // ONVIF_DeviceIO_GetDigitalInputsResponse_INCLUDED
