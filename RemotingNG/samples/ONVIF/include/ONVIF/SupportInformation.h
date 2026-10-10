// 
// SupportInformation.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_SupportInformation_INCLUDED
#define ONVIF_SupportInformation_INCLUDED


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


//@ name=SupportInformation
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API SupportInformation
{
public:
	SupportInformation();

	SupportInformation(
		const Poco::SharedPtr<AttachmentData>& binary, 
		const Poco::Optional<std::string>& string);

	virtual ~SupportInformation();

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


inline const Poco::SharedPtr<AttachmentData>& SupportInformation::getBinary() const
{
	return _binary;
}


inline const Poco::Optional<std::string>& SupportInformation::getString() const
{
	return _string;
}


inline void SupportInformation::setBinary(const Poco::SharedPtr<AttachmentData>& val)
{
	_binary = val;
}


inline void SupportInformation::setBinary(Poco::SharedPtr<AttachmentData>&& val)
{
	_binary = std::move(val);
}


inline void SupportInformation::setString(const Poco::Optional<std::string>& val)
{
	_string = val;
}


inline void SupportInformation::setString(Poco::Optional<std::string>&& val)
{
	_string = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/AttachmentData.h"


#endif // ONVIF_SupportInformation_INCLUDED
