// 
// MediaUri.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_MediaUri_INCLUDED
#define ONVIF_MediaUri_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/URI.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=MediaUri
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API MediaUri
{
public:
	MediaUri();

	MediaUri(
		const Poco::URI& uri, 
		bool invalidAfterConnect, 
		bool invalidAfterReboot, 
		const std::string& timeout);

	virtual ~MediaUri();

	bool getInvalidAfterConnect() const;

	bool getInvalidAfterReboot() const;

	const std::string& getTimeout() const;

	const Poco::URI& getUri() const;

	void setInvalidAfterConnect(bool val);

	void setInvalidAfterReboot(bool val);

	void setTimeout(const std::string& val);

	void setTimeout(std::string&& val);

	void setUri(const Poco::URI& val);

	void setUri(Poco::URI&& val);

private:
	//@ name=Uri
	//@ order=0
	Poco::URI _uri;

	//@ name=InvalidAfterConnect
	//@ order=1
	bool _invalidAfterConnect;

	//@ name=InvalidAfterReboot
	//@ order=2
	bool _invalidAfterReboot;

	//@ name=Timeout
	//@ order=3
	std::string _timeout;

};


inline bool MediaUri::getInvalidAfterConnect() const
{
	return _invalidAfterConnect;
}


inline bool MediaUri::getInvalidAfterReboot() const
{
	return _invalidAfterReboot;
}


inline const std::string& MediaUri::getTimeout() const
{
	return _timeout;
}


inline const Poco::URI& MediaUri::getUri() const
{
	return _uri;
}


inline void MediaUri::setInvalidAfterConnect(bool val)
{
	_invalidAfterConnect = val;
}


inline void MediaUri::setInvalidAfterReboot(bool val)
{
	_invalidAfterReboot = val;
}


inline void MediaUri::setTimeout(const std::string& val)
{
	_timeout = val;
}


inline void MediaUri::setTimeout(std::string&& val)
{
	_timeout = std::move(val);
}


inline void MediaUri::setUri(const Poco::URI& val)
{
	_uri = val;
}


inline void MediaUri::setUri(Poco::URI&& val)
{
	_uri = std::move(val);
}


} // ONVIF


#endif // ONVIF_MediaUri_INCLUDED
