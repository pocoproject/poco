// 
// PresetTour.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_PresetTour_INCLUDED
#define ONVIF_PresetTour_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/PTZPresetTourSpot.h"
#include "ONVIF/PTZPresetTourStartingCondition.h"
#include "ONVIF/PTZPresetTourStatus.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class PTZPresetTourExtension;
} 


namespace ONVIF {


//@ name=PresetTour
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API PresetTour
{
public:
	PresetTour();

	PresetTour(
		const Poco::Optional<std::string>& token, 
		const Poco::Optional<std::string>& name, 
		const PTZPresetTourStatus& status, 
		bool autoStart, 
		const PTZPresetTourStartingCondition& startingCondition, 
		const std::vector<PTZPresetTourSpot>& tourSpot, 
		const Poco::SharedPtr<PTZPresetTourExtension>& extension);

	virtual ~PresetTour();

	bool getAutoStart() const;

	const Poco::SharedPtr<PTZPresetTourExtension>& getExtension() const;

	const Poco::Optional<std::string>& getName() const;

	const PTZPresetTourStartingCondition& getStartingCondition() const;

	const PTZPresetTourStatus& getStatus() const;

	const Poco::Optional<std::string>& getToken() const;

	const std::vector<PTZPresetTourSpot>& getTourSpot() const;

	std::vector<PTZPresetTourSpot>& getTourSpot();

	void setAutoStart(bool val);

	void setExtension(const Poco::SharedPtr<PTZPresetTourExtension>& val);

	void setExtension(Poco::SharedPtr<PTZPresetTourExtension>&& val);

	void setName(const Poco::Optional<std::string>& val);

	void setName(Poco::Optional<std::string>&& val);

	void setStartingCondition(const PTZPresetTourStartingCondition& val);

	void setStartingCondition(PTZPresetTourStartingCondition&& val);

	void setStatus(const PTZPresetTourStatus& val);

	void setStatus(PTZPresetTourStatus&& val);

	void setToken(const Poco::Optional<std::string>& val);

	void setToken(Poco::Optional<std::string>&& val);

	void setTourSpot(const std::vector<PTZPresetTourSpot>& val);

	void setTourSpot(std::vector<PTZPresetTourSpot>&& val);

private:
	//@ mandatory=false
	//@ name=token
	//@ order=0
	//@ type=attr
	Poco::Optional<std::string> _token;

	//@ mandatory=false
	//@ name=Name
	//@ order=1
	Poco::Optional<std::string> _name;

	//@ name=Status
	//@ order=2
	PTZPresetTourStatus _status;

	//@ name=AutoStart
	//@ order=3
	bool _autoStart;

	//@ name=StartingCondition
	//@ order=4
	PTZPresetTourStartingCondition _startingCondition;

	//@ mandatory=false
	//@ name=TourSpot
	//@ order=5
	std::vector<PTZPresetTourSpot> _tourSpot;

	//@ mandatory=false
	//@ name=Extension
	//@ order=6
	Poco::SharedPtr<PTZPresetTourExtension> _extension;

};


inline bool PresetTour::getAutoStart() const
{
	return _autoStart;
}


inline const Poco::SharedPtr<PTZPresetTourExtension>& PresetTour::getExtension() const
{
	return _extension;
}


inline const Poco::Optional<std::string>& PresetTour::getName() const
{
	return _name;
}


inline const PTZPresetTourStartingCondition& PresetTour::getStartingCondition() const
{
	return _startingCondition;
}


inline const PTZPresetTourStatus& PresetTour::getStatus() const
{
	return _status;
}


inline const Poco::Optional<std::string>& PresetTour::getToken() const
{
	return _token;
}


inline const std::vector<PTZPresetTourSpot>& PresetTour::getTourSpot() const
{
	return _tourSpot;
}


inline std::vector<PTZPresetTourSpot>& PresetTour::getTourSpot()
{
	return _tourSpot;
}


inline void PresetTour::setAutoStart(bool val)
{
	_autoStart = val;
}


inline void PresetTour::setExtension(const Poco::SharedPtr<PTZPresetTourExtension>& val)
{
	_extension = val;
}


inline void PresetTour::setExtension(Poco::SharedPtr<PTZPresetTourExtension>&& val)
{
	_extension = std::move(val);
}


inline void PresetTour::setName(const Poco::Optional<std::string>& val)
{
	_name = val;
}


inline void PresetTour::setName(Poco::Optional<std::string>&& val)
{
	_name = std::move(val);
}


inline void PresetTour::setStartingCondition(const PTZPresetTourStartingCondition& val)
{
	_startingCondition = val;
}


inline void PresetTour::setStartingCondition(PTZPresetTourStartingCondition&& val)
{
	_startingCondition = std::move(val);
}


inline void PresetTour::setStatus(const PTZPresetTourStatus& val)
{
	_status = val;
}


inline void PresetTour::setStatus(PTZPresetTourStatus&& val)
{
	_status = std::move(val);
}


inline void PresetTour::setToken(const Poco::Optional<std::string>& val)
{
	_token = val;
}


inline void PresetTour::setToken(Poco::Optional<std::string>&& val)
{
	_token = std::move(val);
}


inline void PresetTour::setTourSpot(const std::vector<PTZPresetTourSpot>& val)
{
	_tourSpot = val;
}


inline void PresetTour::setTourSpot(std::vector<PTZPresetTourSpot>&& val)
{
	_tourSpot = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/PTZPresetTourExtension.h"


#endif // ONVIF_PresetTour_INCLUDED
