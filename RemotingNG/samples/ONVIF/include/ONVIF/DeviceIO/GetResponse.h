// 
// GetResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DeviceIO_GetResponse_INCLUDED
#define ONVIF_DeviceIO_GetResponse_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace DeviceIO {


//@ name=GetResponse
//@ namespace="http://www.onvif.org/ver10/deviceIO/wsdl"
//@ serialize
class ONVIF_API GetResponse
{
public:
	GetResponse();

	GetResponse(const std::vector<std::string>& token);

	virtual ~GetResponse();

	const std::vector<std::string>& getToken() const;

	std::vector<std::string>& getToken();

	void setToken(const std::vector<std::string>& val);

	void setToken(std::vector<std::string>&& val);

private:
	//@ mandatory=false
	//@ name=Token
	//@ order=0
	std::vector<std::string> _token;

};


inline const std::vector<std::string>& GetResponse::getToken() const
{
	return _token;
}


inline std::vector<std::string>& GetResponse::getToken()
{
	return _token;
}


inline void GetResponse::setToken(const std::vector<std::string>& val)
{
	_token = val;
}


inline void GetResponse::setToken(std::vector<std::string>&& val)
{
	_token = std::move(val);
}


} } // ONVIF::DeviceIO


#endif // ONVIF_DeviceIO_GetResponse_INCLUDED
