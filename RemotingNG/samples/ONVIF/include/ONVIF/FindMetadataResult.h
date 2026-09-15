// 
// FindMetadataResult.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_FindMetadataResult_INCLUDED
#define ONVIF_FindMetadataResult_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/DateTime.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=FindMetadataResult
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API FindMetadataResult
{
public:
	FindMetadataResult();

	FindMetadataResult(
		const std::string& recordingToken, 
		const std::string& trackToken, 
		const Poco::DateTime& time);

	virtual ~FindMetadataResult();

	const std::string& getRecordingToken() const;

	//@ return={xsdType=dateTime}
	const Poco::DateTime& getTime() const;

	const std::string& getTrackToken() const;

	void setRecordingToken(const std::string& val);

	void setRecordingToken(std::string&& val);

	//@ $val={xsdType=dateTime}
	void setTime(const Poco::DateTime& val);

	//@ $val={xsdType=dateTime}
	void setTime(Poco::DateTime&& val);

	void setTrackToken(const std::string& val);

	void setTrackToken(std::string&& val);

private:
	//@ name=RecordingToken
	//@ order=0
	std::string _recordingToken;

	//@ name=TrackToken
	//@ order=1
	std::string _trackToken;

	//@ name=Time
	//@ order=2
	//@ xsdType=dateTime
	Poco::DateTime _time;

};


inline const std::string& FindMetadataResult::getRecordingToken() const
{
	return _recordingToken;
}


inline const Poco::DateTime& FindMetadataResult::getTime() const
{
	return _time;
}


inline const std::string& FindMetadataResult::getTrackToken() const
{
	return _trackToken;
}


inline void FindMetadataResult::setRecordingToken(const std::string& val)
{
	_recordingToken = val;
}


inline void FindMetadataResult::setRecordingToken(std::string&& val)
{
	_recordingToken = std::move(val);
}


inline void FindMetadataResult::setTime(const Poco::DateTime& val)
{
	_time = val;
}


inline void FindMetadataResult::setTime(Poco::DateTime&& val)
{
	_time = std::move(val);
}


inline void FindMetadataResult::setTrackToken(const std::string& val)
{
	_trackToken = val;
}


inline void FindMetadataResult::setTrackToken(std::string&& val)
{
	_trackToken = std::move(val);
}


} // ONVIF


#endif // ONVIF_FindMetadataResult_INCLUDED
