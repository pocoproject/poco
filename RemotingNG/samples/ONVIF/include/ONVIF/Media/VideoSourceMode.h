// 
// VideoSourceMode.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_VideoSourceMode_INCLUDED
#define ONVIF_Media_VideoSourceMode_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/VideoResolution.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {
class VideoSourceModeExtension;
} } 


namespace ONVIF {
namespace Media {


//@ name=VideoSourceMode
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API VideoSourceMode
{
public:
	VideoSourceMode();

	VideoSourceMode(
		const std::string& token, 
		Poco::Optional<bool> enabled, 
		float maxFramerate, 
		const ONVIF::VideoResolution& maxResolution, 
		const std::vector<std::string>& encodings, 
		bool reboot, 
		const Poco::Optional<std::string>& description, 
		const Poco::SharedPtr<VideoSourceModeExtension>& extension);

	virtual ~VideoSourceMode();

	const Poco::Optional<std::string>& getDescription() const;

	Poco::Optional<bool> getEnabled() const;

	const std::vector<std::string>& getEncodings() const;

	const Poco::SharedPtr<VideoSourceModeExtension>& getExtension() const;

	float getMaxFramerate() const;

	const ONVIF::VideoResolution& getMaxResolution() const;

	bool getReboot() const;

	const std::string& getToken() const;

	void setDescription(const Poco::Optional<std::string>& val);

	void setDescription(Poco::Optional<std::string>&& val);

	void setEnabled(Poco::Optional<bool> val);

	void setEncodings(const std::vector<std::string>& val);

	void setEncodings(std::vector<std::string>&& val);

	void setExtension(const Poco::SharedPtr<VideoSourceModeExtension>& val);

	void setExtension(Poco::SharedPtr<VideoSourceModeExtension>&& val);

	void setMaxFramerate(float val);

	void setMaxResolution(const ONVIF::VideoResolution& val);

	void setMaxResolution(ONVIF::VideoResolution&& val);

	void setReboot(bool val);

	void setToken(const std::string& val);

	void setToken(std::string&& val);

private:
	//@ name=token
	//@ order=0
	//@ type=attr
	std::string _token;

	//@ mandatory=false
	//@ name=Enabled
	//@ order=1
	//@ type=attr
	Poco::Optional<bool> _enabled;

	//@ name=MaxFramerate
	//@ order=2
	float _maxFramerate;

	//@ name=MaxResolution
	//@ order=3
	ONVIF::VideoResolution _maxResolution;

	//@ name=Encodings
	//@ order=4
	std::vector<std::string> _encodings;

	//@ name=Reboot
	//@ order=5
	bool _reboot;

	//@ mandatory=false
	//@ name=Description
	//@ order=6
	Poco::Optional<std::string> _description;

	//@ mandatory=false
	//@ name=Extension
	//@ order=7
	Poco::SharedPtr<VideoSourceModeExtension> _extension;

};


inline const Poco::Optional<std::string>& VideoSourceMode::getDescription() const
{
	return _description;
}


inline Poco::Optional<bool> VideoSourceMode::getEnabled() const
{
	return _enabled;
}


inline const std::vector<std::string>& VideoSourceMode::getEncodings() const
{
	return _encodings;
}


inline const Poco::SharedPtr<VideoSourceModeExtension>& VideoSourceMode::getExtension() const
{
	return _extension;
}


inline float VideoSourceMode::getMaxFramerate() const
{
	return _maxFramerate;
}


inline const ONVIF::VideoResolution& VideoSourceMode::getMaxResolution() const
{
	return _maxResolution;
}


inline bool VideoSourceMode::getReboot() const
{
	return _reboot;
}


inline const std::string& VideoSourceMode::getToken() const
{
	return _token;
}


inline void VideoSourceMode::setDescription(const Poco::Optional<std::string>& val)
{
	_description = val;
}


inline void VideoSourceMode::setDescription(Poco::Optional<std::string>&& val)
{
	_description = std::move(val);
}


inline void VideoSourceMode::setEnabled(Poco::Optional<bool> val)
{
	_enabled = val;
}


inline void VideoSourceMode::setEncodings(const std::vector<std::string>& val)
{
	_encodings = val;
}


inline void VideoSourceMode::setEncodings(std::vector<std::string>&& val)
{
	_encodings = std::move(val);
}


inline void VideoSourceMode::setExtension(const Poco::SharedPtr<VideoSourceModeExtension>& val)
{
	_extension = val;
}


inline void VideoSourceMode::setExtension(Poco::SharedPtr<VideoSourceModeExtension>&& val)
{
	_extension = std::move(val);
}


inline void VideoSourceMode::setMaxFramerate(float val)
{
	_maxFramerate = val;
}


inline void VideoSourceMode::setMaxResolution(const ONVIF::VideoResolution& val)
{
	_maxResolution = val;
}


inline void VideoSourceMode::setMaxResolution(ONVIF::VideoResolution&& val)
{
	_maxResolution = std::move(val);
}


inline void VideoSourceMode::setReboot(bool val)
{
	_reboot = val;
}


inline void VideoSourceMode::setToken(const std::string& val)
{
	_token = val;
}


inline void VideoSourceMode::setToken(std::string&& val)
{
	_token = std::move(val);
}


} } // ONVIF::Media


// The following headers are required for template instantiation.
#include "ONVIF/Media/VideoSourceModeExtension.h"


#endif // ONVIF_Media_VideoSourceMode_INCLUDED
