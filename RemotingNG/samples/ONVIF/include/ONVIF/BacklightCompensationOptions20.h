// 
// BacklightCompensationOptions20.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_BacklightCompensationOptions20_INCLUDED
#define ONVIF_BacklightCompensationOptions20_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class FloatRange;
} 


namespace ONVIF {


//@ name=BacklightCompensationOptions20
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API BacklightCompensationOptions20
{
public:
	BacklightCompensationOptions20();

	BacklightCompensationOptions20(
		const std::vector<std::string>& mode, 
		const Poco::SharedPtr<FloatRange>& level);

	virtual ~BacklightCompensationOptions20();

	const Poco::SharedPtr<FloatRange>& getLevel() const;

	const std::vector<std::string>& getMode() const;

	std::vector<std::string>& getMode();

	void setLevel(const Poco::SharedPtr<FloatRange>& val);

	void setLevel(Poco::SharedPtr<FloatRange>&& val);

	void setMode(const std::vector<std::string>& val);

	void setMode(std::vector<std::string>&& val);

private:
	//@ name=Mode
	//@ order=0
	std::vector<std::string> _mode;

	//@ mandatory=false
	//@ name=Level
	//@ order=1
	Poco::SharedPtr<FloatRange> _level;

};


inline const Poco::SharedPtr<FloatRange>& BacklightCompensationOptions20::getLevel() const
{
	return _level;
}


inline const std::vector<std::string>& BacklightCompensationOptions20::getMode() const
{
	return _mode;
}


inline std::vector<std::string>& BacklightCompensationOptions20::getMode()
{
	return _mode;
}


inline void BacklightCompensationOptions20::setLevel(const Poco::SharedPtr<FloatRange>& val)
{
	_level = val;
}


inline void BacklightCompensationOptions20::setLevel(Poco::SharedPtr<FloatRange>&& val)
{
	_level = std::move(val);
}


inline void BacklightCompensationOptions20::setMode(const std::vector<std::string>& val)
{
	_mode = val;
}


inline void BacklightCompensationOptions20::setMode(std::vector<std::string>&& val)
{
	_mode = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/FloatRange.h"


#endif // ONVIF_BacklightCompensationOptions20_INCLUDED
