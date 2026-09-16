// 
// PTZPresetTourOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_PTZPresetTourOptions_INCLUDED
#define ONVIF_PTZPresetTourOptions_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/PTZPresetTourSpotOptions.h"
#include "ONVIF/PTZPresetTourStartingConditionOptions.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=PTZPresetTourOptions
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API PTZPresetTourOptions
{
public:
	PTZPresetTourOptions();

	PTZPresetTourOptions(
		bool autoStart, 
		const PTZPresetTourStartingConditionOptions& startingCondition, 
		const PTZPresetTourSpotOptions& tourSpot);

	virtual ~PTZPresetTourOptions();

	bool getAutoStart() const;

	const PTZPresetTourStartingConditionOptions& getStartingCondition() const;

	const PTZPresetTourSpotOptions& getTourSpot() const;

	void setAutoStart(bool val);

	void setStartingCondition(const PTZPresetTourStartingConditionOptions& val);

	void setStartingCondition(PTZPresetTourStartingConditionOptions&& val);

	void setTourSpot(const PTZPresetTourSpotOptions& val);

	void setTourSpot(PTZPresetTourSpotOptions&& val);

private:
	//@ name=AutoStart
	//@ order=0
	bool _autoStart;

	//@ name=StartingCondition
	//@ order=1
	PTZPresetTourStartingConditionOptions _startingCondition;

	//@ name=TourSpot
	//@ order=2
	PTZPresetTourSpotOptions _tourSpot;

};


inline bool PTZPresetTourOptions::getAutoStart() const
{
	return _autoStart;
}


inline const PTZPresetTourStartingConditionOptions& PTZPresetTourOptions::getStartingCondition() const
{
	return _startingCondition;
}


inline const PTZPresetTourSpotOptions& PTZPresetTourOptions::getTourSpot() const
{
	return _tourSpot;
}


inline void PTZPresetTourOptions::setAutoStart(bool val)
{
	_autoStart = val;
}


inline void PTZPresetTourOptions::setStartingCondition(const PTZPresetTourStartingConditionOptions& val)
{
	_startingCondition = val;
}


inline void PTZPresetTourOptions::setStartingCondition(PTZPresetTourStartingConditionOptions&& val)
{
	_startingCondition = std::move(val);
}


inline void PTZPresetTourOptions::setTourSpot(const PTZPresetTourSpotOptions& val)
{
	_tourSpot = val;
}


inline void PTZPresetTourOptions::setTourSpot(PTZPresetTourSpotOptions&& val)
{
	_tourSpot = std::move(val);
}


} // ONVIF


#endif // ONVIF_PTZPresetTourOptions_INCLUDED
