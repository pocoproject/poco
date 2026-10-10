// 
// GetSystemUrisResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_GetSystemUrisResponse_INCLUDED
#define ONVIF_Device_GetSystemUrisResponse_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/URI.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {
class Extension;
} } 
namespace ONVIF {
class SystemLogUriList;
} 


namespace ONVIF {
namespace Device {


//@ name="#GetSystemUrisResponse"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API GetSystemUrisResponse
{
public:
	GetSystemUrisResponse();

	GetSystemUrisResponse(
		const Poco::SharedPtr<ONVIF::SystemLogUriList>& systemLogUris, 
		const Poco::Optional<Poco::URI>& supportInfoUri, 
		const Poco::Optional<Poco::URI>& systemBackupUri, 
		const Poco::SharedPtr<Extension>& extension);

	virtual ~GetSystemUrisResponse();

	const Poco::SharedPtr<Extension>& getExtension() const;

	const Poco::Optional<Poco::URI>& getSupportInfoUri() const;

	const Poco::Optional<Poco::URI>& getSystemBackupUri() const;

	const Poco::SharedPtr<ONVIF::SystemLogUriList>& getSystemLogUris() const;

	void setExtension(const Poco::SharedPtr<Extension>& val);

	void setExtension(Poco::SharedPtr<Extension>&& val);

	void setSupportInfoUri(const Poco::Optional<Poco::URI>& val);

	void setSupportInfoUri(Poco::Optional<Poco::URI>&& val);

	void setSystemBackupUri(const Poco::Optional<Poco::URI>& val);

	void setSystemBackupUri(Poco::Optional<Poco::URI>&& val);

	void setSystemLogUris(const Poco::SharedPtr<ONVIF::SystemLogUriList>& val);

	void setSystemLogUris(Poco::SharedPtr<ONVIF::SystemLogUriList>&& val);

private:
	//@ mandatory=false
	//@ name=SystemLogUris
	//@ order=0
	Poco::SharedPtr<ONVIF::SystemLogUriList> _systemLogUris;

	//@ mandatory=false
	//@ name=SupportInfoUri
	//@ order=1
	Poco::Optional<Poco::URI> _supportInfoUri;

	//@ mandatory=false
	//@ name=SystemBackupUri
	//@ order=2
	Poco::Optional<Poco::URI> _systemBackupUri;

	//@ mandatory=false
	//@ name=Extension
	//@ order=3
	Poco::SharedPtr<Extension> _extension;

};


inline const Poco::SharedPtr<Extension>& GetSystemUrisResponse::getExtension() const
{
	return _extension;
}


inline const Poco::Optional<Poco::URI>& GetSystemUrisResponse::getSupportInfoUri() const
{
	return _supportInfoUri;
}


inline const Poco::Optional<Poco::URI>& GetSystemUrisResponse::getSystemBackupUri() const
{
	return _systemBackupUri;
}


inline const Poco::SharedPtr<ONVIF::SystemLogUriList>& GetSystemUrisResponse::getSystemLogUris() const
{
	return _systemLogUris;
}


inline void GetSystemUrisResponse::setExtension(const Poco::SharedPtr<Extension>& val)
{
	_extension = val;
}


inline void GetSystemUrisResponse::setExtension(Poco::SharedPtr<Extension>&& val)
{
	_extension = std::move(val);
}


inline void GetSystemUrisResponse::setSupportInfoUri(const Poco::Optional<Poco::URI>& val)
{
	_supportInfoUri = val;
}


inline void GetSystemUrisResponse::setSupportInfoUri(Poco::Optional<Poco::URI>&& val)
{
	_supportInfoUri = std::move(val);
}


inline void GetSystemUrisResponse::setSystemBackupUri(const Poco::Optional<Poco::URI>& val)
{
	_systemBackupUri = val;
}


inline void GetSystemUrisResponse::setSystemBackupUri(Poco::Optional<Poco::URI>&& val)
{
	_systemBackupUri = std::move(val);
}


inline void GetSystemUrisResponse::setSystemLogUris(const Poco::SharedPtr<ONVIF::SystemLogUriList>& val)
{
	_systemLogUris = val;
}


inline void GetSystemUrisResponse::setSystemLogUris(Poco::SharedPtr<ONVIF::SystemLogUriList>&& val)
{
	_systemLogUris = std::move(val);
}


} } // ONVIF::Device


// The following headers are required for template instantiation.
#include "ONVIF/Device/Extension.h"
#include "ONVIF/SystemLogUriList.h"


#endif // ONVIF_Device_GetSystemUrisResponse_INCLUDED
