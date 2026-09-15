// 
// GetDot11Status.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_GetDot11Status_INCLUDED
#define ONVIF_Device_GetDot11Status_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#GetDot11Status"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API GetDot11Status
{
public:
	GetDot11Status();

	GetDot11Status(const std::string& interfaceToken);

	virtual ~GetDot11Status();

	const std::string& getInterfaceToken() const;

	void setInterfaceToken(const std::string& val);

	void setInterfaceToken(std::string&& val);

private:
	//@ name=InterfaceToken
	//@ order=0
	std::string _interfaceToken;

};


inline const std::string& GetDot11Status::getInterfaceToken() const
{
	return _interfaceToken;
}


inline void GetDot11Status::setInterfaceToken(const std::string& val)
{
	_interfaceToken = val;
}


inline void GetDot11Status::setInterfaceToken(std::string&& val)
{
	_interfaceToken = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_GetDot11Status_INCLUDED
