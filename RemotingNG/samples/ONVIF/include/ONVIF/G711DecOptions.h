// 
// G711DecOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_G711DecOptions_INCLUDED
#define ONVIF_G711DecOptions_INCLUDED


#include "ONVIF/IntItems.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=G711DecOptions
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API G711DecOptions
{
public:
	G711DecOptions();

	G711DecOptions(
		const IntItems& bitrate, 
		const IntItems& sampleRateRange);

	virtual ~G711DecOptions();

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


inline const IntItems& G711DecOptions::getBitrate() const
{
	return _bitrate;
}


inline const IntItems& G711DecOptions::getSampleRateRange() const
{
	return _sampleRateRange;
}


inline void G711DecOptions::setBitrate(const IntItems& val)
{
	_bitrate = val;
}


inline void G711DecOptions::setBitrate(IntItems&& val)
{
	_bitrate = std::move(val);
}


inline void G711DecOptions::setSampleRateRange(const IntItems& val)
{
	_sampleRateRange = val;
}


inline void G711DecOptions::setSampleRateRange(IntItems&& val)
{
	_sampleRateRange = std::move(val);
}


} // ONVIF


#endif // ONVIF_G711DecOptions_INCLUDED
