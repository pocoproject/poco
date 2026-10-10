// 
// NoiseReductionOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_NoiseReductionOptions_INCLUDED
#define ONVIF_NoiseReductionOptions_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=NoiseReductionOptions
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API NoiseReductionOptions
{
public:
	NoiseReductionOptions();

	NoiseReductionOptions(bool level);

	virtual ~NoiseReductionOptions();

	bool getLevel() const;

	void setLevel(bool val);

private:
	//@ name=Level
	//@ order=0
	bool _level;

};


inline bool NoiseReductionOptions::getLevel() const
{
	return _level;
}


inline void NoiseReductionOptions::setLevel(bool val)
{
	_level = val;
}


} // ONVIF


#endif // ONVIF_NoiseReductionOptions_INCLUDED
