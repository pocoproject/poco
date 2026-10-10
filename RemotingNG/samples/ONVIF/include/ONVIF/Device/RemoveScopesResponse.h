// 
// RemoveScopesResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_RemoveScopesResponse_INCLUDED
#define ONVIF_Device_RemoveScopesResponse_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/URI.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#RemoveScopesResponse"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API RemoveScopesResponse
{
public:
	RemoveScopesResponse();

	RemoveScopesResponse(const std::vector<Poco::URI>& scopeItem);

	virtual ~RemoveScopesResponse();

	const std::vector<Poco::URI>& getScopeItem() const;

	std::vector<Poco::URI>& getScopeItem();

	void setScopeItem(const std::vector<Poco::URI>& val);

	void setScopeItem(std::vector<Poco::URI>&& val);

private:
	//@ mandatory=false
	//@ name=ScopeItem
	//@ order=0
	std::vector<Poco::URI> _scopeItem;

};


inline const std::vector<Poco::URI>& RemoveScopesResponse::getScopeItem() const
{
	return _scopeItem;
}


inline std::vector<Poco::URI>& RemoveScopesResponse::getScopeItem()
{
	return _scopeItem;
}


inline void RemoveScopesResponse::setScopeItem(const std::vector<Poco::URI>& val)
{
	_scopeItem = val;
}


inline void RemoveScopesResponse::setScopeItem(std::vector<Poco::URI>&& val)
{
	_scopeItem = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_RemoveScopesResponse_INCLUDED
