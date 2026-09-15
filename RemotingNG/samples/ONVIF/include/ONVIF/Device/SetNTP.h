// 
// SetNTP.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_SetNTP_INCLUDED
#define ONVIF_Device_SetNTP_INCLUDED


#include "ONVIF/NetworkHost.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#SetNTP"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API SetNTP
{
public:
	SetNTP();

	SetNTP(
		bool fromDHCP, 
		const std::vector<ONVIF::NetworkHost>& nTPManual);

	virtual ~SetNTP();

	bool getFromDHCP() const;

	const std::vector<ONVIF::NetworkHost>& getNTPManual() const;

	std::vector<ONVIF::NetworkHost>& getNTPManual();

	void setFromDHCP(bool val);

	void setNTPManual(const std::vector<ONVIF::NetworkHost>& val);

	void setNTPManual(std::vector<ONVIF::NetworkHost>&& val);

private:
	//@ name=FromDHCP
	//@ order=0
	bool _fromDHCP;

	//@ mandatory=false
	//@ name=NTPManual
	//@ order=1
	std::vector<ONVIF::NetworkHost> _nTPManual;

};


inline bool SetNTP::getFromDHCP() const
{
	return _fromDHCP;
}


inline const std::vector<ONVIF::NetworkHost>& SetNTP::getNTPManual() const
{
	return _nTPManual;
}


inline std::vector<ONVIF::NetworkHost>& SetNTP::getNTPManual()
{
	return _nTPManual;
}


inline void SetNTP::setFromDHCP(bool val)
{
	_fromDHCP = val;
}


inline void SetNTP::setNTPManual(const std::vector<ONVIF::NetworkHost>& val)
{
	_nTPManual = val;
}


inline void SetNTP::setNTPManual(std::vector<ONVIF::NetworkHost>&& val)
{
	_nTPManual = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_SetNTP_INCLUDED
