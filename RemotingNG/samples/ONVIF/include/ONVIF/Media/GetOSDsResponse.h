// 
// GetOSDsResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_GetOSDsResponse_INCLUDED
#define ONVIF_Media_GetOSDsResponse_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/OSDConfiguration.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#GetOSDsResponse"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API GetOSDsResponse
{
public:
	GetOSDsResponse();

	GetOSDsResponse(const std::vector<ONVIF::OSDConfiguration>& oSDs);

	virtual ~GetOSDsResponse();

	const std::vector<ONVIF::OSDConfiguration>& getOSDs() const;

	std::vector<ONVIF::OSDConfiguration>& getOSDs();

	void setOSDs(const std::vector<ONVIF::OSDConfiguration>& val);

	void setOSDs(std::vector<ONVIF::OSDConfiguration>&& val);

private:
	//@ mandatory=false
	//@ name=OSDs
	//@ order=0
	std::vector<ONVIF::OSDConfiguration> _oSDs;

};


inline const std::vector<ONVIF::OSDConfiguration>& GetOSDsResponse::getOSDs() const
{
	return _oSDs;
}


inline std::vector<ONVIF::OSDConfiguration>& GetOSDsResponse::getOSDs()
{
	return _oSDs;
}


inline void GetOSDsResponse::setOSDs(const std::vector<ONVIF::OSDConfiguration>& val)
{
	_oSDs = val;
}


inline void GetOSDsResponse::setOSDs(std::vector<ONVIF::OSDConfiguration>&& val)
{
	_oSDs = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_GetOSDsResponse_INCLUDED
