// 
// MetadataFilter.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_MetadataFilter_INCLUDED
#define ONVIF_MetadataFilter_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=MetadataFilter
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API MetadataFilter
{
public:
	MetadataFilter();

	MetadataFilter(const std::string& metadataStreamFilter);

	virtual ~MetadataFilter();

	const std::string& getMetadataStreamFilter() const;

	void setMetadataStreamFilter(const std::string& val);

	void setMetadataStreamFilter(std::string&& val);

private:
	//@ name=MetadataStreamFilter
	//@ order=0
	std::string _metadataStreamFilter;

};


inline const std::string& MetadataFilter::getMetadataStreamFilter() const
{
	return _metadataStreamFilter;
}


inline void MetadataFilter::setMetadataStreamFilter(const std::string& val)
{
	_metadataStreamFilter = val;
}


inline void MetadataFilter::setMetadataStreamFilter(std::string&& val)
{
	_metadataStreamFilter = std::move(val);
}


} // ONVIF


#endif // ONVIF_MetadataFilter_INCLUDED
