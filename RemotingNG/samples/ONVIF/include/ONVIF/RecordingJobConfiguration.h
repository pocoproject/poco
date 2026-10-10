// 
// RecordingJobConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_RecordingJobConfiguration_INCLUDED
#define ONVIF_RecordingJobConfiguration_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/RecordingJobSource.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class RecordingEventFilter;
class RecordingJobConfigurationExtension;
} 


namespace ONVIF {


//@ name=RecordingJobConfiguration
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API RecordingJobConfiguration
{
public:
	RecordingJobConfiguration();

	RecordingJobConfiguration(
		const Poco::Optional<std::string>& scheduleToken, 
		const std::string& recordingToken, 
		const std::string& mode, 
		int priority, 
		const std::vector<RecordingJobSource>& source, 
		const Poco::SharedPtr<RecordingJobConfigurationExtension>& extension, 
		const Poco::SharedPtr<RecordingEventFilter>& eventFilter);

	virtual ~RecordingJobConfiguration();

	const Poco::SharedPtr<RecordingEventFilter>& getEventFilter() const;

	const Poco::SharedPtr<RecordingJobConfigurationExtension>& getExtension() const;

	const std::string& getMode() const;

	int getPriority() const;

	const std::string& getRecordingToken() const;

	const Poco::Optional<std::string>& getScheduleToken() const;

	const std::vector<RecordingJobSource>& getSource() const;

	std::vector<RecordingJobSource>& getSource();

	void setEventFilter(const Poco::SharedPtr<RecordingEventFilter>& val);

	void setEventFilter(Poco::SharedPtr<RecordingEventFilter>&& val);

	void setExtension(const Poco::SharedPtr<RecordingJobConfigurationExtension>& val);

	void setExtension(Poco::SharedPtr<RecordingJobConfigurationExtension>&& val);

	void setMode(const std::string& val);

	void setMode(std::string&& val);

	void setPriority(int val);

	void setRecordingToken(const std::string& val);

	void setRecordingToken(std::string&& val);

	void setScheduleToken(const Poco::Optional<std::string>& val);

	void setScheduleToken(Poco::Optional<std::string>&& val);

	void setSource(const std::vector<RecordingJobSource>& val);

	void setSource(std::vector<RecordingJobSource>&& val);

private:
	//@ mandatory=false
	//@ name=ScheduleToken
	//@ order=0
	//@ type=attr
	Poco::Optional<std::string> _scheduleToken;

	//@ name=RecordingToken
	//@ order=1
	std::string _recordingToken;

	//@ name=Mode
	//@ order=2
	std::string _mode;

	//@ name=Priority
	//@ order=3
	int _priority;

	//@ mandatory=false
	//@ name=Source
	//@ order=4
	std::vector<RecordingJobSource> _source;

	//@ mandatory=false
	//@ name=Extension
	//@ order=5
	Poco::SharedPtr<RecordingJobConfigurationExtension> _extension;

	//@ mandatory=false
	//@ name=EventFilter
	//@ order=6
	Poco::SharedPtr<RecordingEventFilter> _eventFilter;

};


inline const Poco::SharedPtr<RecordingEventFilter>& RecordingJobConfiguration::getEventFilter() const
{
	return _eventFilter;
}


inline const Poco::SharedPtr<RecordingJobConfigurationExtension>& RecordingJobConfiguration::getExtension() const
{
	return _extension;
}


inline const std::string& RecordingJobConfiguration::getMode() const
{
	return _mode;
}


inline int RecordingJobConfiguration::getPriority() const
{
	return _priority;
}


inline const std::string& RecordingJobConfiguration::getRecordingToken() const
{
	return _recordingToken;
}


inline const Poco::Optional<std::string>& RecordingJobConfiguration::getScheduleToken() const
{
	return _scheduleToken;
}


inline const std::vector<RecordingJobSource>& RecordingJobConfiguration::getSource() const
{
	return _source;
}


inline std::vector<RecordingJobSource>& RecordingJobConfiguration::getSource()
{
	return _source;
}


inline void RecordingJobConfiguration::setEventFilter(const Poco::SharedPtr<RecordingEventFilter>& val)
{
	_eventFilter = val;
}


inline void RecordingJobConfiguration::setEventFilter(Poco::SharedPtr<RecordingEventFilter>&& val)
{
	_eventFilter = std::move(val);
}


inline void RecordingJobConfiguration::setExtension(const Poco::SharedPtr<RecordingJobConfigurationExtension>& val)
{
	_extension = val;
}


inline void RecordingJobConfiguration::setExtension(Poco::SharedPtr<RecordingJobConfigurationExtension>&& val)
{
	_extension = std::move(val);
}


inline void RecordingJobConfiguration::setMode(const std::string& val)
{
	_mode = val;
}


inline void RecordingJobConfiguration::setMode(std::string&& val)
{
	_mode = std::move(val);
}


inline void RecordingJobConfiguration::setPriority(int val)
{
	_priority = val;
}


inline void RecordingJobConfiguration::setRecordingToken(const std::string& val)
{
	_recordingToken = val;
}


inline void RecordingJobConfiguration::setRecordingToken(std::string&& val)
{
	_recordingToken = std::move(val);
}


inline void RecordingJobConfiguration::setScheduleToken(const Poco::Optional<std::string>& val)
{
	_scheduleToken = val;
}


inline void RecordingJobConfiguration::setScheduleToken(Poco::Optional<std::string>&& val)
{
	_scheduleToken = std::move(val);
}


inline void RecordingJobConfiguration::setSource(const std::vector<RecordingJobSource>& val)
{
	_source = val;
}


inline void RecordingJobConfiguration::setSource(std::vector<RecordingJobSource>&& val)
{
	_source = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/RecordingEventFilter.h"
#include "ONVIF/RecordingJobConfigurationExtension.h"


#endif // ONVIF_RecordingJobConfiguration_INCLUDED
