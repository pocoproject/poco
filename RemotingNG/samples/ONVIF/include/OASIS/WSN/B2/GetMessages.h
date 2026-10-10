// 
// GetMessages.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef OASIS_WSN_B2_GetMessages_INCLUDED
#define OASIS_WSN_B2_GetMessages_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/Types.h"
#include <string>
#include <vector>


namespace OASIS {
namespace WSN {
namespace B2 {


//@ name="#GetMessages"
//@ namespace="http://docs.oasis-open.org/wsn/b-2"
//@ serialize
class ONVIF_API GetMessages
{
public:
	GetMessages();

	GetMessages(Poco::Optional<Poco::UInt32> maximumNumber);

	virtual ~GetMessages();

	Poco::Optional<Poco::UInt32> getMaximumNumber() const;

	void setMaximumNumber(Poco::Optional<Poco::UInt32> val);

private:
	//@ mandatory=false
	//@ name=MaximumNumber
	//@ order=0
	Poco::Optional<Poco::UInt32> _maximumNumber;

};


inline Poco::Optional<Poco::UInt32> GetMessages::getMaximumNumber() const
{
	return _maximumNumber;
}


inline void GetMessages::setMaximumNumber(Poco::Optional<Poco::UInt32> val)
{
	_maximumNumber = val;
}


} } } // OASIS::WSN::B2


#endif // OASIS_WSN_B2_GetMessages_INCLUDED
