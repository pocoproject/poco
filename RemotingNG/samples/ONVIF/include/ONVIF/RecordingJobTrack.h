// 
// RecordingJobTrack.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_RecordingJobTrack_INCLUDED
#define ONVIF_RecordingJobTrack_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=RecordingJobTrack
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API RecordingJobTrack
{
public:
	RecordingJobTrack();

	RecordingJobTrack(
		const std::string& sourceTag, 
		const std::string& destination);

	virtual ~RecordingJobTrack();

	const std::string& getDestination() const;

	const std::string& getSourceTag() const;

	void setDestination(const std::string& val);

	void setDestination(std::string&& val);

	void setSourceTag(const std::string& val);

	void setSourceTag(std::string&& val);

private:
	//@ name=SourceTag
	//@ order=0
	std::string _sourceTag;

	//@ name=Destination
	//@ order=1
	std::string _destination;

};


inline const std::string& RecordingJobTrack::getDestination() const
{
	return _destination;
}


inline const std::string& RecordingJobTrack::getSourceTag() const
{
	return _sourceTag;
}


inline void RecordingJobTrack::setDestination(const std::string& val)
{
	_destination = val;
}


inline void RecordingJobTrack::setDestination(std::string&& val)
{
	_destination = std::move(val);
}


inline void RecordingJobTrack::setSourceTag(const std::string& val)
{
	_sourceTag = val;
}


inline void RecordingJobTrack::setSourceTag(std::string&& val)
{
	_sourceTag = std::move(val);
}


} // ONVIF


#endif // ONVIF_RecordingJobTrack_INCLUDED
