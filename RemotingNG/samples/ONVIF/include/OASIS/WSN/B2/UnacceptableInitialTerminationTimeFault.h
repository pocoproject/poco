// 
// UnacceptableInitialTerminationTimeFault.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef OASIS_WSN_B2_UnacceptableInitialTerminationTimeFault_INCLUDED
#define OASIS_WSN_B2_UnacceptableInitialTerminationTimeFault_INCLUDED


#include "OASIS/WSRF/BF2/BaseFault.h"
#include "ONVIF/ONVIF.h"
#include "Poco/DateTime.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace OASIS {
namespace WSN {
namespace B2 {


//@ name=UnacceptableInitialTerminationTimeFaultType
//@ namespace="http://docs.oasis-open.org/wsn/b-2"
//@ serialize
class ONVIF_API UnacceptableInitialTerminationTimeFault: public OASIS::WSRF::BF2::BaseFault
{
public:
	UnacceptableInitialTerminationTimeFault();

	UnacceptableInitialTerminationTimeFault(
		const Poco::DateTime& timestamp, 
		const Poco::SharedPtr<W3::Addressing::EndpointReference>& originator, 
		const Poco::SharedPtr<OASIS::WSRF::BF2::ErrorCode>& errorCode, 
		const std::vector<OASIS::WSRF::BF2::Description>& description, 
		const Poco::SharedPtr<OASIS::WSRF::BF2::FaultCause>& faultCause, 
		const Poco::DateTime& minimumTime, 
		const Poco::Optional<Poco::DateTime>& maximumTime);

	virtual ~UnacceptableInitialTerminationTimeFault();

	//@ return={xsdType=dateTime}
	const Poco::Optional<Poco::DateTime>& getMaximumTime() const;

	//@ return={xsdType=dateTime}
	const Poco::DateTime& getMinimumTime() const;

	//@ $val={xsdType=dateTime}
	void setMaximumTime(const Poco::Optional<Poco::DateTime>& val);

	//@ $val={xsdType=dateTime}
	void setMaximumTime(Poco::Optional<Poco::DateTime>&& val);

	//@ $val={xsdType=dateTime}
	void setMinimumTime(const Poco::DateTime& val);

	//@ $val={xsdType=dateTime}
	void setMinimumTime(Poco::DateTime&& val);

private:
	//@ name=MinimumTime
	//@ order=0
	//@ xsdType=dateTime
	Poco::DateTime _minimumTime;

	//@ mandatory=false
	//@ name=MaximumTime
	//@ order=1
	//@ xsdType=dateTime
	Poco::Optional<Poco::DateTime> _maximumTime;

};


inline const Poco::Optional<Poco::DateTime>& UnacceptableInitialTerminationTimeFault::getMaximumTime() const
{
	return _maximumTime;
}


inline const Poco::DateTime& UnacceptableInitialTerminationTimeFault::getMinimumTime() const
{
	return _minimumTime;
}


inline void UnacceptableInitialTerminationTimeFault::setMaximumTime(const Poco::Optional<Poco::DateTime>& val)
{
	_maximumTime = val;
}


inline void UnacceptableInitialTerminationTimeFault::setMaximumTime(Poco::Optional<Poco::DateTime>&& val)
{
	_maximumTime = std::move(val);
}


inline void UnacceptableInitialTerminationTimeFault::setMinimumTime(const Poco::DateTime& val)
{
	_minimumTime = val;
}


inline void UnacceptableInitialTerminationTimeFault::setMinimumTime(Poco::DateTime&& val)
{
	_minimumTime = std::move(val);
}


} } } // OASIS::WSN::B2


#endif // OASIS_WSN_B2_UnacceptableInitialTerminationTimeFault_INCLUDED
