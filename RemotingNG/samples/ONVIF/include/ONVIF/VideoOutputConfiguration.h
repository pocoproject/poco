// 
// VideoOutputConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_VideoOutputConfiguration_INCLUDED
#define ONVIF_VideoOutputConfiguration_INCLUDED


#include "ONVIF/ConfigurationEntity.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=VideoOutputConfiguration
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API VideoOutputConfiguration: public ConfigurationEntity
{
public:
	VideoOutputConfiguration();

	VideoOutputConfiguration(
		const std::string& token, 
		const std::string& name, 
		int useCount, 
		const std::string& outputToken);

	virtual ~VideoOutputConfiguration();

	const std::string& getOutputToken() const;

	void setOutputToken(const std::string& val);

	void setOutputToken(std::string&& val);

private:
	//@ name=OutputToken
	//@ order=0
	std::string _outputToken;

};


inline const std::string& VideoOutputConfiguration::getOutputToken() const
{
	return _outputToken;
}


inline void VideoOutputConfiguration::setOutputToken(const std::string& val)
{
	_outputToken = val;
}


inline void VideoOutputConfiguration::setOutputToken(std::string&& val)
{
	_outputToken = std::move(val);
}


} // ONVIF


#endif // ONVIF_VideoOutputConfiguration_INCLUDED
