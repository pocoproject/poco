// 
// PTZPresetTourSpot.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_PTZPresetTourSpot_INCLUDED
#define ONVIF_PTZPresetTourSpot_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/PTZPresetTourPresetDetail.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class PTZPresetTourSpotExtension;
class PTZSpeed;
} 


namespace ONVIF {


//@ name=PTZPresetTourSpot
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API PTZPresetTourSpot
{
public:
	PTZPresetTourSpot();

	PTZPresetTourSpot(
		const PTZPresetTourPresetDetail& presetDetail, 
		const Poco::SharedPtr<PTZSpeed>& speed, 
		const Poco::Optional<std::string>& stayTime, 
		const Poco::SharedPtr<PTZPresetTourSpotExtension>& extension);

	virtual ~PTZPresetTourSpot();

	const Poco::SharedPtr<PTZPresetTourSpotExtension>& getExtension() const;

	const PTZPresetTourPresetDetail& getPresetDetail() const;

	const Poco::SharedPtr<PTZSpeed>& getSpeed() const;

	const Poco::Optional<std::string>& getStayTime() const;

	void setExtension(const Poco::SharedPtr<PTZPresetTourSpotExtension>& val);

	void setExtension(Poco::SharedPtr<PTZPresetTourSpotExtension>&& val);

	void setPresetDetail(const PTZPresetTourPresetDetail& val);

	void setPresetDetail(PTZPresetTourPresetDetail&& val);

	void setSpeed(const Poco::SharedPtr<PTZSpeed>& val);

	void setSpeed(Poco::SharedPtr<PTZSpeed>&& val);

	void setStayTime(const Poco::Optional<std::string>& val);

	void setStayTime(Poco::Optional<std::string>&& val);

private:
	//@ name=PresetDetail
	//@ order=0
	PTZPresetTourPresetDetail _presetDetail;

	//@ mandatory=false
	//@ name=Speed
	//@ order=1
	Poco::SharedPtr<PTZSpeed> _speed;

	//@ mandatory=false
	//@ name=StayTime
	//@ order=2
	Poco::Optional<std::string> _stayTime;

	//@ mandatory=false
	//@ name=Extension
	//@ order=3
	Poco::SharedPtr<PTZPresetTourSpotExtension> _extension;

};


inline const Poco::SharedPtr<PTZPresetTourSpotExtension>& PTZPresetTourSpot::getExtension() const
{
	return _extension;
}


inline const PTZPresetTourPresetDetail& PTZPresetTourSpot::getPresetDetail() const
{
	return _presetDetail;
}


inline const Poco::SharedPtr<PTZSpeed>& PTZPresetTourSpot::getSpeed() const
{
	return _speed;
}


inline const Poco::Optional<std::string>& PTZPresetTourSpot::getStayTime() const
{
	return _stayTime;
}


inline void PTZPresetTourSpot::setExtension(const Poco::SharedPtr<PTZPresetTourSpotExtension>& val)
{
	_extension = val;
}


inline void PTZPresetTourSpot::setExtension(Poco::SharedPtr<PTZPresetTourSpotExtension>&& val)
{
	_extension = std::move(val);
}


inline void PTZPresetTourSpot::setPresetDetail(const PTZPresetTourPresetDetail& val)
{
	_presetDetail = val;
}


inline void PTZPresetTourSpot::setPresetDetail(PTZPresetTourPresetDetail&& val)
{
	_presetDetail = std::move(val);
}


inline void PTZPresetTourSpot::setSpeed(const Poco::SharedPtr<PTZSpeed>& val)
{
	_speed = val;
}


inline void PTZPresetTourSpot::setSpeed(Poco::SharedPtr<PTZSpeed>&& val)
{
	_speed = std::move(val);
}


inline void PTZPresetTourSpot::setStayTime(const Poco::Optional<std::string>& val)
{
	_stayTime = val;
}


inline void PTZPresetTourSpot::setStayTime(Poco::Optional<std::string>&& val)
{
	_stayTime = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/PTZPresetTourSpotExtension.h"
#include "ONVIF/PTZSpeed.h"


#endif // ONVIF_PTZPresetTourSpot_INCLUDED
