// 
// G726DecOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_G726DecOptions_INCLUDED
#define ONVIF_G726DecOptions_INCLUDED


#include "ONVIF/IntItems.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=G726DecOptions
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API G726DecOptions
{
public:
	G726DecOptions();

	G726DecOptions(
		const IntItems& bitrate, 
		const IntItems& sampleRateRange);

	virtual ~G726DecOptions();

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


inline const IntItems& G726DecOptions::getBitrate() const
{
	return _bitrate;
}


inline const IntItems& G726DecOptions::getSampleRateRange() const
{
	return _sampleRateRange;
}


inline void G726DecOptions::setBitrate(const IntItems& val)
{
	_bitrate = val;
}


inline void G726DecOptions::setBitrate(IntItems&& val)
{
	_bitrate = std::move(val);
}


inline void G726DecOptions::setSampleRateRange(const IntItems& val)
{
	_sampleRateRange = val;
}


inline void G726DecOptions::setSampleRateRange(IntItems&& val)
{
	_sampleRateRange = std::move(val);
}


} // ONVIF


#endif // ONVIF_G726DecOptions_INCLUDED
