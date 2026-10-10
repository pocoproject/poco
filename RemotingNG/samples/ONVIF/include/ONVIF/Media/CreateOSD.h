// 
// CreateOSD.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_CreateOSD_INCLUDED
#define ONVIF_Media_CreateOSD_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/OSDConfiguration.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#CreateOSD"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API CreateOSD
{
public:
	CreateOSD();

	CreateOSD(const ONVIF::OSDConfiguration& oSD);

	virtual ~CreateOSD();

	const ONVIF::OSDConfiguration& getOSD() const;

	void setOSD(const ONVIF::OSDConfiguration& val);

	void setOSD(ONVIF::OSDConfiguration&& val);

private:
	//@ name=OSD
	//@ order=0
	ONVIF::OSDConfiguration _oSD;

};


inline const ONVIF::OSDConfiguration& CreateOSD::getOSD() const
{
	return _oSD;
}


inline void CreateOSD::setOSD(const ONVIF::OSDConfiguration& val)
{
	_oSD = val;
}


inline void CreateOSD::setOSD(ONVIF::OSDConfiguration&& val)
{
	_oSD = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_CreateOSD_INCLUDED
