// 
// RecordingJobStateSource.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_RecordingJobStateSource_INCLUDED
#define ONVIF_RecordingJobStateSource_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/RecordingJobStateTracks.h"
#include "ONVIF/SourceReference.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=RecordingJobStateSource
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API RecordingJobStateSource
{
public:
	RecordingJobStateSource();

	RecordingJobStateSource(
		const SourceReference& sourceToken, 
		const std::string& state, 
		const RecordingJobStateTracks& tracks);

	virtual ~RecordingJobStateSource();

	const SourceReference& getSourceToken() const;

	const std::string& getState() const;

	const RecordingJobStateTracks& getTracks() const;

	void setSourceToken(const SourceReference& val);

	void setSourceToken(SourceReference&& val);

	void setState(const std::string& val);

	void setState(std::string&& val);

	void setTracks(const RecordingJobStateTracks& val);

	void setTracks(RecordingJobStateTracks&& val);

private:
	//@ name=SourceToken
	//@ order=0
	SourceReference _sourceToken;

	//@ name=State
	//@ order=1
	std::string _state;

	//@ name=Tracks
	//@ order=2
	RecordingJobStateTracks _tracks;

};


inline const SourceReference& RecordingJobStateSource::getSourceToken() const
{
	return _sourceToken;
}


inline const std::string& RecordingJobStateSource::getState() const
{
	return _state;
}


inline const RecordingJobStateTracks& RecordingJobStateSource::getTracks() const
{
	return _tracks;
}


inline void RecordingJobStateSource::setSourceToken(const SourceReference& val)
{
	_sourceToken = val;
}


inline void RecordingJobStateSource::setSourceToken(SourceReference&& val)
{
	_sourceToken = std::move(val);
}


inline void RecordingJobStateSource::setState(const std::string& val)
{
	_state = val;
}


inline void RecordingJobStateSource::setState(std::string&& val)
{
	_state = std::move(val);
}


inline void RecordingJobStateSource::setTracks(const RecordingJobStateTracks& val)
{
	_tracks = val;
}


inline void RecordingJobStateSource::setTracks(RecordingJobStateTracks&& val)
{
	_tracks = std::move(val);
}


} // ONVIF


#endif // ONVIF_RecordingJobStateSource_INCLUDED
