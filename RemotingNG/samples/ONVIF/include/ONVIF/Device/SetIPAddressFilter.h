// 
// SetIPAddressFilter.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_SetIPAddressFilter_INCLUDED
#define ONVIF_Device_SetIPAddressFilter_INCLUDED


#include "ONVIF/IPAddressFilter.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#SetIPAddressFilter"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API SetIPAddressFilter
{
public:
	SetIPAddressFilter();

	SetIPAddressFilter(const ONVIF::IPAddressFilter& iPAddressFilter);

	virtual ~SetIPAddressFilter();

	const ONVIF::IPAddressFilter& getIPAddressFilter() const;

	void setIPAddressFilter(const ONVIF::IPAddressFilter& val);

	void setIPAddressFilter(ONVIF::IPAddressFilter&& val);

private:
	//@ name=IPAddressFilter
	//@ order=0
	ONVIF::IPAddressFilter _iPAddressFilter;

};


inline const ONVIF::IPAddressFilter& SetIPAddressFilter::getIPAddressFilter() const
{
	return _iPAddressFilter;
}


inline void SetIPAddressFilter::setIPAddressFilter(const ONVIF::IPAddressFilter& val)
{
	_iPAddressFilter = val;
}


inline void SetIPAddressFilter::setIPAddressFilter(ONVIF::IPAddressFilter&& val)
{
	_iPAddressFilter = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_SetIPAddressFilter_INCLUDED
