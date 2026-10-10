// 
// SetScopes.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_SetScopes_INCLUDED
#define ONVIF_Device_SetScopes_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/URI.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#SetScopes"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API SetScopes
{
public:
	SetScopes();

	SetScopes(const std::vector<Poco::URI>& scopes);

	virtual ~SetScopes();

	const std::vector<Poco::URI>& getScopes() const;

	std::vector<Poco::URI>& getScopes();

	void setScopes(const std::vector<Poco::URI>& val);

	void setScopes(std::vector<Poco::URI>&& val);

private:
	//@ name=Scopes
	//@ order=0
	std::vector<Poco::URI> _scopes;

};


inline const std::vector<Poco::URI>& SetScopes::getScopes() const
{
	return _scopes;
}


inline std::vector<Poco::URI>& SetScopes::getScopes()
{
	return _scopes;
}


inline void SetScopes::setScopes(const std::vector<Poco::URI>& val)
{
	_scopes = val;
}


inline void SetScopes::setScopes(std::vector<Poco::URI>&& val)
{
	_scopes = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_SetScopes_INCLUDED
