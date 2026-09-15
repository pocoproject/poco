// 
// FindRecordingResultList.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_FindRecordingResultList_INCLUDED
#define ONVIF_FindRecordingResultList_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/RecordingInformation.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=FindRecordingResultList
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API FindRecordingResultList
{
public:
	FindRecordingResultList();

	FindRecordingResultList(
		const std::string& searchState, 
		const std::vector<RecordingInformation>& recordingInformation);

	virtual ~FindRecordingResultList();

	const std::vector<RecordingInformation>& getRecordingInformation() const;

	std::vector<RecordingInformation>& getRecordingInformation();

	const std::string& getSearchState() const;

	void setRecordingInformation(const std::vector<RecordingInformation>& val);

	void setRecordingInformation(std::vector<RecordingInformation>&& val);

	void setSearchState(const std::string& val);

	void setSearchState(std::string&& val);

private:
	//@ name=SearchState
	//@ order=0
	std::string _searchState;

	//@ mandatory=false
	//@ name=RecordingInformation
	//@ order=1
	std::vector<RecordingInformation> _recordingInformation;

};


inline const std::vector<RecordingInformation>& FindRecordingResultList::getRecordingInformation() const
{
	return _recordingInformation;
}


inline std::vector<RecordingInformation>& FindRecordingResultList::getRecordingInformation()
{
	return _recordingInformation;
}


inline const std::string& FindRecordingResultList::getSearchState() const
{
	return _searchState;
}


inline void FindRecordingResultList::setRecordingInformation(const std::vector<RecordingInformation>& val)
{
	_recordingInformation = val;
}


inline void FindRecordingResultList::setRecordingInformation(std::vector<RecordingInformation>&& val)
{
	_recordingInformation = std::move(val);
}


inline void FindRecordingResultList::setSearchState(const std::string& val)
{
	_searchState = val;
}


inline void FindRecordingResultList::setSearchState(std::string&& val)
{
	_searchState = std::move(val);
}


} // ONVIF


#endif // ONVIF_FindRecordingResultList_INCLUDED
