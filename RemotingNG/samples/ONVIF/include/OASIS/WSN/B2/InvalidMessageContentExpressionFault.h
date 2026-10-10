// 
// InvalidMessageContentExpressionFault.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef OASIS_WSN_B2_InvalidMessageContentExpressionFault_INCLUDED
#define OASIS_WSN_B2_InvalidMessageContentExpressionFault_INCLUDED


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


//@ name=InvalidMessageContentExpressionFaultType
//@ namespace="http://docs.oasis-open.org/wsn/b-2"
//@ serialize
class ONVIF_API InvalidMessageContentExpressionFault: public OASIS::WSRF::BF2::BaseFault
{
public:
	InvalidMessageContentExpressionFault();

	InvalidMessageContentExpressionFault(
		const Poco::DateTime& timestamp, 
		const Poco::SharedPtr<W3::Addressing::EndpointReference>& originator, 
		const Poco::SharedPtr<OASIS::WSRF::BF2::ErrorCode>& errorCode, 
		const std::vector<OASIS::WSRF::BF2::Description>& description, 
		const Poco::SharedPtr<OASIS::WSRF::BF2::FaultCause>& faultCause);

	virtual ~InvalidMessageContentExpressionFault();

};


} } } // OASIS::WSN::B2


#endif // OASIS_WSN_B2_InvalidMessageContentExpressionFault_INCLUDED
