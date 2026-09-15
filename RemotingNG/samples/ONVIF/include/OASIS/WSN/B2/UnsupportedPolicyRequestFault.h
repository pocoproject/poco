// 
// UnsupportedPolicyRequestFault.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef OASIS_WSN_B2_UnsupportedPolicyRequestFault_INCLUDED
#define OASIS_WSN_B2_UnsupportedPolicyRequestFault_INCLUDED


#include "OASIS/WSRF/BF2/BaseFault.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace OASIS {
namespace WSN {
namespace B2 {


//@ name=UnsupportedPolicyRequestFaultType
//@ namespace="http://docs.oasis-open.org/wsn/b-2"
//@ serialize
class ONVIF_API UnsupportedPolicyRequestFault: public OASIS::WSRF::BF2::BaseFault
{
public:
	UnsupportedPolicyRequestFault();

	UnsupportedPolicyRequestFault(
		const Poco::DateTime& timestamp, 
		const Poco::SharedPtr<W3::Addressing::EndpointReference>& originator, 
		const Poco::SharedPtr<OASIS::WSRF::BF2::ErrorCode>& errorCode, 
		const std::vector<OASIS::WSRF::BF2::Description>& description, 
		const Poco::SharedPtr<OASIS::WSRF::BF2::FaultCause>& faultCause, 
		const std::vector<std::string>& unsupportedPolicy);

	virtual ~UnsupportedPolicyRequestFault();

	const std::vector<std::string>& getUnsupportedPolicy() const;

	std::vector<std::string>& getUnsupportedPolicy();

	void setUnsupportedPolicy(const std::vector<std::string>& val);

	void setUnsupportedPolicy(std::vector<std::string>&& val);

private:
	//@ mandatory=false
	//@ name=UnsupportedPolicy
	//@ order=0
	std::vector<std::string> _unsupportedPolicy;

};


inline const std::vector<std::string>& UnsupportedPolicyRequestFault::getUnsupportedPolicy() const
{
	return _unsupportedPolicy;
}


inline std::vector<std::string>& UnsupportedPolicyRequestFault::getUnsupportedPolicy()
{
	return _unsupportedPolicy;
}


inline void UnsupportedPolicyRequestFault::setUnsupportedPolicy(const std::vector<std::string>& val)
{
	_unsupportedPolicy = val;
}


inline void UnsupportedPolicyRequestFault::setUnsupportedPolicy(std::vector<std::string>&& val)
{
	_unsupportedPolicy = std::move(val);
}


} } } // OASIS::WSN::B2


#endif // OASIS_WSN_B2_UnsupportedPolicyRequestFault_INCLUDED
