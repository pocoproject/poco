// 
// SetPasswordComplexityConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_SetPasswordComplexityConfiguration_INCLUDED
#define ONVIF_Device_SetPasswordComplexityConfiguration_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#SetPasswordComplexityConfiguration"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API SetPasswordComplexityConfiguration
{
public:
	SetPasswordComplexityConfiguration();

	SetPasswordComplexityConfiguration(
		Poco::Optional<int> minLen, 
		Poco::Optional<int> uppercase, 
		Poco::Optional<int> number, 
		Poco::Optional<int> specialChars, 
		Poco::Optional<bool> blockUsernameOccurrence, 
		Poco::Optional<bool> policyConfigurationLocked);

	virtual ~SetPasswordComplexityConfiguration();

	Poco::Optional<bool> getBlockUsernameOccurrence() const;

	Poco::Optional<int> getMinLen() const;

	Poco::Optional<int> getNumber() const;

	Poco::Optional<bool> getPolicyConfigurationLocked() const;

	Poco::Optional<int> getSpecialChars() const;

	Poco::Optional<int> getUppercase() const;

	void setBlockUsernameOccurrence(Poco::Optional<bool> val);

	void setMinLen(Poco::Optional<int> val);

	void setNumber(Poco::Optional<int> val);

	void setPolicyConfigurationLocked(Poco::Optional<bool> val);

	void setSpecialChars(Poco::Optional<int> val);

	void setUppercase(Poco::Optional<int> val);

private:
	//@ mandatory=false
	//@ name=MinLen
	//@ order=0
	Poco::Optional<int> _minLen;

	//@ mandatory=false
	//@ name=Uppercase
	//@ order=1
	Poco::Optional<int> _uppercase;

	//@ mandatory=false
	//@ name=Number
	//@ order=2
	Poco::Optional<int> _number;

	//@ mandatory=false
	//@ name=SpecialChars
	//@ order=3
	Poco::Optional<int> _specialChars;

	//@ mandatory=false
	//@ name=BlockUsernameOccurrence
	//@ order=4
	Poco::Optional<bool> _blockUsernameOccurrence;

	//@ mandatory=false
	//@ name=PolicyConfigurationLocked
	//@ order=5
	Poco::Optional<bool> _policyConfigurationLocked;

};


inline Poco::Optional<bool> SetPasswordComplexityConfiguration::getBlockUsernameOccurrence() const
{
	return _blockUsernameOccurrence;
}


inline Poco::Optional<int> SetPasswordComplexityConfiguration::getMinLen() const
{
	return _minLen;
}


inline Poco::Optional<int> SetPasswordComplexityConfiguration::getNumber() const
{
	return _number;
}


inline Poco::Optional<bool> SetPasswordComplexityConfiguration::getPolicyConfigurationLocked() const
{
	return _policyConfigurationLocked;
}


inline Poco::Optional<int> SetPasswordComplexityConfiguration::getSpecialChars() const
{
	return _specialChars;
}


inline Poco::Optional<int> SetPasswordComplexityConfiguration::getUppercase() const
{
	return _uppercase;
}


inline void SetPasswordComplexityConfiguration::setBlockUsernameOccurrence(Poco::Optional<bool> val)
{
	_blockUsernameOccurrence = val;
}


inline void SetPasswordComplexityConfiguration::setMinLen(Poco::Optional<int> val)
{
	_minLen = val;
}


inline void SetPasswordComplexityConfiguration::setNumber(Poco::Optional<int> val)
{
	_number = val;
}


inline void SetPasswordComplexityConfiguration::setPolicyConfigurationLocked(Poco::Optional<bool> val)
{
	_policyConfigurationLocked = val;
}


inline void SetPasswordComplexityConfiguration::setSpecialChars(Poco::Optional<int> val)
{
	_specialChars = val;
}


inline void SetPasswordComplexityConfiguration::setUppercase(Poco::Optional<int> val)
{
	_uppercase = val;
}


} } // ONVIF::Device


#endif // ONVIF_Device_SetPasswordComplexityConfiguration_INCLUDED
