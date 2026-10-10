// 
// GetNTPResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_GetNTPResponse_INCLUDED
#define ONVIF_Device_GetNTPResponse_INCLUDED


#include "ONVIF/NTPInformation.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#GetNTPResponse"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API GetNTPResponse
{
public:
	GetNTPResponse();

	GetNTPResponse(const ONVIF::NTPInformation& nTPInformation);

	virtual ~GetNTPResponse();

	const ONVIF::NTPInformation& getNTPInformation() const;

	void setNTPInformation(const ONVIF::NTPInformation& val);

	void setNTPInformation(ONVIF::NTPInformation&& val);

private:
	//@ name=NTPInformation
	//@ order=0
	ONVIF::NTPInformation _nTPInformation;

};


inline const ONVIF::NTPInformation& GetNTPResponse::getNTPInformation() const
{
	return _nTPInformation;
}


inline void GetNTPResponse::setNTPInformation(const ONVIF::NTPInformation& val)
{
	_nTPInformation = val;
}


inline void GetNTPResponse::setNTPInformation(ONVIF::NTPInformation&& val)
{
	_nTPInformation = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_GetNTPResponse_INCLUDED
