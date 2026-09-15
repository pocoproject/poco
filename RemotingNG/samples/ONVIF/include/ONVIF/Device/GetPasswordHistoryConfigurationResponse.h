// 
// GetPasswordHistoryConfigurationResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_GetPasswordHistoryConfigurationResponse_INCLUDED
#define ONVIF_Device_GetPasswordHistoryConfigurationResponse_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#GetPasswordHistoryConfigurationResponse"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API GetPasswordHistoryConfigurationResponse
{
public:
	GetPasswordHistoryConfigurationResponse();

	GetPasswordHistoryConfigurationResponse(
		bool enabled, 
		int length);

	virtual ~GetPasswordHistoryConfigurationResponse();

	bool getEnabled() const;

	int getLength() const;

	void setEnabled(bool val);

	void setLength(int val);

private:
	//@ name=Enabled
	//@ order=0
	bool _enabled;

	//@ name=Length
	//@ order=1
	int _length;

};


inline bool GetPasswordHistoryConfigurationResponse::getEnabled() const
{
	return _enabled;
}


inline int GetPasswordHistoryConfigurationResponse::getLength() const
{
	return _length;
}


inline void GetPasswordHistoryConfigurationResponse::setEnabled(bool val)
{
	_enabled = val;
}


inline void GetPasswordHistoryConfigurationResponse::setLength(int val)
{
	_length = val;
}


} } // ONVIF::Device


#endif // ONVIF_Device_GetPasswordHistoryConfigurationResponse_INCLUDED
