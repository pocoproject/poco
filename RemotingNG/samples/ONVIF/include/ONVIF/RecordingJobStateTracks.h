// 
// RecordingJobStateTracks.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_RecordingJobStateTracks_INCLUDED
#define ONVIF_RecordingJobStateTracks_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/RecordingJobStateTrack.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=RecordingJobStateTracks
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API RecordingJobStateTracks
{
public:
	RecordingJobStateTracks();

	RecordingJobStateTracks(const std::vector<RecordingJobStateTrack>& track);

	virtual ~RecordingJobStateTracks();

	const std::vector<RecordingJobStateTrack>& getTrack() const;

	std::vector<RecordingJobStateTrack>& getTrack();

	void setTrack(const std::vector<RecordingJobStateTrack>& val);

	void setTrack(std::vector<RecordingJobStateTrack>&& val);

private:
	//@ mandatory=false
	//@ name=Track
	//@ order=0
	std::vector<RecordingJobStateTrack> _track;

};


inline const std::vector<RecordingJobStateTrack>& RecordingJobStateTracks::getTrack() const
{
	return _track;
}


inline std::vector<RecordingJobStateTrack>& RecordingJobStateTracks::getTrack()
{
	return _track;
}


inline void RecordingJobStateTracks::setTrack(const std::vector<RecordingJobStateTrack>& val)
{
	_track = val;
}


inline void RecordingJobStateTracks::setTrack(std::vector<RecordingJobStateTrack>&& val)
{
	_track = std::move(val);
}


} // ONVIF


#endif // ONVIF_RecordingJobStateTracks_INCLUDED
