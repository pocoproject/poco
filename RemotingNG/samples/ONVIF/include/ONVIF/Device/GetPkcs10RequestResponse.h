// 
// GetPkcs10RequestResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_GetPkcs10RequestResponse_INCLUDED
#define ONVIF_Device_GetPkcs10RequestResponse_INCLUDED


#include "ONVIF/BinaryData.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#GetPkcs10RequestResponse"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API GetPkcs10RequestResponse
{
public:
	GetPkcs10RequestResponse();

	GetPkcs10RequestResponse(const ONVIF::BinaryData& pkcs10Request);

	virtual ~GetPkcs10RequestResponse();

	const ONVIF::BinaryData& getPkcs10Request() const;

	void setPkcs10Request(const ONVIF::BinaryData& val);

	void setPkcs10Request(ONVIF::BinaryData&& val);

private:
	//@ name=Pkcs10Request
	//@ order=0
	ONVIF::BinaryData _pkcs10Request;

};


inline const ONVIF::BinaryData& GetPkcs10RequestResponse::getPkcs10Request() const
{
	return _pkcs10Request;
}


inline void GetPkcs10RequestResponse::setPkcs10Request(const ONVIF::BinaryData& val)
{
	_pkcs10Request = val;
}


inline void GetPkcs10RequestResponse::setPkcs10Request(ONVIF::BinaryData&& val)
{
	_pkcs10Request = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_GetPkcs10RequestResponse_INCLUDED
