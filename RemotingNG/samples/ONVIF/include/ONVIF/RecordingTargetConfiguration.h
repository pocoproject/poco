// 
// RecordingTargetConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_RecordingTargetConfiguration_INCLUDED
#define ONVIF_RecordingTargetConfiguration_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/RecordingEncryption.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=RecordingTargetConfiguration
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API RecordingTargetConfiguration
{
public:
	RecordingTargetConfiguration();

	RecordingTargetConfiguration(
		const std::string& storage, 
		const std::string& format, 
		const Poco::Optional<std::string>& prefix, 
		const Poco::Optional<std::string>& postfix, 
		const Poco::Optional<std::string>& spanDuration, 
		const std::string& segmentDuration, 
		const std::vector<RecordingEncryption>& encryption);

	virtual ~RecordingTargetConfiguration();

	const std::vector<RecordingEncryption>& getEncryption() const;

	std::vector<RecordingEncryption>& getEncryption();

	const std::string& getFormat() const;

	const Poco::Optional<std::string>& getPostfix() const;

	const Poco::Optional<std::string>& getPrefix() const;

	const std::string& getSegmentDuration() const;

	const Poco::Optional<std::string>& getSpanDuration() const;

	const std::string& getStorage() const;

	void setEncryption(const std::vector<RecordingEncryption>& val);

	void setEncryption(std::vector<RecordingEncryption>&& val);

	void setFormat(const std::string& val);

	void setFormat(std::string&& val);

	void setPostfix(const Poco::Optional<std::string>& val);

	void setPostfix(Poco::Optional<std::string>&& val);

	void setPrefix(const Poco::Optional<std::string>& val);

	void setPrefix(Poco::Optional<std::string>&& val);

	void setSegmentDuration(const std::string& val);

	void setSegmentDuration(std::string&& val);

	void setSpanDuration(const Poco::Optional<std::string>& val);

	void setSpanDuration(Poco::Optional<std::string>&& val);

	void setStorage(const std::string& val);

	void setStorage(std::string&& val);

private:
	//@ name=Storage
	//@ order=0
	std::string _storage;

	//@ name=Format
	//@ order=1
	std::string _format;

	//@ mandatory=false
	//@ name=Prefix
	//@ order=2
	Poco::Optional<std::string> _prefix;

	//@ mandatory=false
	//@ name=Postfix
	//@ order=3
	Poco::Optional<std::string> _postfix;

	//@ mandatory=false
	//@ name=SpanDuration
	//@ order=4
	Poco::Optional<std::string> _spanDuration;

	//@ name=SegmentDuration
	//@ order=5
	std::string _segmentDuration;

	//@ mandatory=false
	//@ name=Encryption
	//@ order=6
	std::vector<RecordingEncryption> _encryption;

};


inline const std::vector<RecordingEncryption>& RecordingTargetConfiguration::getEncryption() const
{
	return _encryption;
}


inline std::vector<RecordingEncryption>& RecordingTargetConfiguration::getEncryption()
{
	return _encryption;
}


inline const std::string& RecordingTargetConfiguration::getFormat() const
{
	return _format;
}


inline const Poco::Optional<std::string>& RecordingTargetConfiguration::getPostfix() const
{
	return _postfix;
}


inline const Poco::Optional<std::string>& RecordingTargetConfiguration::getPrefix() const
{
	return _prefix;
}


inline const std::string& RecordingTargetConfiguration::getSegmentDuration() const
{
	return _segmentDuration;
}


inline const Poco::Optional<std::string>& RecordingTargetConfiguration::getSpanDuration() const
{
	return _spanDuration;
}


inline const std::string& RecordingTargetConfiguration::getStorage() const
{
	return _storage;
}


inline void RecordingTargetConfiguration::setEncryption(const std::vector<RecordingEncryption>& val)
{
	_encryption = val;
}


inline void RecordingTargetConfiguration::setEncryption(std::vector<RecordingEncryption>&& val)
{
	_encryption = std::move(val);
}


inline void RecordingTargetConfiguration::setFormat(const std::string& val)
{
	_format = val;
}


inline void RecordingTargetConfiguration::setFormat(std::string&& val)
{
	_format = std::move(val);
}


inline void RecordingTargetConfiguration::setPostfix(const Poco::Optional<std::string>& val)
{
	_postfix = val;
}


inline void RecordingTargetConfiguration::setPostfix(Poco::Optional<std::string>&& val)
{
	_postfix = std::move(val);
}


inline void RecordingTargetConfiguration::setPrefix(const Poco::Optional<std::string>& val)
{
	_prefix = val;
}


inline void RecordingTargetConfiguration::setPrefix(Poco::Optional<std::string>&& val)
{
	_prefix = std::move(val);
}


inline void RecordingTargetConfiguration::setSegmentDuration(const std::string& val)
{
	_segmentDuration = val;
}


inline void RecordingTargetConfiguration::setSegmentDuration(std::string&& val)
{
	_segmentDuration = std::move(val);
}


inline void RecordingTargetConfiguration::setSpanDuration(const Poco::Optional<std::string>& val)
{
	_spanDuration = val;
}


inline void RecordingTargetConfiguration::setSpanDuration(Poco::Optional<std::string>&& val)
{
	_spanDuration = std::move(val);
}


inline void RecordingTargetConfiguration::setStorage(const std::string& val)
{
	_storage = val;
}


inline void RecordingTargetConfiguration::setStorage(std::string&& val)
{
	_storage = std::move(val);
}


} // ONVIF


#endif // ONVIF_RecordingTargetConfiguration_INCLUDED
