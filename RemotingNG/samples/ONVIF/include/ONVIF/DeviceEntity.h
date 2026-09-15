// 
// DeviceEntity.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DeviceEntity_INCLUDED
#define ONVIF_DeviceEntity_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=DeviceEntity
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API DeviceEntity
{
public:
	DeviceEntity();

	DeviceEntity(const std::string& token);

	virtual ~DeviceEntity();

	const std::string& getToken() const;

	void setToken(const std::string& val);

	void setToken(std::string&& val);

private:
	//@ name=token
	//@ order=0
	//@ type=attr
	std::string _token;

};


inline const std::string& DeviceEntity::getToken() const
{
	return _token;
}


inline void DeviceEntity::setToken(const std::string& val)
{
	_token = val;
}


inline void DeviceEntity::setToken(std::string&& val)
{
	_token = std::move(val);
}


} // ONVIF


#endif // ONVIF_DeviceEntity_INCLUDED
