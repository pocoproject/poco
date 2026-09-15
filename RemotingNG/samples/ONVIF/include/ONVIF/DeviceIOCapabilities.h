// 
// DeviceIOCapabilities.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DeviceIOCapabilities_INCLUDED
#define ONVIF_DeviceIOCapabilities_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/URI.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=DeviceIOCapabilities
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API DeviceIOCapabilities
{
public:
	DeviceIOCapabilities();

	DeviceIOCapabilities(
		const Poco::URI& xAddr, 
		int videoSources, 
		int videoOutputs, 
		int audioSources, 
		int audioOutputs, 
		int relayOutputs);

	virtual ~DeviceIOCapabilities();

	int getAudioOutputs() const;

	int getAudioSources() const;

	int getRelayOutputs() const;

	int getVideoOutputs() const;

	int getVideoSources() const;

	const Poco::URI& getXAddr() const;

	void setAudioOutputs(int val);

	void setAudioSources(int val);

	void setRelayOutputs(int val);

	void setVideoOutputs(int val);

	void setVideoSources(int val);

	void setXAddr(const Poco::URI& val);

	void setXAddr(Poco::URI&& val);

private:
	//@ name=XAddr
	//@ order=0
	Poco::URI _xAddr;

	//@ name=VideoSources
	//@ order=1
	int _videoSources;

	//@ name=VideoOutputs
	//@ order=2
	int _videoOutputs;

	//@ name=AudioSources
	//@ order=3
	int _audioSources;

	//@ name=AudioOutputs
	//@ order=4
	int _audioOutputs;

	//@ name=RelayOutputs
	//@ order=5
	int _relayOutputs;

};


inline int DeviceIOCapabilities::getAudioOutputs() const
{
	return _audioOutputs;
}


inline int DeviceIOCapabilities::getAudioSources() const
{
	return _audioSources;
}


inline int DeviceIOCapabilities::getRelayOutputs() const
{
	return _relayOutputs;
}


inline int DeviceIOCapabilities::getVideoOutputs() const
{
	return _videoOutputs;
}


inline int DeviceIOCapabilities::getVideoSources() const
{
	return _videoSources;
}


inline const Poco::URI& DeviceIOCapabilities::getXAddr() const
{
	return _xAddr;
}


inline void DeviceIOCapabilities::setAudioOutputs(int val)
{
	_audioOutputs = val;
}


inline void DeviceIOCapabilities::setAudioSources(int val)
{
	_audioSources = val;
}


inline void DeviceIOCapabilities::setRelayOutputs(int val)
{
	_relayOutputs = val;
}


inline void DeviceIOCapabilities::setVideoOutputs(int val)
{
	_videoOutputs = val;
}


inline void DeviceIOCapabilities::setVideoSources(int val)
{
	_videoSources = val;
}


inline void DeviceIOCapabilities::setXAddr(const Poco::URI& val)
{
	_xAddr = val;
}


inline void DeviceIOCapabilities::setXAddr(Poco::URI&& val)
{
	_xAddr = std::move(val);
}


} // ONVIF


#endif // ONVIF_DeviceIOCapabilities_INCLUDED
