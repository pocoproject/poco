// 
// GetScopesResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_GetScopesResponse_INCLUDED
#define ONVIF_Device_GetScopesResponse_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/Scope.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#GetScopesResponse"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API GetScopesResponse
{
public:
	GetScopesResponse();

	GetScopesResponse(const std::vector<ONVIF::Scope>& scopes);

	virtual ~GetScopesResponse();

	const std::vector<ONVIF::Scope>& getScopes() const;

	std::vector<ONVIF::Scope>& getScopes();

	void setScopes(const std::vector<ONVIF::Scope>& val);

	void setScopes(std::vector<ONVIF::Scope>&& val);

private:
	//@ name=Scopes
	//@ order=0
	std::vector<ONVIF::Scope> _scopes;

};


inline const std::vector<ONVIF::Scope>& GetScopesResponse::getScopes() const
{
	return _scopes;
}


inline std::vector<ONVIF::Scope>& GetScopesResponse::getScopes()
{
	return _scopes;
}


inline void GetScopesResponse::setScopes(const std::vector<ONVIF::Scope>& val)
{
	_scopes = val;
}


inline void GetScopesResponse::setScopes(std::vector<ONVIF::Scope>&& val)
{
	_scopes = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_GetScopesResponse_INCLUDED
