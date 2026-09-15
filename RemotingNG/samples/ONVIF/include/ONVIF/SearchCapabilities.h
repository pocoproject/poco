// 
// SearchCapabilities.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_SearchCapabilities_INCLUDED
#define ONVIF_SearchCapabilities_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/URI.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=SearchCapabilities
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API SearchCapabilities
{
public:
	SearchCapabilities();

	SearchCapabilities(
		const Poco::URI& xAddr, 
		bool metadataSearch);

	virtual ~SearchCapabilities();

	bool getMetadataSearch() const;

	const Poco::URI& getXAddr() const;

	void setMetadataSearch(bool val);

	void setXAddr(const Poco::URI& val);

	void setXAddr(Poco::URI&& val);

private:
	//@ name=XAddr
	//@ order=0
	Poco::URI _xAddr;

	//@ name=MetadataSearch
	//@ order=1
	bool _metadataSearch;

};


inline bool SearchCapabilities::getMetadataSearch() const
{
	return _metadataSearch;
}


inline const Poco::URI& SearchCapabilities::getXAddr() const
{
	return _xAddr;
}


inline void SearchCapabilities::setMetadataSearch(bool val)
{
	_metadataSearch = val;
}


inline void SearchCapabilities::setXAddr(const Poco::URI& val)
{
	_xAddr = val;
}


inline void SearchCapabilities::setXAddr(Poco::URI&& val)
{
	_xAddr = std::move(val);
}


} // ONVIF


#endif // ONVIF_SearchCapabilities_INCLUDED
