// 
// ElementItemDescription.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_ElementItemDescription_INCLUDED
#define ONVIF_ElementItemDescription_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name="#ElementItemDescription"
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API ElementItemDescription
{
public:
	ElementItemDescription();

	ElementItemDescription(
		const std::string& name, 
		const std::string& type);

	virtual ~ElementItemDescription();

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


inline const std::string& ElementItemDescription::getName() const
{
	return _name;
}


inline const std::string& ElementItemDescription::getType() const
{
	return _type;
}


inline void ElementItemDescription::setName(const std::string& val)
{
	_name = val;
}


inline void ElementItemDescription::setName(std::string&& val)
{
	_name = std::move(val);
}


inline void ElementItemDescription::setType(const std::string& val)
{
	_type = val;
}


inline void ElementItemDescription::setType(std::string&& val)
{
	_type = std::move(val);
}


} // ONVIF


#endif // ONVIF_ElementItemDescription_INCLUDED
