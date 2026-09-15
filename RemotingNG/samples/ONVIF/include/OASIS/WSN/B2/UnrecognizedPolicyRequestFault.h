// 
// UnrecognizedPolicyRequestFault.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef OASIS_WSN_B2_UnrecognizedPolicyRequestFault_INCLUDED
#define OASIS_WSN_B2_UnrecognizedPolicyRequestFault_INCLUDED


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


//@ name=UnrecognizedPolicyRequestFaultType
//@ namespace="http://docs.oasis-open.org/wsn/b-2"
//@ serialize
class ONVIF_API UnrecognizedPolicyRequestFault: public OASIS::WSRF::BF2::BaseFault
{
public:
	UnrecognizedPolicyRequestFault();

	UnrecognizedPolicyRequestFault(
		const Poco::DateTime& timestamp, 
		const Poco::SharedPtr<W3::Addressing::EndpointReference>& originator, 
		const Poco::SharedPtr<OASIS::WSRF::BF2::ErrorCode>& errorCode, 
		const std::vector<OASIS::WSRF::BF2::Description>& description, 
		const Poco::SharedPtr<OASIS::WSRF::BF2::FaultCause>& faultCause, 
		const std::vector<std::string>& unrecognizedPolicy);

	virtual ~UnrecognizedPolicyRequestFault();

	const std::vector<std::string>& getUnrecognizedPolicy() const;

	std::vector<std::string>& getUnrecognizedPolicy();

	void setUnrecognizedPolicy(const std::vector<std::string>& val);

	void setUnrecognizedPolicy(std::vector<std::string>&& val);

private:
	//@ mandatory=false
	//@ name=UnrecognizedPolicy
	//@ order=0
	std::vector<std::string> _unrecognizedPolicy;

};


inline const std::vector<std::string>& UnrecognizedPolicyRequestFault::getUnrecognizedPolicy() const
{
	return _unrecognizedPolicy;
}


inline std::vector<std::string>& UnrecognizedPolicyRequestFault::getUnrecognizedPolicy()
{
	return _unrecognizedPolicy;
}


inline void UnrecognizedPolicyRequestFault::setUnrecognizedPolicy(const std::vector<std::string>& val)
{
	_unrecognizedPolicy = val;
}


inline void UnrecognizedPolicyRequestFault::setUnrecognizedPolicy(std::vector<std::string>&& val)
{
	_unrecognizedPolicy = std::move(val);
}


} } } // OASIS::WSN::B2


#endif // OASIS_WSN_B2_UnrecognizedPolicyRequestFault_INCLUDED
