// 
// PTZPresetTourStatus.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_PTZPresetTourStatus_INCLUDED
#define ONVIF_PTZPresetTourStatus_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class PTZPresetTourSpot;
class PTZPresetTourStatusExtension;
} 


namespace ONVIF {


//@ name=PTZPresetTourStatus
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API PTZPresetTourStatus
{
public:
	PTZPresetTourStatus();

	PTZPresetTourStatus(
		const std::string& state, 
		const Poco::SharedPtr<PTZPresetTourSpot>& currentTourSpot, 
		const Poco::SharedPtr<PTZPresetTourStatusExtension>& extension);

	virtual ~PTZPresetTourStatus();

	const Poco::SharedPtr<PTZPresetTourSpot>& getCurrentTourSpot() const;

	const Poco::SharedPtr<PTZPresetTourStatusExtension>& getExtension() const;

	const std::string& getState() const;

	void setCurrentTourSpot(const Poco::SharedPtr<PTZPresetTourSpot>& val);

	void setCurrentTourSpot(Poco::SharedPtr<PTZPresetTourSpot>&& val);

	void setExtension(const Poco::SharedPtr<PTZPresetTourStatusExtension>& val);

	void setExtension(Poco::SharedPtr<PTZPresetTourStatusExtension>&& val);

	void setState(const std::string& val);

	void setState(std::string&& val);

private:
	//@ name=State
	//@ order=0
	std::string _state;

	//@ mandatory=false
	//@ name=CurrentTourSpot
	//@ order=1
	Poco::SharedPtr<PTZPresetTourSpot> _currentTourSpot;

	//@ mandatory=false
	//@ name=Extension
	//@ order=2
	Poco::SharedPtr<PTZPresetTourStatusExtension> _extension;

};


inline const Poco::SharedPtr<PTZPresetTourSpot>& PTZPresetTourStatus::getCurrentTourSpot() const
{
	return _currentTourSpot;
}


inline const Poco::SharedPtr<PTZPresetTourStatusExtension>& PTZPresetTourStatus::getExtension() const
{
	return _extension;
}


inline const std::string& PTZPresetTourStatus::getState() const
{
	return _state;
}


inline void PTZPresetTourStatus::setCurrentTourSpot(const Poco::SharedPtr<PTZPresetTourSpot>& val)
{
	_currentTourSpot = val;
}


inline void PTZPresetTourStatus::setCurrentTourSpot(Poco::SharedPtr<PTZPresetTourSpot>&& val)
{
	_currentTourSpot = std::move(val);
}


inline void PTZPresetTourStatus::setExtension(const Poco::SharedPtr<PTZPresetTourStatusExtension>& val)
{
	_extension = val;
}


inline void PTZPresetTourStatus::setExtension(Poco::SharedPtr<PTZPresetTourStatusExtension>&& val)
{
	_extension = std::move(val);
}


inline void PTZPresetTourStatus::setState(const std::string& val)
{
	_state = val;
}


inline void PTZPresetTourStatus::setState(std::string&& val)
{
	_state = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/PTZPresetTourSpot.h"
#include "ONVIF/PTZPresetTourStatusExtension.h"


#endif // ONVIF_PTZPresetTourStatus_INCLUDED
