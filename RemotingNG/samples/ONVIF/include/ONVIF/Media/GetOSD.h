// 
// GetOSD.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_GetOSD_INCLUDED
#define ONVIF_Media_GetOSD_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#GetOSD"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API GetOSD
{
public:
	GetOSD();

	GetOSD(const std::string& oSDToken);

	virtual ~GetOSD();

	const std::string& getOSDToken() const;

	void setOSDToken(const std::string& val);

	void setOSDToken(std::string&& val);

private:
	//@ name=OSDToken
	//@ order=0
	std::string _oSDToken;

};


inline const std::string& GetOSD::getOSDToken() const
{
	return _oSDToken;
}


inline void GetOSD::setOSDToken(const std::string& val)
{
	_oSDToken = val;
}


inline void GetOSD::setOSDToken(std::string&& val)
{
	_oSDToken = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_GetOSD_INCLUDED
