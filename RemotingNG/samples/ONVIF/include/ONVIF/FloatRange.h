// 
// FloatRange.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_FloatRange_INCLUDED
#define ONVIF_FloatRange_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=FloatRange
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API FloatRange
{
public:
	FloatRange();

	FloatRange(
		float min, 
		float max);

	virtual ~FloatRange();

	float getMax() const;

	float getMin() const;

	void setMax(float val);

	void setMin(float val);

private:
	//@ name=Min
	//@ order=0
	float _min;

	//@ name=Max
	//@ order=1
	float _max;

};


inline float FloatRange::getMax() const
{
	return _max;
}


inline float FloatRange::getMin() const
{
	return _min;
}


inline void FloatRange::setMax(float val)
{
	_max = val;
}


inline void FloatRange::setMin(float val)
{
	_min = val;
}


} // ONVIF


#endif // ONVIF_FloatRange_INCLUDED
