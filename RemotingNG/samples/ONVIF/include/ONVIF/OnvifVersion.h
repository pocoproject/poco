// 
// OnvifVersion.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_OnvifVersion_INCLUDED
#define ONVIF_OnvifVersion_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=OnvifVersion
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API OnvifVersion
{
public:
	OnvifVersion();

	OnvifVersion(
		int major, 
		int minor);

	virtual ~OnvifVersion();

	int getMajor() const;

	int getMinor() const;

	void setMajor(int val);

	void setMinor(int val);

private:
	//@ name=Major
	//@ order=0
	int _major;

	//@ name=Minor
	//@ order=1
	int _minor;

};


inline int OnvifVersion::getMajor() const
{
	return _major;
}


inline int OnvifVersion::getMinor() const
{
	return _minor;
}


inline void OnvifVersion::setMajor(int val)
{
	_major = val;
}


inline void OnvifVersion::setMinor(int val)
{
	_minor = val;
}


} // ONVIF


#endif // ONVIF_OnvifVersion_INCLUDED
