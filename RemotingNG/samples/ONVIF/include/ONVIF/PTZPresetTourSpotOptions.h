// 
// PTZPresetTourSpotOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_PTZPresetTourSpotOptions_INCLUDED
#define ONVIF_PTZPresetTourSpotOptions_INCLUDED


#include "ONVIF/DurationRange.h"
#include "ONVIF/ONVIF.h"
#include "ONVIF/PTZPresetTourPresetDetailOptions.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=PTZPresetTourSpotOptions
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API PTZPresetTourSpotOptions
{
public:
	PTZPresetTourSpotOptions();

	PTZPresetTourSpotOptions(
		const PTZPresetTourPresetDetailOptions& presetDetail, 
		const DurationRange& stayTime);

	virtual ~PTZPresetTourSpotOptions();

	const PTZPresetTourPresetDetailOptions& getPresetDetail() const;

	const DurationRange& getStayTime() const;

	void setPresetDetail(const PTZPresetTourPresetDetailOptions& val);

	void setPresetDetail(PTZPresetTourPresetDetailOptions&& val);

	void setStayTime(const DurationRange& val);

	void setStayTime(DurationRange&& val);

private:
	//@ name=PresetDetail
	//@ order=0
	PTZPresetTourPresetDetailOptions _presetDetail;

	//@ name=StayTime
	//@ order=1
	DurationRange _stayTime;

};


inline const PTZPresetTourPresetDetailOptions& PTZPresetTourSpotOptions::getPresetDetail() const
{
	return _presetDetail;
}


inline const DurationRange& PTZPresetTourSpotOptions::getStayTime() const
{
	return _stayTime;
}


inline void PTZPresetTourSpotOptions::setPresetDetail(const PTZPresetTourPresetDetailOptions& val)
{
	_presetDetail = val;
}


inline void PTZPresetTourSpotOptions::setPresetDetail(PTZPresetTourPresetDetailOptions&& val)
{
	_presetDetail = std::move(val);
}


inline void PTZPresetTourSpotOptions::setStayTime(const DurationRange& val)
{
	_stayTime = val;
}


inline void PTZPresetTourSpotOptions::setStayTime(DurationRange&& val)
{
	_stayTime = std::move(val);
}


} // ONVIF


#endif // ONVIF_PTZPresetTourSpotOptions_INCLUDED
