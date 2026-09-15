// 
// BacklightCompensation.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_BacklightCompensation_INCLUDED
#define ONVIF_BacklightCompensation_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=BacklightCompensation
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API BacklightCompensation
{
public:
	BacklightCompensation();

	BacklightCompensation(
		const std::string& mode, 
		float level);

	virtual ~BacklightCompensation();

	float getLevel() const;

	const std::string& getMode() const;

	void setLevel(float val);

	void setMode(const std::string& val);

	void setMode(std::string&& val);

private:
	//@ name=Mode
	//@ order=0
	std::string _mode;

	//@ name=Level
	//@ order=1
	float _level;

};


inline float BacklightCompensation::getLevel() const
{
	return _level;
}


inline const std::string& BacklightCompensation::getMode() const
{
	return _mode;
}


inline void BacklightCompensation::setLevel(float val)
{
	_level = val;
}


inline void BacklightCompensation::setMode(const std::string& val)
{
	_mode = val;
}


inline void BacklightCompensation::setMode(std::string&& val)
{
	_mode = std::move(val);
}


} // ONVIF


#endif // ONVIF_BacklightCompensation_INCLUDED
