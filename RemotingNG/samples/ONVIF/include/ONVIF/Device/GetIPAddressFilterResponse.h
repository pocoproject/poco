// 
// GetIPAddressFilterResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_GetIPAddressFilterResponse_INCLUDED
#define ONVIF_Device_GetIPAddressFilterResponse_INCLUDED


#include "ONVIF/IPAddressFilter.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#GetIPAddressFilterResponse"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API GetIPAddressFilterResponse
{
public:
	GetIPAddressFilterResponse();

	GetIPAddressFilterResponse(const ONVIF::IPAddressFilter& iPAddressFilter);

	virtual ~GetIPAddressFilterResponse();

	const ONVIF::IPAddressFilter& getIPAddressFilter() const;

	void setIPAddressFilter(const ONVIF::IPAddressFilter& val);

	void setIPAddressFilter(ONVIF::IPAddressFilter&& val);

private:
	//@ name=IPAddressFilter
	//@ order=0
	ONVIF::IPAddressFilter _iPAddressFilter;

};


inline const ONVIF::IPAddressFilter& GetIPAddressFilterResponse::getIPAddressFilter() const
{
	return _iPAddressFilter;
}


inline void GetIPAddressFilterResponse::setIPAddressFilter(const ONVIF::IPAddressFilter& val)
{
	_iPAddressFilter = val;
}


inline void GetIPAddressFilterResponse::setIPAddressFilter(ONVIF::IPAddressFilter&& val)
{
	_iPAddressFilter = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_GetIPAddressFilterResponse_INCLUDED
