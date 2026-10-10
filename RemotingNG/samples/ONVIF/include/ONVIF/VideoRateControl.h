// 
// VideoRateControl.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_VideoRateControl_INCLUDED
#define ONVIF_VideoRateControl_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=VideoRateControl
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API VideoRateControl
{
public:
	VideoRateControl();

	VideoRateControl(
		int frameRateLimit, 
		int encodingInterval, 
		int bitrateLimit);

	virtual ~VideoRateControl();

	int getBitrateLimit() const;

	int getEncodingInterval() const;

	int getFrameRateLimit() const;

	void setBitrateLimit(int val);

	void setEncodingInterval(int val);

	void setFrameRateLimit(int val);

private:
	//@ name=FrameRateLimit
	//@ order=0
	int _frameRateLimit;

	//@ name=EncodingInterval
	//@ order=1
	int _encodingInterval;

	//@ name=BitrateLimit
	//@ order=2
	int _bitrateLimit;

};


inline int VideoRateControl::getBitrateLimit() const
{
	return _bitrateLimit;
}


inline int VideoRateControl::getEncodingInterval() const
{
	return _encodingInterval;
}


inline int VideoRateControl::getFrameRateLimit() const
{
	return _frameRateLimit;
}


inline void VideoRateControl::setBitrateLimit(int val)
{
	_bitrateLimit = val;
}


inline void VideoRateControl::setEncodingInterval(int val)
{
	_encodingInterval = val;
}


inline void VideoRateControl::setFrameRateLimit(int val)
{
	_frameRateLimit = val;
}


} // ONVIF


#endif // ONVIF_VideoRateControl_INCLUDED
