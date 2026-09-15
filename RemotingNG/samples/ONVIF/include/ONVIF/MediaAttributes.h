// 
// MediaAttributes.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_MediaAttributes_INCLUDED
#define ONVIF_MediaAttributes_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/TrackAttributes.h"
#include "Poco/DateTime.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=MediaAttributes
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API MediaAttributes
{
public:
	MediaAttributes();

	MediaAttributes(
		const std::string& recordingToken, 
		const std::vector<TrackAttributes>& trackAttributes, 
		const Poco::DateTime& from, 
		const Poco::DateTime& until);

	virtual ~MediaAttributes();

	//@ return={xsdType=dateTime}
	const Poco::DateTime& getFrom() const;

	const std::string& getRecordingToken() const;

	const std::vector<TrackAttributes>& getTrackAttributes() const;

	std::vector<TrackAttributes>& getTrackAttributes();

	//@ return={xsdType=dateTime}
	const Poco::DateTime& getUntil() const;

	//@ $val={xsdType=dateTime}
	void setFrom(const Poco::DateTime& val);

	//@ $val={xsdType=dateTime}
	void setFrom(Poco::DateTime&& val);

	void setRecordingToken(const std::string& val);

	void setRecordingToken(std::string&& val);

	void setTrackAttributes(const std::vector<TrackAttributes>& val);

	void setTrackAttributes(std::vector<TrackAttributes>&& val);

	//@ $val={xsdType=dateTime}
	void setUntil(const Poco::DateTime& val);

	//@ $val={xsdType=dateTime}
	void setUntil(Poco::DateTime&& val);

private:
	//@ name=RecordingToken
	//@ order=0
	std::string _recordingToken;

	//@ mandatory=false
	//@ name=TrackAttributes
	//@ order=1
	std::vector<TrackAttributes> _trackAttributes;

	//@ name=From
	//@ order=2
	//@ xsdType=dateTime
	Poco::DateTime _from;

	//@ name=Until
	//@ order=3
	//@ xsdType=dateTime
	Poco::DateTime _until;

};


inline const Poco::DateTime& MediaAttributes::getFrom() const
{
	return _from;
}


inline const std::string& MediaAttributes::getRecordingToken() const
{
	return _recordingToken;
}


inline const std::vector<TrackAttributes>& MediaAttributes::getTrackAttributes() const
{
	return _trackAttributes;
}


inline std::vector<TrackAttributes>& MediaAttributes::getTrackAttributes()
{
	return _trackAttributes;
}


inline const Poco::DateTime& MediaAttributes::getUntil() const
{
	return _until;
}


inline void MediaAttributes::setFrom(const Poco::DateTime& val)
{
	_from = val;
}


inline void MediaAttributes::setFrom(Poco::DateTime&& val)
{
	_from = std::move(val);
}


inline void MediaAttributes::setRecordingToken(const std::string& val)
{
	_recordingToken = val;
}


inline void MediaAttributes::setRecordingToken(std::string&& val)
{
	_recordingToken = std::move(val);
}


inline void MediaAttributes::setTrackAttributes(const std::vector<TrackAttributes>& val)
{
	_trackAttributes = val;
}


inline void MediaAttributes::setTrackAttributes(std::vector<TrackAttributes>&& val)
{
	_trackAttributes = std::move(val);
}


inline void MediaAttributes::setUntil(const Poco::DateTime& val)
{
	_until = val;
}


inline void MediaAttributes::setUntil(Poco::DateTime&& val)
{
	_until = std::move(val);
}


} // ONVIF


#endif // ONVIF_MediaAttributes_INCLUDED
