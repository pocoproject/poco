// 
// LoadCACertificates.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_LoadCACertificates_INCLUDED
#define ONVIF_Device_LoadCACertificates_INCLUDED


#include "ONVIF/Certificate.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#LoadCACertificates"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API LoadCACertificates
{
public:
	LoadCACertificates();

	LoadCACertificates(const std::vector<ONVIF::Certificate>& cACertificate);

	virtual ~LoadCACertificates();

	const std::vector<ONVIF::Certificate>& getCACertificate() const;

	std::vector<ONVIF::Certificate>& getCACertificate();

	void setCACertificate(const std::vector<ONVIF::Certificate>& val);

	void setCACertificate(std::vector<ONVIF::Certificate>&& val);

private:
	//@ name=CACertificate
	//@ order=0
	std::vector<ONVIF::Certificate> _cACertificate;

};


inline const std::vector<ONVIF::Certificate>& LoadCACertificates::getCACertificate() const
{
	return _cACertificate;
}


inline std::vector<ONVIF::Certificate>& LoadCACertificates::getCACertificate()
{
	return _cACertificate;
}


inline void LoadCACertificates::setCACertificate(const std::vector<ONVIF::Certificate>& val)
{
	_cACertificate = val;
}


inline void LoadCACertificates::setCACertificate(std::vector<ONVIF::Certificate>&& val)
{
	_cACertificate = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_LoadCACertificates_INCLUDED
