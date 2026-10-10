// 
// InvalidFilterFault.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef OASIS_WSN_B2_InvalidFilterFault_INCLUDED
#define OASIS_WSN_B2_InvalidFilterFault_INCLUDED


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


//@ name=InvalidFilterFaultType
//@ namespace="http://docs.oasis-open.org/wsn/b-2"
//@ serialize
class ONVIF_API InvalidFilterFault: public OASIS::WSRF::BF2::BaseFault
{
public:
	InvalidFilterFault();

	InvalidFilterFault(
		const Poco::DateTime& timestamp, 
		const Poco::SharedPtr<W3::Addressing::EndpointReference>& originator, 
		const Poco::SharedPtr<OASIS::WSRF::BF2::ErrorCode>& errorCode, 
		const std::vector<OASIS::WSRF::BF2::Description>& description, 
		const Poco::SharedPtr<OASIS::WSRF::BF2::FaultCause>& faultCause, 
		const std::vector<std::string>& unknownFilter);

	virtual ~InvalidFilterFault();

	const std::vector<std::string>& getUnknownFilter() const;

	std::vector<std::string>& getUnknownFilter();

	void setUnknownFilter(const std::vector<std::string>& val);

	void setUnknownFilter(std::vector<std::string>&& val);

private:
	//@ name=UnknownFilter
	//@ order=0
	std::vector<std::string> _unknownFilter;

};


inline const std::vector<std::string>& InvalidFilterFault::getUnknownFilter() const
{
	return _unknownFilter;
}


inline std::vector<std::string>& InvalidFilterFault::getUnknownFilter()
{
	return _unknownFilter;
}


inline void InvalidFilterFault::setUnknownFilter(const std::vector<std::string>& val)
{
	_unknownFilter = val;
}


inline void InvalidFilterFault::setUnknownFilter(std::vector<std::string>&& val)
{
	_unknownFilter = std::move(val);
}


} } } // OASIS::WSN::B2


#endif // OASIS_WSN_B2_InvalidFilterFault_INCLUDED
