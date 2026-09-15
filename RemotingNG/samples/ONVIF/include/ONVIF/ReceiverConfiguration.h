// 
// ReceiverConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_ReceiverConfiguration_INCLUDED
#define ONVIF_ReceiverConfiguration_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/StreamSetup.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/URI.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=ReceiverConfiguration
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API ReceiverConfiguration
{
public:
	ReceiverConfiguration();

	ReceiverConfiguration(
		const std::string& mode, 
		const Poco::URI& mediaUri, 
		const StreamSetup& streamSetup);

	virtual ~ReceiverConfiguration();

	const Poco::URI& getMediaUri() const;

	const std::string& getMode() const;

	const StreamSetup& getStreamSetup() const;

	void setMediaUri(const Poco::URI& val);

	void setMediaUri(Poco::URI&& val);

	void setMode(const std::string& val);

	void setMode(std::string&& val);

	void setStreamSetup(const StreamSetup& val);

	void setStreamSetup(StreamSetup&& val);

private:
	//@ name=Mode
	//@ order=0
	std::string _mode;

	//@ name=MediaUri
	//@ order=1
	Poco::URI _mediaUri;

	//@ name=StreamSetup
	//@ order=2
	StreamSetup _streamSetup;

};


inline const Poco::URI& ReceiverConfiguration::getMediaUri() const
{
	return _mediaUri;
}


inline const std::string& ReceiverConfiguration::getMode() const
{
	return _mode;
}


inline const StreamSetup& ReceiverConfiguration::getStreamSetup() const
{
	return _streamSetup;
}


inline void ReceiverConfiguration::setMediaUri(const Poco::URI& val)
{
	_mediaUri = val;
}


inline void ReceiverConfiguration::setMediaUri(Poco::URI&& val)
{
	_mediaUri = std::move(val);
}


inline void ReceiverConfiguration::setMode(const std::string& val)
{
	_mode = val;
}


inline void ReceiverConfiguration::setMode(std::string&& val)
{
	_mode = std::move(val);
}


inline void ReceiverConfiguration::setStreamSetup(const StreamSetup& val)
{
	_streamSetup = val;
}


inline void ReceiverConfiguration::setStreamSetup(StreamSetup&& val)
{
	_streamSetup = std::move(val);
}


} // ONVIF


#endif // ONVIF_ReceiverConfiguration_INCLUDED
