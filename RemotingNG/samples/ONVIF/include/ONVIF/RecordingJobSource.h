// 
// RecordingJobSource.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_RecordingJobSource_INCLUDED
#define ONVIF_RecordingJobSource_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/RecordingJobTrack.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class RecordingJobSourceExtension;
class SourceReference;
} 


namespace ONVIF {


//@ name=RecordingJobSource
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API RecordingJobSource
{
public:
	RecordingJobSource();

	RecordingJobSource(
		const Poco::SharedPtr<SourceReference>& sourceToken, 
		Poco::Optional<bool> autoCreateReceiver, 
		const std::vector<RecordingJobTrack>& tracks, 
		const Poco::SharedPtr<RecordingJobSourceExtension>& extension);

	virtual ~RecordingJobSource();

	Poco::Optional<bool> getAutoCreateReceiver() const;

	const Poco::SharedPtr<RecordingJobSourceExtension>& getExtension() const;

	const Poco::SharedPtr<SourceReference>& getSourceToken() const;

	const std::vector<RecordingJobTrack>& getTracks() const;

	std::vector<RecordingJobTrack>& getTracks();

	void setAutoCreateReceiver(Poco::Optional<bool> val);

	void setExtension(const Poco::SharedPtr<RecordingJobSourceExtension>& val);

	void setExtension(Poco::SharedPtr<RecordingJobSourceExtension>&& val);

	void setSourceToken(const Poco::SharedPtr<SourceReference>& val);

	void setSourceToken(Poco::SharedPtr<SourceReference>&& val);

	void setTracks(const std::vector<RecordingJobTrack>& val);

	void setTracks(std::vector<RecordingJobTrack>&& val);

private:
	//@ mandatory=false
	//@ name=SourceToken
	//@ order=0
	Poco::SharedPtr<SourceReference> _sourceToken;

	//@ mandatory=false
	//@ name=AutoCreateReceiver
	//@ order=1
	Poco::Optional<bool> _autoCreateReceiver;

	//@ mandatory=false
	//@ name=Tracks
	//@ order=2
	std::vector<RecordingJobTrack> _tracks;

	//@ mandatory=false
	//@ name=Extension
	//@ order=3
	Poco::SharedPtr<RecordingJobSourceExtension> _extension;

};


inline Poco::Optional<bool> RecordingJobSource::getAutoCreateReceiver() const
{
	return _autoCreateReceiver;
}


inline const Poco::SharedPtr<RecordingJobSourceExtension>& RecordingJobSource::getExtension() const
{
	return _extension;
}


inline const Poco::SharedPtr<SourceReference>& RecordingJobSource::getSourceToken() const
{
	return _sourceToken;
}


inline const std::vector<RecordingJobTrack>& RecordingJobSource::getTracks() const
{
	return _tracks;
}


inline std::vector<RecordingJobTrack>& RecordingJobSource::getTracks()
{
	return _tracks;
}


inline void RecordingJobSource::setAutoCreateReceiver(Poco::Optional<bool> val)
{
	_autoCreateReceiver = val;
}


inline void RecordingJobSource::setExtension(const Poco::SharedPtr<RecordingJobSourceExtension>& val)
{
	_extension = val;
}


inline void RecordingJobSource::setExtension(Poco::SharedPtr<RecordingJobSourceExtension>&& val)
{
	_extension = std::move(val);
}


inline void RecordingJobSource::setSourceToken(const Poco::SharedPtr<SourceReference>& val)
{
	_sourceToken = val;
}


inline void RecordingJobSource::setSourceToken(Poco::SharedPtr<SourceReference>&& val)
{
	_sourceToken = std::move(val);
}


inline void RecordingJobSource::setTracks(const std::vector<RecordingJobTrack>& val)
{
	_tracks = val;
}


inline void RecordingJobSource::setTracks(std::vector<RecordingJobTrack>&& val)
{
	_tracks = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/RecordingJobSourceExtension.h"
#include "ONVIF/SourceReference.h"


#endif // ONVIF_RecordingJobSource_INCLUDED
