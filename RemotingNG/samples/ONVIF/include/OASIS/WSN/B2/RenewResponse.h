// 
// RenewResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef OASIS_WSN_B2_RenewResponse_INCLUDED
#define OASIS_WSN_B2_RenewResponse_INCLUDED


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


//@ name="#RenewResponse"
//@ namespace="http://docs.oasis-open.org/wsn/b-2"
//@ serialize
class ONVIF_API RenewResponse
{
public:
	RenewResponse();

	RenewResponse(
		const Poco::Nullable<Poco::DateTime>& terminationTime, 
		const Poco::Optional<Poco::DateTime>& currentTime);

	virtual ~RenewResponse();

	//@ return={xsdType=dateTime}
	const Poco::Optional<Poco::DateTime>& getCurrentTime() const;

	//@ return={xsdType=dateTime}
	const Poco::Nullable<Poco::DateTime>& getTerminationTime() const;

	//@ $val={xsdType=dateTime}
	void setCurrentTime(const Poco::Optional<Poco::DateTime>& val);

	//@ $val={xsdType=dateTime}
	void setCurrentTime(Poco::Optional<Poco::DateTime>&& val);

	//@ $val={xsdType=dateTime}
	void setTerminationTime(const Poco::Nullable<Poco::DateTime>& val);

	//@ $val={xsdType=dateTime}
	void setTerminationTime(Poco::Nullable<Poco::DateTime>&& val);

private:
	//@ name=TerminationTime
	//@ order=0
	//@ xsdType=dateTime
	Poco::Nullable<Poco::DateTime> _terminationTime;

	//@ mandatory=false
	//@ name=CurrentTime
	//@ order=1
	//@ xsdType=dateTime
	Poco::Optional<Poco::DateTime> _currentTime;

};


inline const Poco::Optional<Poco::DateTime>& RenewResponse::getCurrentTime() const
{
	return _currentTime;
}


inline const Poco::Nullable<Poco::DateTime>& RenewResponse::getTerminationTime() const
{
	return _terminationTime;
}


inline void RenewResponse::setCurrentTime(const Poco::Optional<Poco::DateTime>& val)
{
	_currentTime = val;
}


inline void RenewResponse::setCurrentTime(Poco::Optional<Poco::DateTime>&& val)
{
	_currentTime = std::move(val);
}


inline void RenewResponse::setTerminationTime(const Poco::Nullable<Poco::DateTime>& val)
{
	_terminationTime = val;
}


inline void RenewResponse::setTerminationTime(Poco::Nullable<Poco::DateTime>&& val)
{
	_terminationTime = std::move(val);
}


} } } // OASIS::WSN::B2


#endif // OASIS_WSN_B2_RenewResponse_INCLUDED
