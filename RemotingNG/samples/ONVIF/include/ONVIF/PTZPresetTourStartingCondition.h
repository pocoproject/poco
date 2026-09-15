// 
// PTZPresetTourStartingCondition.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_PTZPresetTourStartingCondition_INCLUDED
#define ONVIF_PTZPresetTourStartingCondition_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class PTZPresetTourStartingConditionExtension;
} 


namespace ONVIF {


//@ name=PTZPresetTourStartingCondition
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API PTZPresetTourStartingCondition
{
public:
	PTZPresetTourStartingCondition();

	PTZPresetTourStartingCondition(
		Poco::Optional<bool> randomPresetOrder, 
		Poco::Optional<int> recurringTime, 
		const Poco::Optional<std::string>& recurringDuration, 
		const Poco::Optional<std::string>& direction, 
		const Poco::SharedPtr<PTZPresetTourStartingConditionExtension>& extension);

	virtual ~PTZPresetTourStartingCondition();

	const Poco::Optional<std::string>& getDirection() const;

	const Poco::SharedPtr<PTZPresetTourStartingConditionExtension>& getExtension() const;

	Poco::Optional<bool> getRandomPresetOrder() const;

	const Poco::Optional<std::string>& getRecurringDuration() const;

	Poco::Optional<int> getRecurringTime() const;

	void setDirection(const Poco::Optional<std::string>& val);

	void setDirection(Poco::Optional<std::string>&& val);

	void setExtension(const Poco::SharedPtr<PTZPresetTourStartingConditionExtension>& val);

	void setExtension(Poco::SharedPtr<PTZPresetTourStartingConditionExtension>&& val);

	void setRandomPresetOrder(Poco::Optional<bool> val);

	void setRecurringDuration(const Poco::Optional<std::string>& val);

	void setRecurringDuration(Poco::Optional<std::string>&& val);

	void setRecurringTime(Poco::Optional<int> val);

private:
	//@ mandatory=false
	//@ name=RandomPresetOrder
	//@ order=0
	//@ type=attr
	Poco::Optional<bool> _randomPresetOrder;

	//@ mandatory=false
	//@ name=RecurringTime
	//@ order=1
	Poco::Optional<int> _recurringTime;

	//@ mandatory=false
	//@ name=RecurringDuration
	//@ order=2
	Poco::Optional<std::string> _recurringDuration;

	//@ mandatory=false
	//@ name=Direction
	//@ order=3
	Poco::Optional<std::string> _direction;

	//@ mandatory=false
	//@ name=Extension
	//@ order=4
	Poco::SharedPtr<PTZPresetTourStartingConditionExtension> _extension;

};


inline const Poco::Optional<std::string>& PTZPresetTourStartingCondition::getDirection() const
{
	return _direction;
}


inline const Poco::SharedPtr<PTZPresetTourStartingConditionExtension>& PTZPresetTourStartingCondition::getExtension() const
{
	return _extension;
}


inline Poco::Optional<bool> PTZPresetTourStartingCondition::getRandomPresetOrder() const
{
	return _randomPresetOrder;
}


inline const Poco::Optional<std::string>& PTZPresetTourStartingCondition::getRecurringDuration() const
{
	return _recurringDuration;
}


inline Poco::Optional<int> PTZPresetTourStartingCondition::getRecurringTime() const
{
	return _recurringTime;
}


inline void PTZPresetTourStartingCondition::setDirection(const Poco::Optional<std::string>& val)
{
	_direction = val;
}


inline void PTZPresetTourStartingCondition::setDirection(Poco::Optional<std::string>&& val)
{
	_direction = std::move(val);
}


inline void PTZPresetTourStartingCondition::setExtension(const Poco::SharedPtr<PTZPresetTourStartingConditionExtension>& val)
{
	_extension = val;
}


inline void PTZPresetTourStartingCondition::setExtension(Poco::SharedPtr<PTZPresetTourStartingConditionExtension>&& val)
{
	_extension = std::move(val);
}


inline void PTZPresetTourStartingCondition::setRandomPresetOrder(Poco::Optional<bool> val)
{
	_randomPresetOrder = val;
}


inline void PTZPresetTourStartingCondition::setRecurringDuration(const Poco::Optional<std::string>& val)
{
	_recurringDuration = val;
}


inline void PTZPresetTourStartingCondition::setRecurringDuration(Poco::Optional<std::string>&& val)
{
	_recurringDuration = std::move(val);
}


inline void PTZPresetTourStartingCondition::setRecurringTime(Poco::Optional<int> val)
{
	_recurringTime = val;
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/PTZPresetTourStartingConditionExtension.h"


#endif // ONVIF_PTZPresetTourStartingCondition_INCLUDED
