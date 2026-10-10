// 
// GetPasswordComplexityConfigurationResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_GetPasswordComplexityConfigurationResponse_INCLUDED
#define ONVIF_Device_GetPasswordComplexityConfigurationResponse_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#GetPasswordComplexityConfigurationResponse"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API GetPasswordComplexityConfigurationResponse
{
public:
	GetPasswordComplexityConfigurationResponse();

	GetPasswordComplexityConfigurationResponse(
		Poco::Optional<int> minLen, 
		Poco::Optional<int> uppercase, 
		Poco::Optional<int> number, 
		Poco::Optional<int> specialChars, 
		Poco::Optional<bool> blockUsernameOccurrence, 
		Poco::Optional<bool> policyConfigurationLocked);

	virtual ~GetPasswordComplexityConfigurationResponse();

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


inline Poco::Optional<bool> GetPasswordComplexityConfigurationResponse::getBlockUsernameOccurrence() const
{
	return _blockUsernameOccurrence;
}


inline Poco::Optional<int> GetPasswordComplexityConfigurationResponse::getMinLen() const
{
	return _minLen;
}


inline Poco::Optional<int> GetPasswordComplexityConfigurationResponse::getNumber() const
{
	return _number;
}


inline Poco::Optional<bool> GetPasswordComplexityConfigurationResponse::getPolicyConfigurationLocked() const
{
	return _policyConfigurationLocked;
}


inline Poco::Optional<int> GetPasswordComplexityConfigurationResponse::getSpecialChars() const
{
	return _specialChars;
}


inline Poco::Optional<int> GetPasswordComplexityConfigurationResponse::getUppercase() const
{
	return _uppercase;
}


inline void GetPasswordComplexityConfigurationResponse::setBlockUsernameOccurrence(Poco::Optional<bool> val)
{
	_blockUsernameOccurrence = val;
}


inline void GetPasswordComplexityConfigurationResponse::setMinLen(Poco::Optional<int> val)
{
	_minLen = val;
}


inline void GetPasswordComplexityConfigurationResponse::setNumber(Poco::Optional<int> val)
{
	_number = val;
}


inline void GetPasswordComplexityConfigurationResponse::setPolicyConfigurationLocked(Poco::Optional<bool> val)
{
	_policyConfigurationLocked = val;
}


inline void GetPasswordComplexityConfigurationResponse::setSpecialChars(Poco::Optional<int> val)
{
	_specialChars = val;
}


inline void GetPasswordComplexityConfigurationResponse::setUppercase(Poco::Optional<int> val)
{
	_uppercase = val;
}


} } // ONVIF::Device


#endif // ONVIF_Device_GetPasswordComplexityConfigurationResponse_INCLUDED
