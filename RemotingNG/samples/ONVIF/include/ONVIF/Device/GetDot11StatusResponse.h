// 
// GetDot11StatusResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_GetDot11StatusResponse_INCLUDED
#define ONVIF_Device_GetDot11StatusResponse_INCLUDED


#include "ONVIF/Dot11Status.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#GetDot11StatusResponse"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API GetDot11StatusResponse
{
public:
	GetDot11StatusResponse();

	GetDot11StatusResponse(const ONVIF::Dot11Status& status);

	virtual ~GetDot11StatusResponse();

	const ONVIF::Dot11Status& getStatus() const;

	void setStatus(const ONVIF::Dot11Status& val);

	void setStatus(ONVIF::Dot11Status&& val);

private:
	//@ name=Status
	//@ order=0
	ONVIF::Dot11Status _status;

};


inline const ONVIF::Dot11Status& GetDot11StatusResponse::getStatus() const
{
	return _status;
}


inline void GetDot11StatusResponse::setStatus(const ONVIF::Dot11Status& val)
{
	_status = val;
}


inline void GetDot11StatusResponse::setStatus(ONVIF::Dot11Status&& val)
{
	_status = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_GetDot11StatusResponse_INCLUDED
