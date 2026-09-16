// 
// RemoveIPAddressFilter.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_RemoveIPAddressFilter_INCLUDED
#define ONVIF_Device_RemoveIPAddressFilter_INCLUDED


#include "ONVIF/IPAddressFilter.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#RemoveIPAddressFilter"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API RemoveIPAddressFilter
{
public:
	RemoveIPAddressFilter();

	RemoveIPAddressFilter(const ONVIF::IPAddressFilter& iPAddressFilter);

	virtual ~RemoveIPAddressFilter();

	const ONVIF::IPAddressFilter& getIPAddressFilter() const;

	void setIPAddressFilter(const ONVIF::IPAddressFilter& val);

	void setIPAddressFilter(ONVIF::IPAddressFilter&& val);

private:
	//@ name=IPAddressFilter
	//@ order=0
	ONVIF::IPAddressFilter _iPAddressFilter;

};


inline const ONVIF::IPAddressFilter& RemoveIPAddressFilter::getIPAddressFilter() const
{
	return _iPAddressFilter;
}


inline void RemoveIPAddressFilter::setIPAddressFilter(const ONVIF::IPAddressFilter& val)
{
	_iPAddressFilter = val;
}


inline void RemoveIPAddressFilter::setIPAddressFilter(ONVIF::IPAddressFilter&& val)
{
	_iPAddressFilter = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_RemoveIPAddressFilter_INCLUDED
