// 
// Renew.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef OASIS_WSN_B2_Renew_INCLUDED
#define OASIS_WSN_B2_Renew_INCLUDED


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


//@ name="#Renew"
//@ namespace="http://docs.oasis-open.org/wsn/b-2"
//@ serialize
class ONVIF_API Renew
{
public:
	Renew();

	Renew(const Poco::Nullable<Poco::DateTime>& terminationTime);

	virtual ~Renew();

	//@ return={xsdType}
	const Poco::Nullable<Poco::DateTime>& getTerminationTime() const;

	//@ $val={xsdType}
	void setTerminationTime(const Poco::Nullable<Poco::DateTime>& val);

	//@ $val={xsdType}
	void setTerminationTime(Poco::Nullable<Poco::DateTime>&& val);

private:
	//@ name=TerminationTime
	//@ order=0
	//@ xsdType
	Poco::Nullable<Poco::DateTime> _terminationTime;

};


inline const Poco::Nullable<Poco::DateTime>& Renew::getTerminationTime() const
{
	return _terminationTime;
}


inline void Renew::setTerminationTime(const Poco::Nullable<Poco::DateTime>& val)
{
	_terminationTime = val;
}


inline void Renew::setTerminationTime(Poco::Nullable<Poco::DateTime>&& val)
{
	_terminationTime = std::move(val);
}


} } } // OASIS::WSN::B2


#endif // OASIS_WSN_B2_Renew_INCLUDED
