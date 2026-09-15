// 
// H264Options2.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_H264Options2_INCLUDED
#define ONVIF_H264Options2_INCLUDED


#include "ONVIF/H264Options.h"
#include "ONVIF/IntRange.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=H264Options2
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API H264Options2: public H264Options
{
public:
	H264Options2();

	H264Options2(
		const std::vector<VideoResolution>& resolutionsAvailable, 
		const IntRange& govLengthRange, 
		const IntRange& frameRateRange, 
		const IntRange& encodingIntervalRange, 
		const std::vector<std::string>& h264ProfilesSupported, 
		const IntRange& bitrateRange);

	virtual ~H264Options2();

	const IntRange& getBitrateRange() const;

	void setBitrateRange(const IntRange& val);

	void setBitrateRange(IntRange&& val);

private:
	//@ name=BitrateRange
	//@ order=0
	IntRange _bitrateRange;

};


inline const IntRange& H264Options2::getBitrateRange() const
{
	return _bitrateRange;
}


inline void H264Options2::setBitrateRange(const IntRange& val)
{
	_bitrateRange = val;
}


inline void H264Options2::setBitrateRange(IntRange&& val)
{
	_bitrateRange = std::move(val);
}


} // ONVIF


#endif // ONVIF_H264Options2_INCLUDED
