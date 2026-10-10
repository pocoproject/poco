// 
// NoiseReduction.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_NoiseReduction_INCLUDED
#define ONVIF_NoiseReduction_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=NoiseReduction
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API NoiseReduction
{
public:
	NoiseReduction();

	NoiseReduction(float level);

	virtual ~NoiseReduction();

	float getLevel() const;

	void setLevel(float val);

private:
	//@ name=Level
	//@ order=0
	float _level;

};


inline float NoiseReduction::getLevel() const
{
	return _level;
}


inline void NoiseReduction::setLevel(float val)
{
	_level = val;
}


} // ONVIF


#endif // ONVIF_NoiseReduction_INCLUDED
