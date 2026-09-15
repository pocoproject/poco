// 
// WideDynamicRangeOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_WideDynamicRangeOptions_INCLUDED
#define ONVIF_WideDynamicRangeOptions_INCLUDED


#include "ONVIF/FloatRange.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=WideDynamicRangeOptions
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API WideDynamicRangeOptions
{
public:
	WideDynamicRangeOptions();

	WideDynamicRangeOptions(
		const std::vector<std::string>& mode, 
		const FloatRange& level);

	virtual ~WideDynamicRangeOptions();

	const FloatRange& getLevel() const;

	const std::vector<std::string>& getMode() const;

	std::vector<std::string>& getMode();

	void setLevel(const FloatRange& val);

	void setLevel(FloatRange&& val);

	void setMode(const std::vector<std::string>& val);

	void setMode(std::vector<std::string>&& val);

private:
	//@ name=Mode
	//@ order=0
	std::vector<std::string> _mode;

	//@ name=Level
	//@ order=1
	FloatRange _level;

};


inline const FloatRange& WideDynamicRangeOptions::getLevel() const
{
	return _level;
}


inline const std::vector<std::string>& WideDynamicRangeOptions::getMode() const
{
	return _mode;
}


inline std::vector<std::string>& WideDynamicRangeOptions::getMode()
{
	return _mode;
}


inline void WideDynamicRangeOptions::setLevel(const FloatRange& val)
{
	_level = val;
}


inline void WideDynamicRangeOptions::setLevel(FloatRange&& val)
{
	_level = std::move(val);
}


inline void WideDynamicRangeOptions::setMode(const std::vector<std::string>& val)
{
	_mode = val;
}


inline void WideDynamicRangeOptions::setMode(std::vector<std::string>&& val)
{
	_mode = std::move(val);
}


} // ONVIF


#endif // ONVIF_WideDynamicRangeOptions_INCLUDED
