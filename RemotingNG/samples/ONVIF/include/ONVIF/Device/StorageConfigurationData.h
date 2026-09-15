// 
// StorageConfigurationData.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_StorageConfigurationData_INCLUDED
#define ONVIF_Device_StorageConfigurationData_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/URI.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {
class Extension_2;
class UserCredential;
} } 


namespace ONVIF {
namespace Device {


//@ name=StorageConfigurationData
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API StorageConfigurationData
{
public:
	StorageConfigurationData();

	StorageConfigurationData(
		const std::string& type, 
		const Poco::Optional<std::string>& region, 
		const Poco::Optional<Poco::URI>& localPath, 
		const Poco::Optional<Poco::URI>& storageUri, 
		const Poco::SharedPtr<UserCredential>& user, 
		const Poco::SharedPtr<Extension_2>& extension);

	virtual ~StorageConfigurationData();

	const Poco::SharedPtr<Extension_2>& getExtension() const;

	const Poco::Optional<Poco::URI>& getLocalPath() const;

	const Poco::Optional<std::string>& getRegion() const;

	const Poco::Optional<Poco::URI>& getStorageUri() const;

	const std::string& getType() const;

	const Poco::SharedPtr<UserCredential>& getUser() const;

	void setExtension(const Poco::SharedPtr<Extension_2>& val);

	void setExtension(Poco::SharedPtr<Extension_2>&& val);

	void setLocalPath(const Poco::Optional<Poco::URI>& val);

	void setLocalPath(Poco::Optional<Poco::URI>&& val);

	void setRegion(const Poco::Optional<std::string>& val);

	void setRegion(Poco::Optional<std::string>&& val);

	void setStorageUri(const Poco::Optional<Poco::URI>& val);

	void setStorageUri(Poco::Optional<Poco::URI>&& val);

	void setType(const std::string& val);

	void setType(std::string&& val);

	void setUser(const Poco::SharedPtr<UserCredential>& val);

	void setUser(Poco::SharedPtr<UserCredential>&& val);

private:
	//@ name=type
	//@ order=0
	//@ type=attr
	std::string _type;

	//@ mandatory=false
	//@ name=Region
	//@ order=1
	//@ type=attr
	Poco::Optional<std::string> _region;

	//@ mandatory=false
	//@ name=LocalPath
	//@ order=2
	Poco::Optional<Poco::URI> _localPath;

	//@ mandatory=false
	//@ name=StorageUri
	//@ order=3
	Poco::Optional<Poco::URI> _storageUri;

	//@ mandatory=false
	//@ name=User
	//@ order=4
	Poco::SharedPtr<UserCredential> _user;

	//@ mandatory=false
	//@ name=Extension
	//@ order=5
	Poco::SharedPtr<Extension_2> _extension;

};


inline const Poco::SharedPtr<Extension_2>& StorageConfigurationData::getExtension() const
{
	return _extension;
}


inline const Poco::Optional<Poco::URI>& StorageConfigurationData::getLocalPath() const
{
	return _localPath;
}


inline const Poco::Optional<std::string>& StorageConfigurationData::getRegion() const
{
	return _region;
}


inline const Poco::Optional<Poco::URI>& StorageConfigurationData::getStorageUri() const
{
	return _storageUri;
}


inline const std::string& StorageConfigurationData::getType() const
{
	return _type;
}


inline const Poco::SharedPtr<UserCredential>& StorageConfigurationData::getUser() const
{
	return _user;
}


inline void StorageConfigurationData::setExtension(const Poco::SharedPtr<Extension_2>& val)
{
	_extension = val;
}


inline void StorageConfigurationData::setExtension(Poco::SharedPtr<Extension_2>&& val)
{
	_extension = std::move(val);
}


inline void StorageConfigurationData::setLocalPath(const Poco::Optional<Poco::URI>& val)
{
	_localPath = val;
}


inline void StorageConfigurationData::setLocalPath(Poco::Optional<Poco::URI>&& val)
{
	_localPath = std::move(val);
}


inline void StorageConfigurationData::setRegion(const Poco::Optional<std::string>& val)
{
	_region = val;
}


inline void StorageConfigurationData::setRegion(Poco::Optional<std::string>&& val)
{
	_region = std::move(val);
}


inline void StorageConfigurationData::setStorageUri(const Poco::Optional<Poco::URI>& val)
{
	_storageUri = val;
}


inline void StorageConfigurationData::setStorageUri(Poco::Optional<Poco::URI>&& val)
{
	_storageUri = std::move(val);
}


inline void StorageConfigurationData::setType(const std::string& val)
{
	_type = val;
}


inline void StorageConfigurationData::setType(std::string&& val)
{
	_type = std::move(val);
}


inline void StorageConfigurationData::setUser(const Poco::SharedPtr<UserCredential>& val)
{
	_user = val;
}


inline void StorageConfigurationData::setUser(Poco::SharedPtr<UserCredential>&& val)
{
	_user = std::move(val);
}


} } // ONVIF::Device


// The following headers are required for template instantiation.
#include "ONVIF/Device/Extension_2.h"
#include "ONVIF/Device/UserCredential.h"


#endif // ONVIF_Device_StorageConfigurationData_INCLUDED
