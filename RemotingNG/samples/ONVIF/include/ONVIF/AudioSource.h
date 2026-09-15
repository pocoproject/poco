// 
// AudioSource.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_AudioSource_INCLUDED
#define ONVIF_AudioSource_INCLUDED


#include "ONVIF/DeviceEntity.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=AudioSource
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API AudioSource: public DeviceEntity
{
public:
	AudioSource();

	AudioSource(
		const std::string& token, 
		int channels);

	virtual ~AudioSource();

	int getChannels() const;

	void setChannels(int val);

private:
	//@ name=Channels
	//@ order=0
	int _channels;

};


inline int AudioSource::getChannels() const
{
	return _channels;
}


inline void AudioSource::setChannels(int val)
{
	_channels = val;
}


} // ONVIF


#endif // ONVIF_AudioSource_INCLUDED
