// 
// Capabilities.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_Capabilities_INCLUDED
#define ONVIF_Media_Capabilities_INCLUDED


#include "ONVIF/Media/ProfileCapabilities.h"
#include "ONVIF/Media/StreamingCapabilities.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name=Capabilities
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API Capabilities
{
public:
	Capabilities();

	Capabilities(
		Poco::Optional<bool> snapshotUri, 
		Poco::Optional<bool> rotation, 
		Poco::Optional<bool> videoSourceMode, 
		Poco::Optional<bool> oSD, 
		Poco::Optional<bool> temporaryOSDText, 
		Poco::Optional<bool> eXICompression, 
		const ProfileCapabilities& profileCapabilities, 
		const StreamingCapabilities& streamingCapabilities);

	virtual ~Capabilities();

	Poco::Optional<bool> getEXICompression() const;

	Poco::Optional<bool> getOSD() const;

	const ProfileCapabilities& getProfileCapabilities() const;

	Poco::Optional<bool> getRotation() const;

	Poco::Optional<bool> getSnapshotUri() const;

	const StreamingCapabilities& getStreamingCapabilities() const;

	Poco::Optional<bool> getTemporaryOSDText() const;

	Poco::Optional<bool> getVideoSourceMode() const;

	void setEXICompression(Poco::Optional<bool> val);

	void setOSD(Poco::Optional<bool> val);

	void setProfileCapabilities(const ProfileCapabilities& val);

	void setProfileCapabilities(ProfileCapabilities&& val);

	void setRotation(Poco::Optional<bool> val);

	void setSnapshotUri(Poco::Optional<bool> val);

	void setStreamingCapabilities(const StreamingCapabilities& val);

	void setStreamingCapabilities(StreamingCapabilities&& val);

	void setTemporaryOSDText(Poco::Optional<bool> val);

	void setVideoSourceMode(Poco::Optional<bool> val);

private:
	//@ mandatory=false
	//@ name=SnapshotUri
	//@ order=0
	//@ type=attr
	Poco::Optional<bool> _snapshotUri;

	//@ mandatory=false
	//@ name=Rotation
	//@ order=1
	//@ type=attr
	Poco::Optional<bool> _rotation;

	//@ mandatory=false
	//@ name=VideoSourceMode
	//@ order=2
	//@ type=attr
	Poco::Optional<bool> _videoSourceMode;

	//@ mandatory=false
	//@ name=OSD
	//@ order=3
	//@ type=attr
	Poco::Optional<bool> _oSD;

	//@ mandatory=false
	//@ name=TemporaryOSDText
	//@ order=4
	//@ type=attr
	Poco::Optional<bool> _temporaryOSDText;

	//@ mandatory=false
	//@ name=EXICompression
	//@ order=5
	//@ type=attr
	Poco::Optional<bool> _eXICompression;

	//@ name=ProfileCapabilities
	//@ order=6
	ProfileCapabilities _profileCapabilities;

	//@ name=StreamingCapabilities
	//@ order=7
	StreamingCapabilities _streamingCapabilities;

};


inline Poco::Optional<bool> Capabilities::getEXICompression() const
{
	return _eXICompression;
}


inline Poco::Optional<bool> Capabilities::getOSD() const
{
	return _oSD;
}


inline const ProfileCapabilities& Capabilities::getProfileCapabilities() const
{
	return _profileCapabilities;
}


inline Poco::Optional<bool> Capabilities::getRotation() const
{
	return _rotation;
}


inline Poco::Optional<bool> Capabilities::getSnapshotUri() const
{
	return _snapshotUri;
}


inline const StreamingCapabilities& Capabilities::getStreamingCapabilities() const
{
	return _streamingCapabilities;
}


inline Poco::Optional<bool> Capabilities::getTemporaryOSDText() const
{
	return _temporaryOSDText;
}


inline Poco::Optional<bool> Capabilities::getVideoSourceMode() const
{
	return _videoSourceMode;
}


inline void Capabilities::setEXICompression(Poco::Optional<bool> val)
{
	_eXICompression = val;
}


inline void Capabilities::setOSD(Poco::Optional<bool> val)
{
	_oSD = val;
}


inline void Capabilities::setProfileCapabilities(const ProfileCapabilities& val)
{
	_profileCapabilities = val;
}


inline void Capabilities::setProfileCapabilities(ProfileCapabilities&& val)
{
	_profileCapabilities = std::move(val);
}


inline void Capabilities::setRotation(Poco::Optional<bool> val)
{
	_rotation = val;
}


inline void Capabilities::setSnapshotUri(Poco::Optional<bool> val)
{
	_snapshotUri = val;
}


inline void Capabilities::setStreamingCapabilities(const StreamingCapabilities& val)
{
	_streamingCapabilities = val;
}


inline void Capabilities::setStreamingCapabilities(StreamingCapabilities&& val)
{
	_streamingCapabilities = std::move(val);
}


inline void Capabilities::setTemporaryOSDText(Poco::Optional<bool> val)
{
	_temporaryOSDText = val;
}


inline void Capabilities::setVideoSourceMode(Poco::Optional<bool> val)
{
	_videoSourceMode = val;
}


} } // ONVIF::Media


#endif // ONVIF_Media_Capabilities_INCLUDED
