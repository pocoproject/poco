// 
// RecordingJobStateInformation.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_RecordingJobStateInformation_INCLUDED
#define ONVIF_RecordingJobStateInformation_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/RecordingJobStateSource.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class RecordingJobStateInformationExtension;
} 


namespace ONVIF {


//@ name=RecordingJobStateInformation
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API RecordingJobStateInformation
{
public:
	RecordingJobStateInformation();

	RecordingJobStateInformation(
		const std::string& recordingToken, 
		const std::string& state, 
		const std::vector<RecordingJobStateSource>& sources, 
		const Poco::SharedPtr<RecordingJobStateInformationExtension>& extension);

	virtual ~RecordingJobStateInformation();

	const Poco::SharedPtr<RecordingJobStateInformationExtension>& getExtension() const;

	const std::string& getRecordingToken() const;

	const std::vector<RecordingJobStateSource>& getSources() const;

	std::vector<RecordingJobStateSource>& getSources();

	const std::string& getState() const;

	void setExtension(const Poco::SharedPtr<RecordingJobStateInformationExtension>& val);

	void setExtension(Poco::SharedPtr<RecordingJobStateInformationExtension>&& val);

	void setRecordingToken(const std::string& val);

	void setRecordingToken(std::string&& val);

	void setSources(const std::vector<RecordingJobStateSource>& val);

	void setSources(std::vector<RecordingJobStateSource>&& val);

	void setState(const std::string& val);

	void setState(std::string&& val);

private:
	//@ name=RecordingToken
	//@ order=0
	std::string _recordingToken;

	//@ name=State
	//@ order=1
	std::string _state;

	//@ mandatory=false
	//@ name=Sources
	//@ order=2
	std::vector<RecordingJobStateSource> _sources;

	//@ mandatory=false
	//@ name=Extension
	//@ order=3
	Poco::SharedPtr<RecordingJobStateInformationExtension> _extension;

};


inline const Poco::SharedPtr<RecordingJobStateInformationExtension>& RecordingJobStateInformation::getExtension() const
{
	return _extension;
}


inline const std::string& RecordingJobStateInformation::getRecordingToken() const
{
	return _recordingToken;
}


inline const std::vector<RecordingJobStateSource>& RecordingJobStateInformation::getSources() const
{
	return _sources;
}


inline std::vector<RecordingJobStateSource>& RecordingJobStateInformation::getSources()
{
	return _sources;
}


inline const std::string& RecordingJobStateInformation::getState() const
{
	return _state;
}


inline void RecordingJobStateInformation::setExtension(const Poco::SharedPtr<RecordingJobStateInformationExtension>& val)
{
	_extension = val;
}


inline void RecordingJobStateInformation::setExtension(Poco::SharedPtr<RecordingJobStateInformationExtension>&& val)
{
	_extension = std::move(val);
}


inline void RecordingJobStateInformation::setRecordingToken(const std::string& val)
{
	_recordingToken = val;
}


inline void RecordingJobStateInformation::setRecordingToken(std::string&& val)
{
	_recordingToken = std::move(val);
}


inline void RecordingJobStateInformation::setSources(const std::vector<RecordingJobStateSource>& val)
{
	_sources = val;
}


inline void RecordingJobStateInformation::setSources(std::vector<RecordingJobStateSource>&& val)
{
	_sources = std::move(val);
}


inline void RecordingJobStateInformation::setState(const std::string& val)
{
	_state = val;
}


inline void RecordingJobStateInformation::setState(std::string&& val)
{
	_state = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/RecordingJobStateInformationExtension.h"


#endif // ONVIF_RecordingJobStateInformation_INCLUDED
