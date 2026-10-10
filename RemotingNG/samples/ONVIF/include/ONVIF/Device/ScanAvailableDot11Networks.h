// 
// ScanAvailableDot11Networks.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_ScanAvailableDot11Networks_INCLUDED
#define ONVIF_Device_ScanAvailableDot11Networks_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#ScanAvailableDot11Networks"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API ScanAvailableDot11Networks
{
public:
	ScanAvailableDot11Networks();

	ScanAvailableDot11Networks(const std::string& interfaceToken);

	virtual ~ScanAvailableDot11Networks();

	const std::string& getInterfaceToken() const;

	void setInterfaceToken(const std::string& val);

	void setInterfaceToken(std::string&& val);

private:
	//@ name=InterfaceToken
	//@ order=0
	std::string _interfaceToken;

};


inline const std::string& ScanAvailableDot11Networks::getInterfaceToken() const
{
	return _interfaceToken;
}


inline void ScanAvailableDot11Networks::setInterfaceToken(const std::string& val)
{
	_interfaceToken = val;
}


inline void ScanAvailableDot11Networks::setInterfaceToken(std::string&& val)
{
	_interfaceToken = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_ScanAvailableDot11Networks_INCLUDED
