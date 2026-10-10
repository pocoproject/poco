// 
// GetRecordingJobsResponseItem.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_GetRecordingJobsResponseItem_INCLUDED
#define ONVIF_GetRecordingJobsResponseItem_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/RecordingJobConfiguration.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=GetRecordingJobsResponseItem
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API GetRecordingJobsResponseItem
{
public:
	GetRecordingJobsResponseItem();

	GetRecordingJobsResponseItem(
		const std::string& jobToken, 
		const RecordingJobConfiguration& jobConfiguration);

	virtual ~GetRecordingJobsResponseItem();

	const RecordingJobConfiguration& getJobConfiguration() const;

	const std::string& getJobToken() const;

	void setJobConfiguration(const RecordingJobConfiguration& val);

	void setJobConfiguration(RecordingJobConfiguration&& val);

	void setJobToken(const std::string& val);

	void setJobToken(std::string&& val);

private:
	//@ name=JobToken
	//@ order=0
	std::string _jobToken;

	//@ name=JobConfiguration
	//@ order=1
	RecordingJobConfiguration _jobConfiguration;

};


inline const RecordingJobConfiguration& GetRecordingJobsResponseItem::getJobConfiguration() const
{
	return _jobConfiguration;
}


inline const std::string& GetRecordingJobsResponseItem::getJobToken() const
{
	return _jobToken;
}


inline void GetRecordingJobsResponseItem::setJobConfiguration(const RecordingJobConfiguration& val)
{
	_jobConfiguration = val;
}


inline void GetRecordingJobsResponseItem::setJobConfiguration(RecordingJobConfiguration&& val)
{
	_jobConfiguration = std::move(val);
}


inline void GetRecordingJobsResponseItem::setJobToken(const std::string& val)
{
	_jobToken = val;
}


inline void GetRecordingJobsResponseItem::setJobToken(std::string&& val)
{
	_jobToken = std::move(val);
}


} // ONVIF


#endif // ONVIF_GetRecordingJobsResponseItem_INCLUDED
