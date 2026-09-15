// 
// VideoSourceConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_VideoSourceConfiguration_INCLUDED
#define ONVIF_VideoSourceConfiguration_INCLUDED


#include "ONVIF/ConfigurationEntity.h"
#include "ONVIF/IntRectangle.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class VideoSourceConfigurationExtension;
} 


namespace ONVIF {


//@ name=VideoSourceConfiguration
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API VideoSourceConfiguration: public ConfigurationEntity
{
public:
	VideoSourceConfiguration();

	VideoSourceConfiguration(
		const std::string& token, 
		const std::string& name, 
		int useCount, 
		const Poco::Optional<std::string>& viewMode, 
		const std::string& sourceToken, 
		const IntRectangle& bounds, 
		const Poco::SharedPtr<VideoSourceConfigurationExtension>& extension);

	virtual ~VideoSourceConfiguration();

	const IntRectangle& getBounds() const;

	const Poco::SharedPtr<VideoSourceConfigurationExtension>& getExtension() const;

	const std::string& getSourceToken() const;

	const Poco::Optional<std::string>& getViewMode() const;

	void setBounds(const IntRectangle& val);

	void setBounds(IntRectangle&& val);

	void setExtension(const Poco::SharedPtr<VideoSourceConfigurationExtension>& val);

	void setExtension(Poco::SharedPtr<VideoSourceConfigurationExtension>&& val);

	void setSourceToken(const std::string& val);

	void setSourceToken(std::string&& val);

	void setViewMode(const Poco::Optional<std::string>& val);

	void setViewMode(Poco::Optional<std::string>&& val);

private:
	//@ mandatory=false
	//@ name=ViewMode
	//@ order=0
	//@ type=attr
	Poco::Optional<std::string> _viewMode;

	//@ name=SourceToken
	//@ order=1
	std::string _sourceToken;

	//@ name=Bounds
	//@ order=2
	IntRectangle _bounds;

	//@ mandatory=false
	//@ name=Extension
	//@ order=3
	Poco::SharedPtr<VideoSourceConfigurationExtension> _extension;

};


inline const IntRectangle& VideoSourceConfiguration::getBounds() const
{
	return _bounds;
}


inline const Poco::SharedPtr<VideoSourceConfigurationExtension>& VideoSourceConfiguration::getExtension() const
{
	return _extension;
}


inline const std::string& VideoSourceConfiguration::getSourceToken() const
{
	return _sourceToken;
}


inline const Poco::Optional<std::string>& VideoSourceConfiguration::getViewMode() const
{
	return _viewMode;
}


inline void VideoSourceConfiguration::setBounds(const IntRectangle& val)
{
	_bounds = val;
}


inline void VideoSourceConfiguration::setBounds(IntRectangle&& val)
{
	_bounds = std::move(val);
}


inline void VideoSourceConfiguration::setExtension(const Poco::SharedPtr<VideoSourceConfigurationExtension>& val)
{
	_extension = val;
}


inline void VideoSourceConfiguration::setExtension(Poco::SharedPtr<VideoSourceConfigurationExtension>&& val)
{
	_extension = std::move(val);
}


inline void VideoSourceConfiguration::setSourceToken(const std::string& val)
{
	_sourceToken = val;
}


inline void VideoSourceConfiguration::setSourceToken(std::string&& val)
{
	_sourceToken = std::move(val);
}


inline void VideoSourceConfiguration::setViewMode(const Poco::Optional<std::string>& val)
{
	_viewMode = val;
}


inline void VideoSourceConfiguration::setViewMode(Poco::Optional<std::string>&& val)
{
	_viewMode = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/VideoSourceConfigurationExtension.h"


#endif // ONVIF_VideoSourceConfiguration_INCLUDED
