// 
// TrackAttributes.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_TrackAttributes_INCLUDED
#define ONVIF_TrackAttributes_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/TrackInformation.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class AudioAttributes;
class MetadataAttributes;
class TrackAttributesExtension;
class VideoAttributes;
} 


namespace ONVIF {


//@ name=TrackAttributes
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API TrackAttributes
{
public:
	TrackAttributes();

	TrackAttributes(
		const TrackInformation& trackInformation, 
		const Poco::SharedPtr<VideoAttributes>& videoAttributes, 
		const Poco::SharedPtr<AudioAttributes>& audioAttributes, 
		const Poco::SharedPtr<MetadataAttributes>& metadataAttributes, 
		const Poco::SharedPtr<TrackAttributesExtension>& extension);

	virtual ~TrackAttributes();

	const Poco::SharedPtr<AudioAttributes>& getAudioAttributes() const;

	const Poco::SharedPtr<TrackAttributesExtension>& getExtension() const;

	const Poco::SharedPtr<MetadataAttributes>& getMetadataAttributes() const;

	const TrackInformation& getTrackInformation() const;

	const Poco::SharedPtr<VideoAttributes>& getVideoAttributes() const;

	void setAudioAttributes(const Poco::SharedPtr<AudioAttributes>& val);

	void setAudioAttributes(Poco::SharedPtr<AudioAttributes>&& val);

	void setExtension(const Poco::SharedPtr<TrackAttributesExtension>& val);

	void setExtension(Poco::SharedPtr<TrackAttributesExtension>&& val);

	void setMetadataAttributes(const Poco::SharedPtr<MetadataAttributes>& val);

	void setMetadataAttributes(Poco::SharedPtr<MetadataAttributes>&& val);

	void setTrackInformation(const TrackInformation& val);

	void setTrackInformation(TrackInformation&& val);

	void setVideoAttributes(const Poco::SharedPtr<VideoAttributes>& val);

	void setVideoAttributes(Poco::SharedPtr<VideoAttributes>&& val);

private:
	//@ name=TrackInformation
	//@ order=0
	TrackInformation _trackInformation;

	//@ mandatory=false
	//@ name=VideoAttributes
	//@ order=1
	Poco::SharedPtr<VideoAttributes> _videoAttributes;

	//@ mandatory=false
	//@ name=AudioAttributes
	//@ order=2
	Poco::SharedPtr<AudioAttributes> _audioAttributes;

	//@ mandatory=false
	//@ name=MetadataAttributes
	//@ order=3
	Poco::SharedPtr<MetadataAttributes> _metadataAttributes;

	//@ mandatory=false
	//@ name=Extension
	//@ order=4
	Poco::SharedPtr<TrackAttributesExtension> _extension;

};


inline const Poco::SharedPtr<AudioAttributes>& TrackAttributes::getAudioAttributes() const
{
	return _audioAttributes;
}


inline const Poco::SharedPtr<TrackAttributesExtension>& TrackAttributes::getExtension() const
{
	return _extension;
}


inline const Poco::SharedPtr<MetadataAttributes>& TrackAttributes::getMetadataAttributes() const
{
	return _metadataAttributes;
}


inline const TrackInformation& TrackAttributes::getTrackInformation() const
{
	return _trackInformation;
}


inline const Poco::SharedPtr<VideoAttributes>& TrackAttributes::getVideoAttributes() const
{
	return _videoAttributes;
}


inline void TrackAttributes::setAudioAttributes(const Poco::SharedPtr<AudioAttributes>& val)
{
	_audioAttributes = val;
}


inline void TrackAttributes::setAudioAttributes(Poco::SharedPtr<AudioAttributes>&& val)
{
	_audioAttributes = std::move(val);
}


inline void TrackAttributes::setExtension(const Poco::SharedPtr<TrackAttributesExtension>& val)
{
	_extension = val;
}


inline void TrackAttributes::setExtension(Poco::SharedPtr<TrackAttributesExtension>&& val)
{
	_extension = std::move(val);
}


inline void TrackAttributes::setMetadataAttributes(const Poco::SharedPtr<MetadataAttributes>& val)
{
	_metadataAttributes = val;
}


inline void TrackAttributes::setMetadataAttributes(Poco::SharedPtr<MetadataAttributes>&& val)
{
	_metadataAttributes = std::move(val);
}


inline void TrackAttributes::setTrackInformation(const TrackInformation& val)
{
	_trackInformation = val;
}


inline void TrackAttributes::setTrackInformation(TrackInformation&& val)
{
	_trackInformation = std::move(val);
}


inline void TrackAttributes::setVideoAttributes(const Poco::SharedPtr<VideoAttributes>& val)
{
	_videoAttributes = val;
}


inline void TrackAttributes::setVideoAttributes(Poco::SharedPtr<VideoAttributes>&& val)
{
	_videoAttributes = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/AudioAttributes.h"
#include "ONVIF/MetadataAttributes.h"
#include "ONVIF/TrackAttributesExtension.h"
#include "ONVIF/VideoAttributes.h"


#endif // ONVIF_TrackAttributes_INCLUDED
