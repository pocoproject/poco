// 
// Capabilities.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DeviceIO_Capabilities_INCLUDED
#define ONVIF_DeviceIO_Capabilities_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace DeviceIO {


//@ name=Capabilities
//@ namespace="http://www.onvif.org/ver10/deviceIO/wsdl"
//@ serialize
class ONVIF_API Capabilities
{
public:
	Capabilities();

	Capabilities(
		Poco::Optional<int> videoSources, 
		Poco::Optional<int> videoOutputs, 
		Poco::Optional<int> audioSources, 
		Poco::Optional<int> audioOutputs, 
		Poco::Optional<int> relayOutputs, 
		Poco::Optional<int> serialPorts, 
		Poco::Optional<int> digitalInputs, 
		Poco::Optional<bool> digitalInputOptions);

	virtual ~Capabilities();

	Poco::Optional<int> getAudioOutputs() const;

	Poco::Optional<int> getAudioSources() const;

	Poco::Optional<bool> getDigitalInputOptions() const;

	Poco::Optional<int> getDigitalInputs() const;

	Poco::Optional<int> getRelayOutputs() const;

	Poco::Optional<int> getSerialPorts() const;

	Poco::Optional<int> getVideoOutputs() const;

	Poco::Optional<int> getVideoSources() const;

	void setAudioOutputs(Poco::Optional<int> val);

	void setAudioSources(Poco::Optional<int> val);

	void setDigitalInputOptions(Poco::Optional<bool> val);

	void setDigitalInputs(Poco::Optional<int> val);

	void setRelayOutputs(Poco::Optional<int> val);

	void setSerialPorts(Poco::Optional<int> val);

	void setVideoOutputs(Poco::Optional<int> val);

	void setVideoSources(Poco::Optional<int> val);

private:
	//@ mandatory=false
	//@ name=VideoSources
	//@ order=0
	//@ type=attr
	Poco::Optional<int> _videoSources;

	//@ mandatory=false
	//@ name=VideoOutputs
	//@ order=1
	//@ type=attr
	Poco::Optional<int> _videoOutputs;

	//@ mandatory=false
	//@ name=AudioSources
	//@ order=2
	//@ type=attr
	Poco::Optional<int> _audioSources;

	//@ mandatory=false
	//@ name=AudioOutputs
	//@ order=3
	//@ type=attr
	Poco::Optional<int> _audioOutputs;

	//@ mandatory=false
	//@ name=RelayOutputs
	//@ order=4
	//@ type=attr
	Poco::Optional<int> _relayOutputs;

	//@ mandatory=false
	//@ name=SerialPorts
	//@ order=5
	//@ type=attr
	Poco::Optional<int> _serialPorts;

	//@ mandatory=false
	//@ name=DigitalInputs
	//@ order=6
	//@ type=attr
	Poco::Optional<int> _digitalInputs;

	//@ mandatory=false
	//@ name=DigitalInputOptions
	//@ order=7
	//@ type=attr
	Poco::Optional<bool> _digitalInputOptions;

};


inline Poco::Optional<int> Capabilities::getAudioOutputs() const
{
	return _audioOutputs;
}


inline Poco::Optional<int> Capabilities::getAudioSources() const
{
	return _audioSources;
}


inline Poco::Optional<bool> Capabilities::getDigitalInputOptions() const
{
	return _digitalInputOptions;
}


inline Poco::Optional<int> Capabilities::getDigitalInputs() const
{
	return _digitalInputs;
}


inline Poco::Optional<int> Capabilities::getRelayOutputs() const
{
	return _relayOutputs;
}


inline Poco::Optional<int> Capabilities::getSerialPorts() const
{
	return _serialPorts;
}


inline Poco::Optional<int> Capabilities::getVideoOutputs() const
{
	return _videoOutputs;
}


inline Poco::Optional<int> Capabilities::getVideoSources() const
{
	return _videoSources;
}


inline void Capabilities::setAudioOutputs(Poco::Optional<int> val)
{
	_audioOutputs = val;
}


inline void Capabilities::setAudioSources(Poco::Optional<int> val)
{
	_audioSources = val;
}


inline void Capabilities::setDigitalInputOptions(Poco::Optional<bool> val)
{
	_digitalInputOptions = val;
}


inline void Capabilities::setDigitalInputs(Poco::Optional<int> val)
{
	_digitalInputs = val;
}


inline void Capabilities::setRelayOutputs(Poco::Optional<int> val)
{
	_relayOutputs = val;
}


inline void Capabilities::setSerialPorts(Poco::Optional<int> val)
{
	_serialPorts = val;
}


inline void Capabilities::setVideoOutputs(Poco::Optional<int> val)
{
	_videoOutputs = val;
}


inline void Capabilities::setVideoSources(Poco::Optional<int> val)
{
	_videoSources = val;
}


} } // ONVIF::DeviceIO


#endif // ONVIF_DeviceIO_Capabilities_INCLUDED
