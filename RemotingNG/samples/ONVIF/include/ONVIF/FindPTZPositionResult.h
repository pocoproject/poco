// 
// FindPTZPositionResult.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_FindPTZPositionResult_INCLUDED
#define ONVIF_FindPTZPositionResult_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/PTZVector.h"
#include "Poco/DateTime.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=FindPTZPositionResult
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API FindPTZPositionResult
{
public:
	FindPTZPositionResult();

	FindPTZPositionResult(
		const std::string& recordingToken, 
		const std::string& trackToken, 
		const Poco::DateTime& time, 
		const PTZVector& position);

	virtual ~FindPTZPositionResult();

	const PTZVector& getPosition() const;

	const std::string& getRecordingToken() const;

	//@ return={xsdType=dateTime}
	const Poco::DateTime& getTime() const;

	const std::string& getTrackToken() const;

	void setPosition(const PTZVector& val);

	void setPosition(PTZVector&& val);

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

	//@ name=Position
	//@ order=3
	PTZVector _position;

};


inline const PTZVector& FindPTZPositionResult::getPosition() const
{
	return _position;
}


inline const std::string& FindPTZPositionResult::getRecordingToken() const
{
	return _recordingToken;
}


inline const Poco::DateTime& FindPTZPositionResult::getTime() const
{
	return _time;
}


inline const std::string& FindPTZPositionResult::getTrackToken() const
{
	return _trackToken;
}


inline void FindPTZPositionResult::setPosition(const PTZVector& val)
{
	_position = val;
}


inline void FindPTZPositionResult::setPosition(PTZVector&& val)
{
	_position = std::move(val);
}


inline void FindPTZPositionResult::setRecordingToken(const std::string& val)
{
	_recordingToken = val;
}


inline void FindPTZPositionResult::setRecordingToken(std::string&& val)
{
	_recordingToken = std::move(val);
}


inline void FindPTZPositionResult::setTime(const Poco::DateTime& val)
{
	_time = val;
}


inline void FindPTZPositionResult::setTime(Poco::DateTime&& val)
{
	_time = std::move(val);
}


inline void FindPTZPositionResult::setTrackToken(const std::string& val)
{
	_trackToken = val;
}


inline void FindPTZPositionResult::setTrackToken(std::string&& val)
{
	_trackToken = std::move(val);
}


} // ONVIF


#endif // ONVIF_FindPTZPositionResult_INCLUDED
