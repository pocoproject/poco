// 
// GetWsdlUrlResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_GetWsdlUrlResponse_INCLUDED
#define ONVIF_Device_GetWsdlUrlResponse_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/URI.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#GetWsdlUrlResponse"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API GetWsdlUrlResponse
{
public:
	GetWsdlUrlResponse();

	GetWsdlUrlResponse(const Poco::URI& wsdlUrl);

	virtual ~GetWsdlUrlResponse();

	const Poco::URI& getWsdlUrl() const;

	void setWsdlUrl(const Poco::URI& val);

	void setWsdlUrl(Poco::URI&& val);

private:
	//@ name=WsdlUrl
	//@ order=0
	Poco::URI _wsdlUrl;

};


inline const Poco::URI& GetWsdlUrlResponse::getWsdlUrl() const
{
	return _wsdlUrl;
}


inline void GetWsdlUrlResponse::setWsdlUrl(const Poco::URI& val)
{
	_wsdlUrl = val;
}


inline void GetWsdlUrlResponse::setWsdlUrl(Poco::URI&& val)
{
	_wsdlUrl = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_GetWsdlUrlResponse_INCLUDED
