// 
// RecordingConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_RecordingConfiguration_INCLUDED
#define ONVIF_RecordingConfiguration_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/RecordingSourceInformation.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class RecordingTargetConfiguration;
} 


namespace ONVIF {


//@ name=RecordingConfiguration
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API RecordingConfiguration
{
public:
	RecordingConfiguration();

	RecordingConfiguration(
		const RecordingSourceInformation& source, 
		const std::string& content, 
		const std::string& maximumRetentionTime, 
		const Poco::SharedPtr<RecordingTargetConfiguration>& target);

	virtual ~RecordingConfiguration();

	const std::string& getContent() const;

	const std::string& getMaximumRetentionTime() const;

	const RecordingSourceInformation& getSource() const;

	const Poco::SharedPtr<RecordingTargetConfiguration>& getTarget() const;

	void setContent(const std::string& val);

	void setContent(std::string&& val);

	void setMaximumRetentionTime(const std::string& val);

	void setMaximumRetentionTime(std::string&& val);

	void setSource(const RecordingSourceInformation& val);

	void setSource(RecordingSourceInformation&& val);

	void setTarget(const Poco::SharedPtr<RecordingTargetConfiguration>& val);

	void setTarget(Poco::SharedPtr<RecordingTargetConfiguration>&& val);

private:
	//@ name=Source
	//@ order=0
	RecordingSourceInformation _source;

	//@ name=Content
	//@ order=1
	std::string _content;

	//@ name=MaximumRetentionTime
	//@ order=2
	std::string _maximumRetentionTime;

	//@ mandatory=false
	//@ name=Target
	//@ order=3
	Poco::SharedPtr<RecordingTargetConfiguration> _target;

};


inline const std::string& RecordingConfiguration::getContent() const
{
	return _content;
}


inline const std::string& RecordingConfiguration::getMaximumRetentionTime() const
{
	return _maximumRetentionTime;
}


inline const RecordingSourceInformation& RecordingConfiguration::getSource() const
{
	return _source;
}


inline const Poco::SharedPtr<RecordingTargetConfiguration>& RecordingConfiguration::getTarget() const
{
	return _target;
}


inline void RecordingConfiguration::setContent(const std::string& val)
{
	_content = val;
}


inline void RecordingConfiguration::setContent(std::string&& val)
{
	_content = std::move(val);
}


inline void RecordingConfiguration::setMaximumRetentionTime(const std::string& val)
{
	_maximumRetentionTime = val;
}


inline void RecordingConfiguration::setMaximumRetentionTime(std::string&& val)
{
	_maximumRetentionTime = std::move(val);
}


inline void RecordingConfiguration::setSource(const RecordingSourceInformation& val)
{
	_source = val;
}


inline void RecordingConfiguration::setSource(RecordingSourceInformation&& val)
{
	_source = std::move(val);
}


inline void RecordingConfiguration::setTarget(const Poco::SharedPtr<RecordingTargetConfiguration>& val)
{
	_target = val;
}


inline void RecordingConfiguration::setTarget(Poco::SharedPtr<RecordingTargetConfiguration>&& val)
{
	_target = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/RecordingTargetConfiguration.h"


#endif // ONVIF_RecordingConfiguration_INCLUDED
