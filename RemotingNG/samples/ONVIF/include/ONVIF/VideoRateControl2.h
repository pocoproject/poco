// 
// VideoRateControl2.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_VideoRateControl2_INCLUDED
#define ONVIF_VideoRateControl2_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=VideoRateControl2
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API VideoRateControl2
{
public:
	VideoRateControl2();

	VideoRateControl2(
		Poco::Optional<bool> constantBitRate, 
		float frameRateLimit, 
		int bitrateLimit);

	virtual ~VideoRateControl2();

	int getBitrateLimit() const;

	Poco::Optional<bool> getConstantBitRate() const;

	float getFrameRateLimit() const;

	void setBitrateLimit(int val);

	void setConstantBitRate(Poco::Optional<bool> val);

	void setFrameRateLimit(float val);

private:
	//@ mandatory=false
	//@ name=ConstantBitRate
	//@ order=0
	//@ type=attr
	Poco::Optional<bool> _constantBitRate;

	//@ name=FrameRateLimit
	//@ order=1
	float _frameRateLimit;

	//@ name=BitrateLimit
	//@ order=2
	int _bitrateLimit;

};


inline int VideoRateControl2::getBitrateLimit() const
{
	return _bitrateLimit;
}


inline Poco::Optional<bool> VideoRateControl2::getConstantBitRate() const
{
	return _constantBitRate;
}


inline float VideoRateControl2::getFrameRateLimit() const
{
	return _frameRateLimit;
}


inline void VideoRateControl2::setBitrateLimit(int val)
{
	_bitrateLimit = val;
}


inline void VideoRateControl2::setConstantBitRate(Poco::Optional<bool> val)
{
	_constantBitRate = val;
}


inline void VideoRateControl2::setFrameRateLimit(float val)
{
	_frameRateLimit = val;
}


} // ONVIF


#endif // ONVIF_VideoRateControl2_INCLUDED
