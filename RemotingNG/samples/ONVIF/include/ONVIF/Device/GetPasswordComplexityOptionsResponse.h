// 
// GetPasswordComplexityOptionsResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_GetPasswordComplexityOptionsResponse_INCLUDED
#define ONVIF_Device_GetPasswordComplexityOptionsResponse_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class IntRange;
} 


namespace ONVIF {
namespace Device {


//@ name="#GetPasswordComplexityOptionsResponse"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API GetPasswordComplexityOptionsResponse
{
public:
	GetPasswordComplexityOptionsResponse();

	GetPasswordComplexityOptionsResponse(
		const Poco::SharedPtr<ONVIF::IntRange>& minLenRange, 
		const Poco::SharedPtr<ONVIF::IntRange>& uppercaseRange, 
		const Poco::SharedPtr<ONVIF::IntRange>& numberRange, 
		const Poco::SharedPtr<ONVIF::IntRange>& specialCharsRange, 
		Poco::Optional<bool> blockUsernameOccurrenceSupported, 
		Poco::Optional<bool> policyConfigurationLockSupported);

	virtual ~GetPasswordComplexityOptionsResponse();

	Poco::Optional<bool> getBlockUsernameOccurrenceSupported() const;

	const Poco::SharedPtr<ONVIF::IntRange>& getMinLenRange() const;

	const Poco::SharedPtr<ONVIF::IntRange>& getNumberRange() const;

	Poco::Optional<bool> getPolicyConfigurationLockSupported() const;

	const Poco::SharedPtr<ONVIF::IntRange>& getSpecialCharsRange() const;

	const Poco::SharedPtr<ONVIF::IntRange>& getUppercaseRange() const;

	void setBlockUsernameOccurrenceSupported(Poco::Optional<bool> val);

	void setMinLenRange(const Poco::SharedPtr<ONVIF::IntRange>& val);

	void setMinLenRange(Poco::SharedPtr<ONVIF::IntRange>&& val);

	void setNumberRange(const Poco::SharedPtr<ONVIF::IntRange>& val);

	void setNumberRange(Poco::SharedPtr<ONVIF::IntRange>&& val);

	void setPolicyConfigurationLockSupported(Poco::Optional<bool> val);

	void setSpecialCharsRange(const Poco::SharedPtr<ONVIF::IntRange>& val);

	void setSpecialCharsRange(Poco::SharedPtr<ONVIF::IntRange>&& val);

	void setUppercaseRange(const Poco::SharedPtr<ONVIF::IntRange>& val);

	void setUppercaseRange(Poco::SharedPtr<ONVIF::IntRange>&& val);

private:
	//@ mandatory=false
	//@ name=MinLenRange
	//@ order=0
	Poco::SharedPtr<ONVIF::IntRange> _minLenRange;

	//@ mandatory=false
	//@ name=UppercaseRange
	//@ order=1
	Poco::SharedPtr<ONVIF::IntRange> _uppercaseRange;

	//@ mandatory=false
	//@ name=NumberRange
	//@ order=2
	Poco::SharedPtr<ONVIF::IntRange> _numberRange;

	//@ mandatory=false
	//@ name=SpecialCharsRange
	//@ order=3
	Poco::SharedPtr<ONVIF::IntRange> _specialCharsRange;

	//@ mandatory=false
	//@ name=BlockUsernameOccurrenceSupported
	//@ order=4
	Poco::Optional<bool> _blockUsernameOccurrenceSupported;

	//@ mandatory=false
	//@ name=PolicyConfigurationLockSupported
	//@ order=5
	Poco::Optional<bool> _policyConfigurationLockSupported;

};


inline Poco::Optional<bool> GetPasswordComplexityOptionsResponse::getBlockUsernameOccurrenceSupported() const
{
	return _blockUsernameOccurrenceSupported;
}


inline const Poco::SharedPtr<ONVIF::IntRange>& GetPasswordComplexityOptionsResponse::getMinLenRange() const
{
	return _minLenRange;
}


inline const Poco::SharedPtr<ONVIF::IntRange>& GetPasswordComplexityOptionsResponse::getNumberRange() const
{
	return _numberRange;
}


inline Poco::Optional<bool> GetPasswordComplexityOptionsResponse::getPolicyConfigurationLockSupported() const
{
	return _policyConfigurationLockSupported;
}


inline const Poco::SharedPtr<ONVIF::IntRange>& GetPasswordComplexityOptionsResponse::getSpecialCharsRange() const
{
	return _specialCharsRange;
}


inline const Poco::SharedPtr<ONVIF::IntRange>& GetPasswordComplexityOptionsResponse::getUppercaseRange() const
{
	return _uppercaseRange;
}


inline void GetPasswordComplexityOptionsResponse::setBlockUsernameOccurrenceSupported(Poco::Optional<bool> val)
{
	_blockUsernameOccurrenceSupported = val;
}


inline void GetPasswordComplexityOptionsResponse::setMinLenRange(const Poco::SharedPtr<ONVIF::IntRange>& val)
{
	_minLenRange = val;
}


inline void GetPasswordComplexityOptionsResponse::setMinLenRange(Poco::SharedPtr<ONVIF::IntRange>&& val)
{
	_minLenRange = std::move(val);
}


inline void GetPasswordComplexityOptionsResponse::setNumberRange(const Poco::SharedPtr<ONVIF::IntRange>& val)
{
	_numberRange = val;
}


inline void GetPasswordComplexityOptionsResponse::setNumberRange(Poco::SharedPtr<ONVIF::IntRange>&& val)
{
	_numberRange = std::move(val);
}


inline void GetPasswordComplexityOptionsResponse::setPolicyConfigurationLockSupported(Poco::Optional<bool> val)
{
	_policyConfigurationLockSupported = val;
}


inline void GetPasswordComplexityOptionsResponse::setSpecialCharsRange(const Poco::SharedPtr<ONVIF::IntRange>& val)
{
	_specialCharsRange = val;
}


inline void GetPasswordComplexityOptionsResponse::setSpecialCharsRange(Poco::SharedPtr<ONVIF::IntRange>&& val)
{
	_specialCharsRange = std::move(val);
}


inline void GetPasswordComplexityOptionsResponse::setUppercaseRange(const Poco::SharedPtr<ONVIF::IntRange>& val)
{
	_uppercaseRange = val;
}


inline void GetPasswordComplexityOptionsResponse::setUppercaseRange(Poco::SharedPtr<ONVIF::IntRange>&& val)
{
	_uppercaseRange = std::move(val);
}


} } // ONVIF::Device


// The following headers are required for template instantiation.
#include "ONVIF/IntRange.h"


#endif // ONVIF_Device_GetPasswordComplexityOptionsResponse_INCLUDED
