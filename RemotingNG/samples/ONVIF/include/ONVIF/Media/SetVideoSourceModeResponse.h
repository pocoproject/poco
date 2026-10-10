// 
// SetVideoSourceModeResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_SetVideoSourceModeResponse_INCLUDED
#define ONVIF_Media_SetVideoSourceModeResponse_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#SetVideoSourceModeResponse"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API SetVideoSourceModeResponse
{
public:
	SetVideoSourceModeResponse();

	SetVideoSourceModeResponse(bool reboot);

	virtual ~SetVideoSourceModeResponse();

	bool getReboot() const;

	void setReboot(bool val);

private:
	//@ name=Reboot
	//@ order=0
	bool _reboot;

};


inline bool SetVideoSourceModeResponse::getReboot() const
{
	return _reboot;
}


inline void SetVideoSourceModeResponse::setReboot(bool val)
{
	_reboot = val;
}


} } // ONVIF::Media


#endif // ONVIF_Media_SetVideoSourceModeResponse_INCLUDED
