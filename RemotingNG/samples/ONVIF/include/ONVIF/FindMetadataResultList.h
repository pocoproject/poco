// 
// FindMetadataResultList.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_FindMetadataResultList_INCLUDED
#define ONVIF_FindMetadataResultList_INCLUDED


#include "ONVIF/FindMetadataResult.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=FindMetadataResultList
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API FindMetadataResultList
{
public:
	FindMetadataResultList();

	FindMetadataResultList(
		const std::string& searchState, 
		const std::vector<FindMetadataResult>& result);

	virtual ~FindMetadataResultList();

	const std::vector<FindMetadataResult>& getResult() const;

	std::vector<FindMetadataResult>& getResult();

	const std::string& getSearchState() const;

	void setResult(const std::vector<FindMetadataResult>& val);

	void setResult(std::vector<FindMetadataResult>&& val);

	void setSearchState(const std::string& val);

	void setSearchState(std::string&& val);

private:
	//@ name=SearchState
	//@ order=0
	std::string _searchState;

	//@ mandatory=false
	//@ name=Result
	//@ order=1
	std::vector<FindMetadataResult> _result;

};


inline const std::vector<FindMetadataResult>& FindMetadataResultList::getResult() const
{
	return _result;
}


inline std::vector<FindMetadataResult>& FindMetadataResultList::getResult()
{
	return _result;
}


inline const std::string& FindMetadataResultList::getSearchState() const
{
	return _searchState;
}


inline void FindMetadataResultList::setResult(const std::vector<FindMetadataResult>& val)
{
	_result = val;
}


inline void FindMetadataResultList::setResult(std::vector<FindMetadataResult>&& val)
{
	_result = std::move(val);
}


inline void FindMetadataResultList::setSearchState(const std::string& val)
{
	_searchState = val;
}


inline void FindMetadataResultList::setSearchState(std::string&& val)
{
	_searchState = std::move(val);
}


} // ONVIF


#endif // ONVIF_FindMetadataResultList_INCLUDED
