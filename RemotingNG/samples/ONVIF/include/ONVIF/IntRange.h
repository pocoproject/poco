// 
// IntRange.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_IntRange_INCLUDED
#define ONVIF_IntRange_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=IntRange
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API IntRange
{
public:
	IntRange();

	IntRange(
		int min, 
		int max);

	virtual ~IntRange();

	int getMax() const;

	int getMin() const;

	void setMax(int val);

	void setMin(int val);

private:
	//@ name=Min
	//@ order=0
	int _min;

	//@ name=Max
	//@ order=1
	int _max;

};


inline int IntRange::getMax() const
{
	return _max;
}


inline int IntRange::getMin() const
{
	return _min;
}


inline void IntRange::setMax(int val)
{
	_max = val;
}


inline void IntRange::setMin(int val)
{
	_min = val;
}


} // ONVIF


#endif // ONVIF_IntRange_INCLUDED
