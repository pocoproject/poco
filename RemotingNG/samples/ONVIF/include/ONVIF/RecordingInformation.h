// 
// RecordingInformation.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_RecordingInformation_INCLUDED
#define ONVIF_RecordingInformation_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/RecordingSourceInformation.h"
#include "ONVIF/TrackInformation.h"
#include "Poco/DateTime.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=RecordingInformation
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API RecordingInformation
{
public:
	RecordingInformation();

	RecordingInformation(
		const std::string& recordingToken, 
		const RecordingSourceInformation& source, 
		const Poco::Optional<Poco::DateTime>& earliestRecording, 
		const Poco::Optional<Poco::DateTime>& latestRecording, 
		const std::string& content, 
		const std::vector<TrackInformation>& track, 
		const std::string& recordingStatus);

	virtual ~RecordingInformation();

	const std::string& getContent() const;

	//@ return={xsdType=dateTime}
	const Poco::Optional<Poco::DateTime>& getEarliestRecording() const;

	//@ return={xsdType=dateTime}
	const Poco::Optional<Poco::DateTime>& getLatestRecording() const;

	const std::string& getRecordingStatus() const;

	const std::string& getRecordingToken() const;

	const RecordingSourceInformation& getSource() const;

	const std::vector<TrackInformation>& getTrack() const;

	std::vector<TrackInformation>& getTrack();

	void setContent(const std::string& val);

	void setContent(std::string&& val);

	//@ $val={xsdType=dateTime}
	void setEarliestRecording(const Poco::Optional<Poco::DateTime>& val);

	//@ $val={xsdType=dateTime}
	void setEarliestRecording(Poco::Optional<Poco::DateTime>&& val);

	//@ $val={xsdType=dateTime}
	void setLatestRecording(const Poco::Optional<Poco::DateTime>& val);

	//@ $val={xsdType=dateTime}
	void setLatestRecording(Poco::Optional<Poco::DateTime>&& val);

	void setRecordingStatus(const std::string& val);

	void setRecordingStatus(std::string&& val);

	void setRecordingToken(const std::string& val);

	void setRecordingToken(std::string&& val);

	void setSource(const RecordingSourceInformation& val);

	void setSource(RecordingSourceInformation&& val);

	void setTrack(const std::vector<TrackInformation>& val);

	void setTrack(std::vector<TrackInformation>&& val);

private:
	//@ name=RecordingToken
	//@ order=0
	std::string _recordingToken;

	//@ name=Source
	//@ order=1
	RecordingSourceInformation _source;

	//@ mandatory=false
	//@ name=EarliestRecording
	//@ order=2
	//@ xsdType=dateTime
	Poco::Optional<Poco::DateTime> _earliestRecording;

	//@ mandatory=false
	//@ name=LatestRecording
	//@ order=3
	//@ xsdType=dateTime
	Poco::Optional<Poco::DateTime> _latestRecording;

	//@ name=Content
	//@ order=4
	std::string _content;

	//@ mandatory=false
	//@ name=Track
	//@ order=5
	std::vector<TrackInformation> _track;

	//@ name=RecordingStatus
	//@ order=6
	std::string _recordingStatus;

};


inline const std::string& RecordingInformation::getContent() const
{
	return _content;
}


inline const Poco::Optional<Poco::DateTime>& RecordingInformation::getEarliestRecording() const
{
	return _earliestRecording;
}


inline const Poco::Optional<Poco::DateTime>& RecordingInformation::getLatestRecording() const
{
	return _latestRecording;
}


inline const std::string& RecordingInformation::getRecordingStatus() const
{
	return _recordingStatus;
}


inline const std::string& RecordingInformation::getRecordingToken() const
{
	return _recordingToken;
}


inline const RecordingSourceInformation& RecordingInformation::getSource() const
{
	return _source;
}


inline const std::vector<TrackInformation>& RecordingInformation::getTrack() const
{
	return _track;
}


inline std::vector<TrackInformation>& RecordingInformation::getTrack()
{
	return _track;
}


inline void RecordingInformation::setContent(const std::string& val)
{
	_content = val;
}


inline void RecordingInformation::setContent(std::string&& val)
{
	_content = std::move(val);
}


inline void RecordingInformation::setEarliestRecording(const Poco::Optional<Poco::DateTime>& val)
{
	_earliestRecording = val;
}


inline void RecordingInformation::setEarliestRecording(Poco::Optional<Poco::DateTime>&& val)
{
	_earliestRecording = std::move(val);
}


inline void RecordingInformation::setLatestRecording(const Poco::Optional<Poco::DateTime>& val)
{
	_latestRecording = val;
}


inline void RecordingInformation::setLatestRecording(Poco::Optional<Poco::DateTime>&& val)
{
	_latestRecording = std::move(val);
}


inline void RecordingInformation::setRecordingStatus(const std::string& val)
{
	_recordingStatus = val;
}


inline void RecordingInformation::setRecordingStatus(std::string&& val)
{
	_recordingStatus = std::move(val);
}


inline void RecordingInformation::setRecordingToken(const std::string& val)
{
	_recordingToken = val;
}


inline void RecordingInformation::setRecordingToken(std::string&& val)
{
	_recordingToken = std::move(val);
}


inline void RecordingInformation::setSource(const RecordingSourceInformation& val)
{
	_source = val;
}


inline void RecordingInformation::setSource(RecordingSourceInformation&& val)
{
	_source = std::move(val);
}


inline void RecordingInformation::setTrack(const std::vector<TrackInformation>& val)
{
	_track = val;
}


inline void RecordingInformation::setTrack(std::vector<TrackInformation>&& val)
{
	_track = std::move(val);
}


} // ONVIF


#endif // ONVIF_RecordingInformation_INCLUDED
