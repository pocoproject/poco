// 
// GetEndpointReferenceResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_GetEndpointReferenceResponse_INCLUDED
#define ONVIF_Device_GetEndpointReferenceResponse_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#GetEndpointReferenceResponse"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API GetEndpointReferenceResponse
{
public:
	GetEndpointReferenceResponse();

	GetEndpointReferenceResponse(const std::string& gUID);

	virtual ~GetEndpointReferenceResponse();

	const std::string& getGUID() const;

	void setGUID(const std::string& val);

	void setGUID(std::string&& val);

private:
	//@ name=GUID
	//@ order=0
	std::string _gUID;

};


inline const std::string& GetEndpointReferenceResponse::getGUID() const
{
	return _gUID;
}


inline void GetEndpointReferenceResponse::setGUID(const std::string& val)
{
	_gUID = val;
}


inline void GetEndpointReferenceResponse::setGUID(std::string&& val)
{
	_gUID = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_GetEndpointReferenceResponse_INCLUDED
