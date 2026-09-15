// 
// PTZPresetTourStartingConditionOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_PTZPresetTourStartingConditionOptions_INCLUDED
#define ONVIF_PTZPresetTourStartingConditionOptions_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class DurationRange;
class IntRange;
class PTZPresetTourStartingConditionOptionsExtension;
} 


namespace ONVIF {


//@ name=PTZPresetTourStartingConditionOptions
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API PTZPresetTourStartingConditionOptions
{
public:
	PTZPresetTourStartingConditionOptions();

	PTZPresetTourStartingConditionOptions(
		const Poco::SharedPtr<IntRange>& recurringTime, 
		const Poco::SharedPtr<DurationRange>& recurringDuration, 
		const std::vector<std::string>& direction, 
		const Poco::SharedPtr<PTZPresetTourStartingConditionOptionsExtension>& extension);

	virtual ~PTZPresetTourStartingConditionOptions();

	const std::vector<std::string>& getDirection() const;

	std::vector<std::string>& getDirection();

	const Poco::SharedPtr<PTZPresetTourStartingConditionOptionsExtension>& getExtension() const;

	const Poco::SharedPtr<DurationRange>& getRecurringDuration() const;

	const Poco::SharedPtr<IntRange>& getRecurringTime() const;

	void setDirection(const std::vector<std::string>& val);

	void setDirection(std::vector<std::string>&& val);

	void setExtension(const Poco::SharedPtr<PTZPresetTourStartingConditionOptionsExtension>& val);

	void setExtension(Poco::SharedPtr<PTZPresetTourStartingConditionOptionsExtension>&& val);

	void setRecurringDuration(const Poco::SharedPtr<DurationRange>& val);

	void setRecurringDuration(Poco::SharedPtr<DurationRange>&& val);

	void setRecurringTime(const Poco::SharedPtr<IntRange>& val);

	void setRecurringTime(Poco::SharedPtr<IntRange>&& val);

private:
	//@ mandatory=false
	//@ name=RecurringTime
	//@ order=0
	Poco::SharedPtr<IntRange> _recurringTime;

	//@ mandatory=false
	//@ name=RecurringDuration
	//@ order=1
	Poco::SharedPtr<DurationRange> _recurringDuration;

	//@ mandatory=false
	//@ name=Direction
	//@ order=2
	std::vector<std::string> _direction;

	//@ mandatory=false
	//@ name=Extension
	//@ order=3
	Poco::SharedPtr<PTZPresetTourStartingConditionOptionsExtension> _extension;

};


inline const std::vector<std::string>& PTZPresetTourStartingConditionOptions::getDirection() const
{
	return _direction;
}


inline std::vector<std::string>& PTZPresetTourStartingConditionOptions::getDirection()
{
	return _direction;
}


inline const Poco::SharedPtr<PTZPresetTourStartingConditionOptionsExtension>& PTZPresetTourStartingConditionOptions::getExtension() const
{
	return _extension;
}


inline const Poco::SharedPtr<DurationRange>& PTZPresetTourStartingConditionOptions::getRecurringDuration() const
{
	return _recurringDuration;
}


inline const Poco::SharedPtr<IntRange>& PTZPresetTourStartingConditionOptions::getRecurringTime() const
{
	return _recurringTime;
}


inline void PTZPresetTourStartingConditionOptions::setDirection(const std::vector<std::string>& val)
{
	_direction = val;
}


inline void PTZPresetTourStartingConditionOptions::setDirection(std::vector<std::string>&& val)
{
	_direction = std::move(val);
}


inline void PTZPresetTourStartingConditionOptions::setExtension(const Poco::SharedPtr<PTZPresetTourStartingConditionOptionsExtension>& val)
{
	_extension = val;
}


inline void PTZPresetTourStartingConditionOptions::setExtension(Poco::SharedPtr<PTZPresetTourStartingConditionOptionsExtension>&& val)
{
	_extension = std::move(val);
}


inline void PTZPresetTourStartingConditionOptions::setRecurringDuration(const Poco::SharedPtr<DurationRange>& val)
{
	_recurringDuration = val;
}


inline void PTZPresetTourStartingConditionOptions::setRecurringDuration(Poco::SharedPtr<DurationRange>&& val)
{
	_recurringDuration = std::move(val);
}


inline void PTZPresetTourStartingConditionOptions::setRecurringTime(const Poco::SharedPtr<IntRange>& val)
{
	_recurringTime = val;
}


inline void PTZPresetTourStartingConditionOptions::setRecurringTime(Poco::SharedPtr<IntRange>&& val)
{
	_recurringTime = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/DurationRange.h"
#include "ONVIF/IntRange.h"
#include "ONVIF/PTZPresetTourStartingConditionOptionsExtension.h"


#endif // ONVIF_PTZPresetTourStartingConditionOptions_INCLUDED
