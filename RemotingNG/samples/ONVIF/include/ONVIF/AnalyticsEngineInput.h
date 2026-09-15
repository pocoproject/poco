// 
// AnalyticsEngineInput.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_AnalyticsEngineInput_INCLUDED
#define ONVIF_AnalyticsEngineInput_INCLUDED


#include "ONVIF/ConfigurationEntity.h"
#include "ONVIF/MetadataInput.h"
#include "ONVIF/ONVIF.h"
#include "ONVIF/SourceIdentification.h"
#include "ONVIF/VideoEncoderConfiguration.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=AnalyticsEngineInput
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API AnalyticsEngineInput: public ConfigurationEntity
{
public:
	AnalyticsEngineInput();

	AnalyticsEngineInput(
		const std::string& token, 
		const std::string& name, 
		int useCount, 
		const SourceIdentification& sourceIdentification, 
		const VideoEncoderConfiguration& videoInput, 
		const MetadataInput& metadataInput);

	virtual ~AnalyticsEngineInput();

	const MetadataInput& getMetadataInput() const;

	const SourceIdentification& getSourceIdentification() const;

	const VideoEncoderConfiguration& getVideoInput() const;

	void setMetadataInput(const MetadataInput& val);

	void setMetadataInput(MetadataInput&& val);

	void setSourceIdentification(const SourceIdentification& val);

	void setSourceIdentification(SourceIdentification&& val);

	void setVideoInput(const VideoEncoderConfiguration& val);

	void setVideoInput(VideoEncoderConfiguration&& val);

private:
	//@ name=SourceIdentification
	//@ order=0
	SourceIdentification _sourceIdentification;

	//@ name=VideoInput
	//@ order=1
	VideoEncoderConfiguration _videoInput;

	//@ name=MetadataInput
	//@ order=2
	MetadataInput _metadataInput;

};


inline const MetadataInput& AnalyticsEngineInput::getMetadataInput() const
{
	return _metadataInput;
}


inline const SourceIdentification& AnalyticsEngineInput::getSourceIdentification() const
{
	return _sourceIdentification;
}


inline const VideoEncoderConfiguration& AnalyticsEngineInput::getVideoInput() const
{
	return _videoInput;
}


inline void AnalyticsEngineInput::setMetadataInput(const MetadataInput& val)
{
	_metadataInput = val;
}


inline void AnalyticsEngineInput::setMetadataInput(MetadataInput&& val)
{
	_metadataInput = std::move(val);
}


inline void AnalyticsEngineInput::setSourceIdentification(const SourceIdentification& val)
{
	_sourceIdentification = val;
}


inline void AnalyticsEngineInput::setSourceIdentification(SourceIdentification&& val)
{
	_sourceIdentification = std::move(val);
}


inline void AnalyticsEngineInput::setVideoInput(const VideoEncoderConfiguration& val)
{
	_videoInput = val;
}


inline void AnalyticsEngineInput::setVideoInput(VideoEncoderConfiguration&& val)
{
	_videoInput = std::move(val);
}


} // ONVIF


#endif // ONVIF_AnalyticsEngineInput_INCLUDED
