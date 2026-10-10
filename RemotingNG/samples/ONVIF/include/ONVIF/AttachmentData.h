// 
// AttachmentData.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_AttachmentData_INCLUDED
#define ONVIF_AttachmentData_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "W3/XOP/Include.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=AttachmentData
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API AttachmentData
{
public:
	AttachmentData();

	AttachmentData(
		const Poco::Optional<std::string>& contentType, 
		const W3::XOP::Include& include);

	virtual ~AttachmentData();

	const Poco::Optional<std::string>& getContentType() const;

	const W3::XOP::Include& getInclude() const;

	void setContentType(const Poco::Optional<std::string>& val);

	void setContentType(Poco::Optional<std::string>&& val);

	void setInclude(const W3::XOP::Include& val);

	void setInclude(W3::XOP::Include&& val);

private:
	//@ mandatory=false
	//@ name=contentType
	//@ namespace="http://www.w3.org/2005/05/xmlmime"
	//@ order=0
	//@ type=attr
	Poco::Optional<std::string> _contentType;

	//@ name=Include
	//@ namespace="http://www.w3.org/2004/08/xop/include"
	//@ order=1
	W3::XOP::Include _include;

};


inline const Poco::Optional<std::string>& AttachmentData::getContentType() const
{
	return _contentType;
}


inline const W3::XOP::Include& AttachmentData::getInclude() const
{
	return _include;
}


inline void AttachmentData::setContentType(const Poco::Optional<std::string>& val)
{
	_contentType = val;
}


inline void AttachmentData::setContentType(Poco::Optional<std::string>&& val)
{
	_contentType = std::move(val);
}


inline void AttachmentData::setInclude(const W3::XOP::Include& val)
{
	_include = val;
}


inline void AttachmentData::setInclude(W3::XOP::Include&& val)
{
	_include = std::move(val);
}


} // ONVIF


#endif // ONVIF_AttachmentData_INCLUDED
