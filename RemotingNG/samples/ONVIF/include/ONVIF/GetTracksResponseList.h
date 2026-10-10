// 
// GetTracksResponseList.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_GetTracksResponseList_INCLUDED
#define ONVIF_GetTracksResponseList_INCLUDED


#include "ONVIF/GetTracksResponseItem.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=GetTracksResponseList
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API GetTracksResponseList
{
public:
	GetTracksResponseList();

	GetTracksResponseList(const std::vector<GetTracksResponseItem>& track);

	virtual ~GetTracksResponseList();

	const std::vector<GetTracksResponseItem>& getTrack() const;

	std::vector<GetTracksResponseItem>& getTrack();

	void setTrack(const std::vector<GetTracksResponseItem>& val);

	void setTrack(std::vector<GetTracksResponseItem>&& val);

private:
	//@ mandatory=false
	//@ name=Track
	//@ order=0
	std::vector<GetTracksResponseItem> _track;

};


inline const std::vector<GetTracksResponseItem>& GetTracksResponseList::getTrack() const
{
	return _track;
}


inline std::vector<GetTracksResponseItem>& GetTracksResponseList::getTrack()
{
	return _track;
}


inline void GetTracksResponseList::setTrack(const std::vector<GetTracksResponseItem>& val)
{
	_track = val;
}


inline void GetTracksResponseList::setTrack(std::vector<GetTracksResponseItem>&& val)
{
	_track = std::move(val);
}


} // ONVIF


#endif // ONVIF_GetTracksResponseList_INCLUDED
