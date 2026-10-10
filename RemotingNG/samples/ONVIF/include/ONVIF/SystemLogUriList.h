// 
// SystemLogUriList.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_SystemLogUriList_INCLUDED
#define ONVIF_SystemLogUriList_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/SystemLogUri.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=SystemLogUriList
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API SystemLogUriList
{
public:
	SystemLogUriList();

	SystemLogUriList(const std::vector<SystemLogUri>& systemLog);

	virtual ~SystemLogUriList();

	const std::vector<SystemLogUri>& getSystemLog() const;

	std::vector<SystemLogUri>& getSystemLog();

	void setSystemLog(const std::vector<SystemLogUri>& val);

	void setSystemLog(std::vector<SystemLogUri>&& val);

private:
	//@ mandatory=false
	//@ name=SystemLog
	//@ order=0
	std::vector<SystemLogUri> _systemLog;

};


inline const std::vector<SystemLogUri>& SystemLogUriList::getSystemLog() const
{
	return _systemLog;
}


inline std::vector<SystemLogUri>& SystemLogUriList::getSystemLog()
{
	return _systemLog;
}


inline void SystemLogUriList::setSystemLog(const std::vector<SystemLogUri>& val)
{
	_systemLog = val;
}


inline void SystemLogUriList::setSystemLog(std::vector<SystemLogUri>&& val)
{
	_systemLog = std::move(val);
}


} // ONVIF


#endif // ONVIF_SystemLogUriList_INCLUDED
