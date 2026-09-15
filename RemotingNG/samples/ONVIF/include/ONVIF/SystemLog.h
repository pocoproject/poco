// 
// SystemLog.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_SystemLog_INCLUDED
#define ONVIF_SystemLog_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class AttachmentData;
} 


namespace ONVIF {


//@ name=SystemLog
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API SystemLog
{
public:
	SystemLog();

	SystemLog(
		const Poco::SharedPtr<AttachmentData>& binary, 
		const Poco::Optional<std::string>& string);

	virtual ~SystemLog();

	const Poco::SharedPtr<AttachmentData>& getBinary() const;

	const Poco::Optional<std::string>& getString() const;

	void setBinary(const Poco::SharedPtr<AttachmentData>& val);

	void setBinary(Poco::SharedPtr<AttachmentData>&& val);

	void setString(const Poco::Optional<std::string>& val);

	void setString(Poco::Optional<std::string>&& val);

private:
	//@ mandatory=false
	//@ name=Binary
	//@ order=0
	Poco::SharedPtr<AttachmentData> _binary;

	//@ mandatory=false
	//@ name=String
	//@ order=1
	Poco::Optional<std::string> _string;

};


inline const Poco::SharedPtr<AttachmentData>& SystemLog::getBinary() const
{
	return _binary;
}


inline const Poco::Optional<std::string>& SystemLog::getString() const
{
	return _string;
}


inline void SystemLog::setBinary(const Poco::SharedPtr<AttachmentData>& val)
{
	_binary = val;
}


inline void SystemLog::setBinary(Poco::SharedPtr<AttachmentData>&& val)
{
	_binary = std::move(val);
}


inline void SystemLog::setString(const Poco::Optional<std::string>& val)
{
	_string = val;
}


inline void SystemLog::setString(Poco::Optional<std::string>&& val)
{
	_string = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/AttachmentData.h"


#endif // ONVIF_SystemLog_INCLUDED
