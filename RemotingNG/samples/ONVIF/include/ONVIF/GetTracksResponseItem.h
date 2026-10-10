// 
// GetTracksResponseItem.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_GetTracksResponseItem_INCLUDED
#define ONVIF_GetTracksResponseItem_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/TrackConfiguration.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=GetTracksResponseItem
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API GetTracksResponseItem
{
public:
	GetTracksResponseItem();

	GetTracksResponseItem(
		const std::string& trackToken, 
		const TrackConfiguration& configuration);

	virtual ~GetTracksResponseItem();

	const TrackConfiguration& getConfiguration() const;

	const std::string& getTrackToken() const;

	void setConfiguration(const TrackConfiguration& val);

	void setConfiguration(TrackConfiguration&& val);

	void setTrackToken(const std::string& val);

	void setTrackToken(std::string&& val);

private:
	//@ name=TrackToken
	//@ order=0
	std::string _trackToken;

	//@ name=Configuration
	//@ order=1
	TrackConfiguration _configuration;

};


inline const TrackConfiguration& GetTracksResponseItem::getConfiguration() const
{
	return _configuration;
}


inline const std::string& GetTracksResponseItem::getTrackToken() const
{
	return _trackToken;
}


inline void GetTracksResponseItem::setConfiguration(const TrackConfiguration& val)
{
	_configuration = val;
}


inline void GetTracksResponseItem::setConfiguration(TrackConfiguration&& val)
{
	_configuration = std::move(val);
}


inline void GetTracksResponseItem::setTrackToken(const std::string& val)
{
	_trackToken = val;
}


inline void GetTracksResponseItem::setTrackToken(std::string&& val)
{
	_trackToken = std::move(val);
}


} // ONVIF


#endif // ONVIF_GetTracksResponseItem_INCLUDED
