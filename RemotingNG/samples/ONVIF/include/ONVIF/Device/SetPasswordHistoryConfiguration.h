// 
// SetPasswordHistoryConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_SetPasswordHistoryConfiguration_INCLUDED
#define ONVIF_Device_SetPasswordHistoryConfiguration_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#SetPasswordHistoryConfiguration"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API SetPasswordHistoryConfiguration
{
public:
	SetPasswordHistoryConfiguration();

	SetPasswordHistoryConfiguration(
		bool enabled, 
		int length);

	virtual ~SetPasswordHistoryConfiguration();

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


inline bool SetPasswordHistoryConfiguration::getEnabled() const
{
	return _enabled;
}


inline int SetPasswordHistoryConfiguration::getLength() const
{
	return _length;
}


inline void SetPasswordHistoryConfiguration::setEnabled(bool val)
{
	_enabled = val;
}


inline void SetPasswordHistoryConfiguration::setLength(int val)
{
	_length = val;
}


} } // ONVIF::Device


#endif // ONVIF_Device_SetPasswordHistoryConfiguration_INCLUDED
