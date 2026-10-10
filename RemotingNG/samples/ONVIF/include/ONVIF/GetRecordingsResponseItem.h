// 
// GetRecordingsResponseItem.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_GetRecordingsResponseItem_INCLUDED
#define ONVIF_GetRecordingsResponseItem_INCLUDED


#include "ONVIF/GetTracksResponseList.h"
#include "ONVIF/ONVIF.h"
#include "ONVIF/RecordingConfiguration.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=GetRecordingsResponseItem
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API GetRecordingsResponseItem
{
public:
	GetRecordingsResponseItem();

	GetRecordingsResponseItem(
		const std::string& recordingToken, 
		const RecordingConfiguration& configuration, 
		const GetTracksResponseList& tracks);

	virtual ~GetRecordingsResponseItem();

	const RecordingConfiguration& getConfiguration() const;

	const std::string& getRecordingToken() const;

	const GetTracksResponseList& getTracks() const;

	void setConfiguration(const RecordingConfiguration& val);

	void setConfiguration(RecordingConfiguration&& val);

	void setRecordingToken(const std::string& val);

	void setRecordingToken(std::string&& val);

	void setTracks(const GetTracksResponseList& val);

	void setTracks(GetTracksResponseList&& val);

private:
	//@ name=RecordingToken
	//@ order=0
	std::string _recordingToken;

	//@ name=Configuration
	//@ order=1
	RecordingConfiguration _configuration;

	//@ name=Tracks
	//@ order=2
	GetTracksResponseList _tracks;

};


inline const RecordingConfiguration& GetRecordingsResponseItem::getConfiguration() const
{
	return _configuration;
}


inline const std::string& GetRecordingsResponseItem::getRecordingToken() const
{
	return _recordingToken;
}


inline const GetTracksResponseList& GetRecordingsResponseItem::getTracks() const
{
	return _tracks;
}


inline void GetRecordingsResponseItem::setConfiguration(const RecordingConfiguration& val)
{
	_configuration = val;
}


inline void GetRecordingsResponseItem::setConfiguration(RecordingConfiguration&& val)
{
	_configuration = std::move(val);
}


inline void GetRecordingsResponseItem::setRecordingToken(const std::string& val)
{
	_recordingToken = val;
}


inline void GetRecordingsResponseItem::setRecordingToken(std::string&& val)
{
	_recordingToken = std::move(val);
}


inline void GetRecordingsResponseItem::setTracks(const GetTracksResponseList& val)
{
	_tracks = val;
}


inline void GetRecordingsResponseItem::setTracks(GetTracksResponseList&& val)
{
	_tracks = std::move(val);
}


} // ONVIF


#endif // ONVIF_GetRecordingsResponseItem_INCLUDED
