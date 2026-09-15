// 
// Description.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef OASIS_WSRF_BF2_Description_INCLUDED
#define OASIS_WSRF_BF2_Description_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace OASIS {
namespace WSRF {
namespace BF2 {


//@ name="#Description"
//@ namespace="http://docs.oasis-open.org/wsrf/bf-2"
//@ serialize
class ONVIF_API Description
{
public:
	Description();

	Description(const Poco::Optional<std::string>& value);

	virtual ~Description();

	const Poco::Optional<std::string>& getValue() const;

	void setValue(const Poco::Optional<std::string>& val);

	void setValue(Poco::Optional<std::string>&& val);

private:
	//@ inline=true
	//@ mandatory=false
	//@ name=value
	//@ order=0
	Poco::Optional<std::string> _value;

};


inline const Poco::Optional<std::string>& Description::getValue() const
{
	return _value;
}


inline void Description::setValue(const Poco::Optional<std::string>& val)
{
	_value = val;
}


inline void Description::setValue(Poco::Optional<std::string>&& val)
{
	_value = std::move(val);
}


} } } // OASIS::WSRF::BF2


#endif // OASIS_WSRF_BF2_Description_INCLUDED
