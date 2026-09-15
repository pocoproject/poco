// 
// FindPTZPositionResultList.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_FindPTZPositionResultList_INCLUDED
#define ONVIF_FindPTZPositionResultList_INCLUDED


#include "ONVIF/FindPTZPositionResult.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=FindPTZPositionResultList
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API FindPTZPositionResultList
{
public:
	FindPTZPositionResultList();

	FindPTZPositionResultList(
		const std::string& searchState, 
		const std::vector<FindPTZPositionResult>& result);

	virtual ~FindPTZPositionResultList();

	const std::vector<FindPTZPositionResult>& getResult() const;

	std::vector<FindPTZPositionResult>& getResult();

	const std::string& getSearchState() const;

	void setResult(const std::vector<FindPTZPositionResult>& val);

	void setResult(std::vector<FindPTZPositionResult>&& val);

	void setSearchState(const std::string& val);

	void setSearchState(std::string&& val);

private:
	//@ name=SearchState
	//@ order=0
	std::string _searchState;

	//@ mandatory=false
	//@ name=Result
	//@ order=1
	std::vector<FindPTZPositionResult> _result;

};


inline const std::vector<FindPTZPositionResult>& FindPTZPositionResultList::getResult() const
{
	return _result;
}


inline std::vector<FindPTZPositionResult>& FindPTZPositionResultList::getResult()
{
	return _result;
}


inline const std::string& FindPTZPositionResultList::getSearchState() const
{
	return _searchState;
}


inline void FindPTZPositionResultList::setResult(const std::vector<FindPTZPositionResult>& val)
{
	_result = val;
}


inline void FindPTZPositionResultList::setResult(std::vector<FindPTZPositionResult>&& val)
{
	_result = std::move(val);
}


inline void FindPTZPositionResultList::setSearchState(const std::string& val)
{
	_searchState = val;
}


inline void FindPTZPositionResultList::setSearchState(std::string&& val)
{
	_searchState = std::move(val);
}


} // ONVIF


#endif // ONVIF_FindPTZPositionResultList_INCLUDED
