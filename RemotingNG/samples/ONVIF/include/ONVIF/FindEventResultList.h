// 
// FindEventResultList.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_FindEventResultList_INCLUDED
#define ONVIF_FindEventResultList_INCLUDED


#include "ONVIF/FindEventResult.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=FindEventResultList
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API FindEventResultList
{
public:
	FindEventResultList();

	FindEventResultList(
		const std::string& searchState, 
		const std::vector<FindEventResult>& result);

	virtual ~FindEventResultList();

	const std::vector<FindEventResult>& getResult() const;

	std::vector<FindEventResult>& getResult();

	const std::string& getSearchState() const;

	void setResult(const std::vector<FindEventResult>& val);

	void setResult(std::vector<FindEventResult>&& val);

	void setSearchState(const std::string& val);

	void setSearchState(std::string&& val);

private:
	//@ name=SearchState
	//@ order=0
	std::string _searchState;

	//@ mandatory=false
	//@ name=Result
	//@ order=1
	std::vector<FindEventResult> _result;

};


inline const std::vector<FindEventResult>& FindEventResultList::getResult() const
{
	return _result;
}


inline std::vector<FindEventResult>& FindEventResultList::getResult()
{
	return _result;
}


inline const std::string& FindEventResultList::getSearchState() const
{
	return _searchState;
}


inline void FindEventResultList::setResult(const std::vector<FindEventResult>& val)
{
	_result = val;
}


inline void FindEventResultList::setResult(std::vector<FindEventResult>&& val)
{
	_result = std::move(val);
}


inline void FindEventResultList::setSearchState(const std::string& val)
{
	_searchState = val;
}


inline void FindEventResultList::setSearchState(std::string&& val)
{
	_searchState = std::move(val);
}


} // ONVIF


#endif // ONVIF_FindEventResultList_INCLUDED
