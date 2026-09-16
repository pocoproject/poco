// 
// SimpleItemDescription.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_SimpleItemDescription_INCLUDED
#define ONVIF_SimpleItemDescription_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name="#SimpleItemDescription"
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API SimpleItemDescription
{
public:
	SimpleItemDescription();

	SimpleItemDescription(
		const std::string& name, 
		const std::string& type);

	virtual ~SimpleItemDescription();

	const std::string& getName() const;

	const std::string& getType() const;

	void setName(const std::string& val);

	void setName(std::string&& val);

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

};


inline const std::string& SimpleItemDescription::getName() const
{
	return _name;
}


inline const std::string& SimpleItemDescription::getType() const
{
	return _type;
}


inline void SimpleItemDescription::setName(const std::string& val)
{
	_name = val;
}


inline void SimpleItemDescription::setName(std::string&& val)
{
	_name = std::move(val);
}


inline void SimpleItemDescription::setType(const std::string& val)
{
	_type = val;
}


inline void SimpleItemDescription::setType(std::string&& val)
{
	_type = std::move(val);
}


} // ONVIF


#endif // ONVIF_SimpleItemDescription_INCLUDED
