// 
// SystemLogUri.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_SystemLogUri_INCLUDED
#define ONVIF_SystemLogUri_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/SystemLog.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/URI.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=SystemLogUri
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API SystemLogUri
{
public:
	SystemLogUri();

	SystemLogUri(
		const SystemLog& type, 
		const Poco::URI& uri);

	virtual ~SystemLogUri();

	const SystemLog& getType() const;

	const Poco::URI& getUri() const;

	void setType(const SystemLog& val);

	void setType(SystemLog&& val);

	void setUri(const Poco::URI& val);

	void setUri(Poco::URI&& val);

private:
	//@ name=Type
	//@ order=0
	SystemLog _type;

	//@ name=Uri
	//@ order=1
	Poco::URI _uri;

};


inline const SystemLog& SystemLogUri::getType() const
{
	return _type;
}


inline const Poco::URI& SystemLogUri::getUri() const
{
	return _uri;
}


inline void SystemLogUri::setType(const SystemLog& val)
{
	_type = val;
}


inline void SystemLogUri::setType(SystemLog&& val)
{
	_type = std::move(val);
}


inline void SystemLogUri::setUri(const Poco::URI& val)
{
	_uri = val;
}


inline void SystemLogUri::setUri(Poco::URI&& val)
{
	_uri = std::move(val);
}


} // ONVIF


#endif // ONVIF_SystemLogUri_INCLUDED
