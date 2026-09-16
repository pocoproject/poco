// 
// ConfigurationEntity.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_ConfigurationEntity_INCLUDED
#define ONVIF_ConfigurationEntity_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=ConfigurationEntity
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API ConfigurationEntity
{
public:
	ConfigurationEntity();

	ConfigurationEntity(
		const std::string& token, 
		const std::string& name, 
		int useCount);

	virtual ~ConfigurationEntity();

	const std::string& getName() const;

	const std::string& getToken() const;

	int getUseCount() const;

	void setName(const std::string& val);

	void setName(std::string&& val);

	void setToken(const std::string& val);

	void setToken(std::string&& val);

	void setUseCount(int val);

private:
	//@ name=token
	//@ order=0
	//@ type=attr
	std::string _token;

	//@ name=Name
	//@ order=1
	std::string _name;

	//@ name=UseCount
	//@ order=2
	int _useCount;

};


inline const std::string& ConfigurationEntity::getName() const
{
	return _name;
}


inline const std::string& ConfigurationEntity::getToken() const
{
	return _token;
}


inline int ConfigurationEntity::getUseCount() const
{
	return _useCount;
}


inline void ConfigurationEntity::setName(const std::string& val)
{
	_name = val;
}


inline void ConfigurationEntity::setName(std::string&& val)
{
	_name = std::move(val);
}


inline void ConfigurationEntity::setToken(const std::string& val)
{
	_token = val;
}


inline void ConfigurationEntity::setToken(std::string&& val)
{
	_token = std::move(val);
}


inline void ConfigurationEntity::setUseCount(int val)
{
	_useCount = val;
}


} // ONVIF


#endif // ONVIF_ConfigurationEntity_INCLUDED
