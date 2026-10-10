// 
// Config.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Config_INCLUDED
#define ONVIF_Config_INCLUDED


#include "ONVIF/ItemList.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=Config
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API Config
{
public:
	Config();

	Config(
		const std::string& name, 
		const std::string& type, 
		const ItemList& parameters);

	virtual ~Config();

	const std::string& getName() const;

	const ItemList& getParameters() const;

	const std::string& getType() const;

	void setName(const std::string& val);

	void setName(std::string&& val);

	void setParameters(const ItemList& val);

	void setParameters(ItemList&& val);

	void setType(const std::string& val);

	void setType(std::string&& val);

private:
	//@ name=Name
	//@ order=0
	//@ type=attr
	std::string _name;

	//@ name=Type
	//@ order=1
	//@ type=attr
	std::string _type;

	//@ name=Parameters
	//@ order=2
	ItemList _parameters;

};


inline const std::string& Config::getName() const
{
	return _name;
}


inline const ItemList& Config::getParameters() const
{
	return _parameters;
}


inline const std::string& Config::getType() const
{
	return _type;
}


inline void Config::setName(const std::string& val)
{
	_name = val;
}


inline void Config::setName(std::string&& val)
{
	_name = std::move(val);
}


inline void Config::setParameters(const ItemList& val)
{
	_parameters = val;
}


inline void Config::setParameters(ItemList&& val)
{
	_parameters = std::move(val);
}


inline void Config::setType(const std::string& val)
{
	_type = val;
}


inline void Config::setType(std::string&& val)
{
	_type = std::move(val);
}


} // ONVIF


#endif // ONVIF_Config_INCLUDED
