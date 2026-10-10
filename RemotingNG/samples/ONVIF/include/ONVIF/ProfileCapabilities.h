// 
// ProfileCapabilities.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_ProfileCapabilities_INCLUDED
#define ONVIF_ProfileCapabilities_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=ProfileCapabilities
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API ProfileCapabilities
{
public:
	ProfileCapabilities();

	ProfileCapabilities(int maximumNumberOfProfiles);

	virtual ~ProfileCapabilities();

	int getMaximumNumberOfProfiles() const;

	void setMaximumNumberOfProfiles(int val);

private:
	//@ name=MaximumNumberOfProfiles
	//@ order=0
	int _maximumNumberOfProfiles;

};


inline int ProfileCapabilities::getMaximumNumberOfProfiles() const
{
	return _maximumNumberOfProfiles;
}


inline void ProfileCapabilities::setMaximumNumberOfProfiles(int val)
{
	_maximumNumberOfProfiles = val;
}


} // ONVIF


#endif // ONVIF_ProfileCapabilities_INCLUDED
