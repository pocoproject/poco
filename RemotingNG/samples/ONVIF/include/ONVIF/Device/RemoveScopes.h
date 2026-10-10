// 
// RemoveScopes.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_RemoveScopes_INCLUDED
#define ONVIF_Device_RemoveScopes_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/URI.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#RemoveScopes"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API RemoveScopes
{
public:
	RemoveScopes();

	RemoveScopes(const std::vector<Poco::URI>& scopeItem);

	virtual ~RemoveScopes();

	const std::vector<Poco::URI>& getScopeItem() const;

	std::vector<Poco::URI>& getScopeItem();

	void setScopeItem(const std::vector<Poco::URI>& val);

	void setScopeItem(std::vector<Poco::URI>&& val);

private:
	//@ name=ScopeItem
	//@ order=0
	std::vector<Poco::URI> _scopeItem;

};


inline const std::vector<Poco::URI>& RemoveScopes::getScopeItem() const
{
	return _scopeItem;
}


inline std::vector<Poco::URI>& RemoveScopes::getScopeItem()
{
	return _scopeItem;
}


inline void RemoveScopes::setScopeItem(const std::vector<Poco::URI>& val)
{
	_scopeItem = val;
}


inline void RemoveScopes::setScopeItem(std::vector<Poco::URI>&& val)
{
	_scopeItem = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_RemoveScopes_INCLUDED
