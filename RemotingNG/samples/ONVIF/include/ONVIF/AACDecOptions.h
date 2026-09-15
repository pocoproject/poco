// 
// AACDecOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_AACDecOptions_INCLUDED
#define ONVIF_AACDecOptions_INCLUDED


#include "ONVIF/IntItems.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=AACDecOptions
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API AACDecOptions
{
public:
	AACDecOptions();

	AACDecOptions(
		const IntItems& bitrate, 
		const IntItems& sampleRateRange);

	virtual ~AACDecOptions();

	const IntItems& getBitrate() const;

	const IntItems& getSampleRateRange() const;

	void setBitrate(const IntItems& val);

	void setBitrate(IntItems&& val);

	void setSampleRateRange(const IntItems& val);

	void setSampleRateRange(IntItems&& val);

private:
	//@ name=Bitrate
	//@ order=0
	IntItems _bitrate;

	//@ name=SampleRateRange
	//@ order=1
	IntItems _sampleRateRange;

};


inline const IntItems& AACDecOptions::getBitrate() const
{
	return _bitrate;
}


inline const IntItems& AACDecOptions::getSampleRateRange() const
{
	return _sampleRateRange;
}


inline void AACDecOptions::setBitrate(const IntItems& val)
{
	_bitrate = val;
}


inline void AACDecOptions::setBitrate(IntItems&& val)
{
	_bitrate = std::move(val);
}


inline void AACDecOptions::setSampleRateRange(const IntItems& val)
{
	_sampleRateRange = val;
}


inline void AACDecOptions::setSampleRateRange(IntItems&& val)
{
	_sampleRateRange = std::move(val);
}


} // ONVIF


#endif // ONVIF_AACDecOptions_INCLUDED
