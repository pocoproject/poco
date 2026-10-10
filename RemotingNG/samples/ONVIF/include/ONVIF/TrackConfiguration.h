// 
// TrackConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_TrackConfiguration_INCLUDED
#define ONVIF_TrackConfiguration_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=TrackConfiguration
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API TrackConfiguration
{
public:
	TrackConfiguration();

	TrackConfiguration(
		const std::string& trackType, 
		const std::string& description);

	virtual ~TrackConfiguration();

	const std::string& getDescription() const;

	const std::string& getTrackType() const;

	void setDescription(const std::string& val);

	void setDescription(std::string&& val);

	void setTrackType(const std::string& val);

	void setTrackType(std::string&& val);

private:
	//@ name=TrackType
	//@ order=0
	std::string _trackType;

	//@ name=Description
	//@ order=1
	std::string _description;

};


inline const std::string& TrackConfiguration::getDescription() const
{
	return _description;
}


inline const std::string& TrackConfiguration::getTrackType() const
{
	return _trackType;
}


inline void TrackConfiguration::setDescription(const std::string& val)
{
	_description = val;
}


inline void TrackConfiguration::setDescription(std::string&& val)
{
	_description = std::move(val);
}


inline void TrackConfiguration::setTrackType(const std::string& val)
{
	_trackType = val;
}


inline void TrackConfiguration::setTrackType(std::string&& val)
{
	_trackType = std::move(val);
}


} // ONVIF


#endif // ONVIF_TrackConfiguration_INCLUDED
