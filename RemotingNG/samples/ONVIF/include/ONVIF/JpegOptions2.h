// 
// JpegOptions2.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_JpegOptions2_INCLUDED
#define ONVIF_JpegOptions2_INCLUDED


#include "ONVIF/IntRange.h"
#include "ONVIF/JpegOptions.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=JpegOptions2
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API JpegOptions2: public JpegOptions
{
public:
	JpegOptions2();

	JpegOptions2(
		const std::vector<VideoResolution>& resolutionsAvailable, 
		const IntRange& frameRateRange, 
		const IntRange& encodingIntervalRange, 
		const IntRange& bitrateRange);

	virtual ~JpegOptions2();

	const IntRange& getBitrateRange() const;

	void setBitrateRange(const IntRange& val);

	void setBitrateRange(IntRange&& val);

private:
	//@ name=BitrateRange
	//@ order=0
	IntRange _bitrateRange;

};


inline const IntRange& JpegOptions2::getBitrateRange() const
{
	return _bitrateRange;
}


inline void JpegOptions2::setBitrateRange(const IntRange& val)
{
	_bitrateRange = val;
}


inline void JpegOptions2::setBitrateRange(IntRange&& val)
{
	_bitrateRange = std::move(val);
}


} // ONVIF


#endif // ONVIF_JpegOptions2_INCLUDED
