// 
// ProfileCapabilities.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_ProfileCapabilities_INCLUDED
#define ONVIF_Media_ProfileCapabilities_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name=ProfileCapabilities
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API ProfileCapabilities
{
public:
	ProfileCapabilities();

	ProfileCapabilities(Poco::Optional<int> maximumNumberOfProfiles);

	virtual ~ProfileCapabilities();

	Poco::Optional<int> getMaximumNumberOfProfiles() const;

	void setMaximumNumberOfProfiles(Poco::Optional<int> val);

private:
	//@ mandatory=false
	//@ name=MaximumNumberOfProfiles
	//@ order=0
	//@ type=attr
	Poco::Optional<int> _maximumNumberOfProfiles;

};


inline Poco::Optional<int> ProfileCapabilities::getMaximumNumberOfProfiles() const
{
	return _maximumNumberOfProfiles;
}


inline void ProfileCapabilities::setMaximumNumberOfProfiles(Poco::Optional<int> val)
{
	_maximumNumberOfProfiles = val;
}


} } // ONVIF::Media


#endif // ONVIF_Media_ProfileCapabilities_INCLUDED
