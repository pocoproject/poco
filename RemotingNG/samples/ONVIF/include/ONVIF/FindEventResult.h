// 
// FindEventResult.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_FindEventResult_INCLUDED
#define ONVIF_FindEventResult_INCLUDED


#include "OASIS/WSN/B2/NotificationMessageHolder.h"
#include "ONVIF/ONVIF.h"
#include "Poco/DateTime.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=FindEventResult
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API FindEventResult
{
public:
	FindEventResult();

	FindEventResult(
		const std::string& recordingToken, 
		const std::string& trackToken, 
		const Poco::DateTime& time, 
		const OASIS::WSN::B2::NotificationMessageHolder& event, 
		bool startStateEvent);

	virtual ~FindEventResult();

	const OASIS::WSN::B2::NotificationMessageHolder& getEvent() const;

	const std::string& getRecordingToken() const;

	bool getStartStateEvent() const;

	//@ return={xsdType=dateTime}
	const Poco::DateTime& getTime() const;

	const std::string& getTrackToken() const;

	void setEvent(const OASIS::WSN::B2::NotificationMessageHolder& val);

	void setEvent(OASIS::WSN::B2::NotificationMessageHolder&& val);

	void setRecordingToken(const std::string& val);

	void setRecordingToken(std::string&& val);

	void setStartStateEvent(bool val);

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

	//@ name=Event
	//@ order=3
	OASIS::WSN::B2::NotificationMessageHolder _event;

	//@ name=StartStateEvent
	//@ order=4
	bool _startStateEvent;

};


inline const OASIS::WSN::B2::NotificationMessageHolder& FindEventResult::getEvent() const
{
	return _event;
}


inline const std::string& FindEventResult::getRecordingToken() const
{
	return _recordingToken;
}


inline bool FindEventResult::getStartStateEvent() const
{
	return _startStateEvent;
}


inline const Poco::DateTime& FindEventResult::getTime() const
{
	return _time;
}


inline const std::string& FindEventResult::getTrackToken() const
{
	return _trackToken;
}


inline void FindEventResult::setEvent(const OASIS::WSN::B2::NotificationMessageHolder& val)
{
	_event = val;
}


inline void FindEventResult::setEvent(OASIS::WSN::B2::NotificationMessageHolder&& val)
{
	_event = std::move(val);
}


inline void FindEventResult::setRecordingToken(const std::string& val)
{
	_recordingToken = val;
}


inline void FindEventResult::setRecordingToken(std::string&& val)
{
	_recordingToken = std::move(val);
}


inline void FindEventResult::setStartStateEvent(bool val)
{
	_startStateEvent = val;
}


inline void FindEventResult::setTime(const Poco::DateTime& val)
{
	_time = val;
}


inline void FindEventResult::setTime(Poco::DateTime&& val)
{
	_time = std::move(val);
}


inline void FindEventResult::setTrackToken(const std::string& val)
{
	_trackToken = val;
}


inline void FindEventResult::setTrackToken(std::string&& val)
{
	_trackToken = std::move(val);
}


} // ONVIF


#endif // ONVIF_FindEventResult_INCLUDED
