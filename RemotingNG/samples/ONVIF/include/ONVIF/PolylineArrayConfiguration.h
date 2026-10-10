// 
// PolylineArrayConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_PolylineArrayConfiguration_INCLUDED
#define ONVIF_PolylineArrayConfiguration_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/PolylineArray.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=PolylineArrayConfiguration
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API PolylineArrayConfiguration
{
public:
	PolylineArrayConfiguration();

	PolylineArrayConfiguration(const PolylineArray& polylineArray);

	virtual ~PolylineArrayConfiguration();

	const PolylineArray& getPolylineArray() const;

	void setPolylineArray(const PolylineArray& val);

	void setPolylineArray(PolylineArray&& val);

private:
	//@ name=PolylineArray
	//@ order=0
	PolylineArray _polylineArray;

};


inline const PolylineArray& PolylineArrayConfiguration::getPolylineArray() const
{
	return _polylineArray;
}


inline void PolylineArrayConfiguration::setPolylineArray(const PolylineArray& val)
{
	_polylineArray = val;
}


inline void PolylineArrayConfiguration::setPolylineArray(PolylineArray&& val)
{
	_polylineArray = std::move(val);
}


} // ONVIF


#endif // ONVIF_PolylineArrayConfiguration_INCLUDED
