// 
// Mpeg4Options2.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Mpeg4Options2_INCLUDED
#define ONVIF_Mpeg4Options2_INCLUDED


#include "ONVIF/IntRange.h"
#include "ONVIF/Mpeg4Options.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=Mpeg4Options2
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API Mpeg4Options2: public Mpeg4Options
{
public:
	Mpeg4Options2();

	Mpeg4Options2(
		const std::vector<VideoResolution>& resolutionsAvailable, 
		const IntRange& govLengthRange, 
		const IntRange& frameRateRange, 
		const IntRange& encodingIntervalRange, 
		const std::vector<std::string>& mpeg4ProfilesSupported, 
		const IntRange& bitrateRange);

	virtual ~Mpeg4Options2();

	const IntRange& getBitrateRange() const;

	void setBitrateRange(const IntRange& val);

	void setBitrateRange(IntRange&& val);

private:
	//@ name=BitrateRange
	//@ order=0
	IntRange _bitrateRange;

};


inline const IntRange& Mpeg4Options2::getBitrateRange() const
{
	return _bitrateRange;
}


inline void Mpeg4Options2::setBitrateRange(const IntRange& val)
{
	_bitrateRange = val;
}


inline void Mpeg4Options2::setBitrateRange(IntRange&& val)
{
	_bitrateRange = std::move(val);
}


} // ONVIF


#endif // ONVIF_Mpeg4Options2_INCLUDED
