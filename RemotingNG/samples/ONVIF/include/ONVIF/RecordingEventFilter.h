// 
// RecordingEventFilter.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_RecordingEventFilter_INCLUDED
#define ONVIF_RecordingEventFilter_INCLUDED


#include "ONVIF/Filter.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=RecordingEventFilter
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API RecordingEventFilter
{
public:
	RecordingEventFilter();

	RecordingEventFilter(
		const std::vector<Filter>& filter, 
		const Poco::Optional<std::string>& before, 
		const Poco::Optional<std::string>& after);

	virtual ~RecordingEventFilter();

	const Poco::Optional<std::string>& getAfter() const;

	const Poco::Optional<std::string>& getBefore() const;

	const std::vector<Filter>& getFilter() const;

	std::vector<Filter>& getFilter();

	void setAfter(const Poco::Optional<std::string>& val);

	void setAfter(Poco::Optional<std::string>&& val);

	void setBefore(const Poco::Optional<std::string>& val);

	void setBefore(Poco::Optional<std::string>&& val);

	void setFilter(const std::vector<Filter>& val);

	void setFilter(std::vector<Filter>&& val);

private:
	//@ name=Filter
	//@ order=0
	std::vector<Filter> _filter;

	//@ mandatory=false
	//@ name=Before
	//@ order=1
	Poco::Optional<std::string> _before;

	//@ mandatory=false
	//@ name=After
	//@ order=2
	Poco::Optional<std::string> _after;

};


inline const Poco::Optional<std::string>& RecordingEventFilter::getAfter() const
{
	return _after;
}


inline const Poco::Optional<std::string>& RecordingEventFilter::getBefore() const
{
	return _before;
}


inline const std::vector<Filter>& RecordingEventFilter::getFilter() const
{
	return _filter;
}


inline std::vector<Filter>& RecordingEventFilter::getFilter()
{
	return _filter;
}


inline void RecordingEventFilter::setAfter(const Poco::Optional<std::string>& val)
{
	_after = val;
}


inline void RecordingEventFilter::setAfter(Poco::Optional<std::string>&& val)
{
	_after = std::move(val);
}


inline void RecordingEventFilter::setBefore(const Poco::Optional<std::string>& val)
{
	_before = val;
}


inline void RecordingEventFilter::setBefore(Poco::Optional<std::string>&& val)
{
	_before = std::move(val);
}


inline void RecordingEventFilter::setFilter(const std::vector<Filter>& val)
{
	_filter = val;
}


inline void RecordingEventFilter::setFilter(std::vector<Filter>&& val)
{
	_filter = std::move(val);
}


} // ONVIF


#endif // ONVIF_RecordingEventFilter_INCLUDED
