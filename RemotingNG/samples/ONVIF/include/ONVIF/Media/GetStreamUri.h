// 
// GetStreamUri.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_GetStreamUri_INCLUDED
#define ONVIF_Media_GetStreamUri_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/StreamSetup.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#GetStreamUri"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API GetStreamUri
{
public:
	GetStreamUri();

	GetStreamUri(
		const ONVIF::StreamSetup& streamSetup, 
		const std::string& profileToken);

	virtual ~GetStreamUri();

	const std::string& getProfileToken() const;

	const ONVIF::StreamSetup& getStreamSetup() const;

	void setProfileToken(const std::string& val);

	void setProfileToken(std::string&& val);

	void setStreamSetup(const ONVIF::StreamSetup& val);

	void setStreamSetup(ONVIF::StreamSetup&& val);

private:
	//@ name=StreamSetup
	//@ order=0
	ONVIF::StreamSetup _streamSetup;

	//@ name=ProfileToken
	//@ order=1
	std::string _profileToken;

};


inline const std::string& GetStreamUri::getProfileToken() const
{
	return _profileToken;
}


inline const ONVIF::StreamSetup& GetStreamUri::getStreamSetup() const
{
	return _streamSetup;
}


inline void GetStreamUri::setProfileToken(const std::string& val)
{
	_profileToken = val;
}


inline void GetStreamUri::setProfileToken(std::string&& val)
{
	_profileToken = std::move(val);
}


inline void GetStreamUri::setStreamSetup(const ONVIF::StreamSetup& val)
{
	_streamSetup = val;
}


inline void GetStreamUri::setStreamSetup(ONVIF::StreamSetup&& val)
{
	_streamSetup = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_GetStreamUri_INCLUDED
