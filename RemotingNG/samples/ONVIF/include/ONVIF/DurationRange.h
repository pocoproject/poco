// 
// DurationRange.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DurationRange_INCLUDED
#define ONVIF_DurationRange_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=DurationRange
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API DurationRange
{
public:
	DurationRange();

	DurationRange(
		const std::string& min, 
		const std::string& max);

	virtual ~DurationRange();

	const std::string& getMax() const;

	const std::string& getMin() const;

	void setMax(const std::string& val);

	void setMax(std::string&& val);

	void setMin(const std::string& val);

	void setMin(std::string&& val);

private:
	//@ name=Min
	//@ order=0
	std::string _min;

	//@ name=Max
	//@ order=1
	std::string _max;

};


inline const std::string& DurationRange::getMax() const
{
	return _max;
}


inline const std::string& DurationRange::getMin() const
{
	return _min;
}


inline void DurationRange::setMax(const std::string& val)
{
	_max = val;
}


inline void DurationRange::setMax(std::string&& val)
{
	_max = std::move(val);
}


inline void DurationRange::setMin(const std::string& val)
{
	_min = val;
}


inline void DurationRange::setMin(std::string&& val)
{
	_min = std::move(val);
}


} // ONVIF


#endif // ONVIF_DurationRange_INCLUDED
