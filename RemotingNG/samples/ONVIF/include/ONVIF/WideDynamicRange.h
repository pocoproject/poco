// 
// WideDynamicRange.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_WideDynamicRange_INCLUDED
#define ONVIF_WideDynamicRange_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=WideDynamicRange
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API WideDynamicRange
{
public:
	WideDynamicRange();

	WideDynamicRange(
		const std::string& mode, 
		float level);

	virtual ~WideDynamicRange();

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


inline float WideDynamicRange::getLevel() const
{
	return _level;
}


inline const std::string& WideDynamicRange::getMode() const
{
	return _mode;
}


inline void WideDynamicRange::setLevel(float val)
{
	_level = val;
}


inline void WideDynamicRange::setMode(const std::string& val)
{
	_mode = val;
}


inline void WideDynamicRange::setMode(std::string&& val)
{
	_mode = std::move(val);
}


} // ONVIF


#endif // ONVIF_WideDynamicRange_INCLUDED
