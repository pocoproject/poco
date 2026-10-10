// 
// RecordingJobStateTrack.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_RecordingJobStateTrack_INCLUDED
#define ONVIF_RecordingJobStateTrack_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=RecordingJobStateTrack
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API RecordingJobStateTrack
{
public:
	RecordingJobStateTrack();

	RecordingJobStateTrack(
		const std::string& sourceTag, 
		const std::string& destination, 
		const Poco::Optional<std::string>& error, 
		const std::string& state);

	virtual ~RecordingJobStateTrack();

	const std::string& getDestination() const;

	const Poco::Optional<std::string>& getError() const;

	const std::string& getSourceTag() const;

	const std::string& getState() const;

	void setDestination(const std::string& val);

	void setDestination(std::string&& val);

	void setError(const Poco::Optional<std::string>& val);

	void setError(Poco::Optional<std::string>&& val);

	void setSourceTag(const std::string& val);

	void setSourceTag(std::string&& val);

	void setState(const std::string& val);

	void setState(std::string&& val);

private:
	//@ name=SourceTag
	//@ order=0
	std::string _sourceTag;

	//@ name=Destination
	//@ order=1
	std::string _destination;

	//@ mandatory=false
	//@ name=Error
	//@ order=2
	Poco::Optional<std::string> _error;

	//@ name=State
	//@ order=3
	std::string _state;

};


inline const std::string& RecordingJobStateTrack::getDestination() const
{
	return _destination;
}


inline const Poco::Optional<std::string>& RecordingJobStateTrack::getError() const
{
	return _error;
}


inline const std::string& RecordingJobStateTrack::getSourceTag() const
{
	return _sourceTag;
}


inline const std::string& RecordingJobStateTrack::getState() const
{
	return _state;
}


inline void RecordingJobStateTrack::setDestination(const std::string& val)
{
	_destination = val;
}


inline void RecordingJobStateTrack::setDestination(std::string&& val)
{
	_destination = std::move(val);
}


inline void RecordingJobStateTrack::setError(const Poco::Optional<std::string>& val)
{
	_error = val;
}


inline void RecordingJobStateTrack::setError(Poco::Optional<std::string>&& val)
{
	_error = std::move(val);
}


inline void RecordingJobStateTrack::setSourceTag(const std::string& val)
{
	_sourceTag = val;
}


inline void RecordingJobStateTrack::setSourceTag(std::string&& val)
{
	_sourceTag = std::move(val);
}


inline void RecordingJobStateTrack::setState(const std::string& val)
{
	_state = val;
}


inline void RecordingJobStateTrack::setState(std::string&& val)
{
	_state = std::move(val);
}


} // ONVIF


#endif // ONVIF_RecordingJobStateTrack_INCLUDED
