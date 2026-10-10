// 
// ConfigDescription.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_ConfigDescription_INCLUDED
#define ONVIF_ConfigDescription_INCLUDED


#include "ONVIF/ItemListDescription.h"
#include "ONVIF/Messages.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class ConfigDescriptionExtension;
} 


namespace ONVIF {


//@ name=ConfigDescription
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API ConfigDescription
{
public:
	ConfigDescription();

	ConfigDescription(
		const std::string& name, 
		Poco::Optional<bool> fixed, 
		Poco::Optional<int> maxInstances, 
		const ItemListDescription& parameters, 
		const std::vector<Messages>& messages, 
		const Poco::SharedPtr<ConfigDescriptionExtension>& extension);

	virtual ~ConfigDescription();

	const Poco::SharedPtr<ConfigDescriptionExtension>& getExtension() const;

	Poco::Optional<bool> getFixed() const;

	Poco::Optional<int> getMaxInstances() const;

	const std::vector<Messages>& getMessages() const;

	std::vector<Messages>& getMessages();

	const std::string& getName() const;

	const ItemListDescription& getParameters() const;

	void setExtension(const Poco::SharedPtr<ConfigDescriptionExtension>& val);

	void setExtension(Poco::SharedPtr<ConfigDescriptionExtension>&& val);

	void setFixed(Poco::Optional<bool> val);

	void setMaxInstances(Poco::Optional<int> val);

	void setMessages(const std::vector<Messages>& val);

	void setMessages(std::vector<Messages>&& val);

	void setName(const std::string& val);

	void setName(std::string&& val);

	void setParameters(const ItemListDescription& val);

	void setParameters(ItemListDescription&& val);

private:
	//@ name=Name
	//@ order=0
	//@ type=attr
	std::string _name;

	//@ mandatory=false
	//@ name=fixed
	//@ order=1
	//@ type=attr
	Poco::Optional<bool> _fixed;

	//@ mandatory=false
	//@ name=maxInstances
	//@ order=2
	//@ type=attr
	Poco::Optional<int> _maxInstances;

	//@ name=Parameters
	//@ order=3
	ItemListDescription _parameters;

	//@ mandatory=false
	//@ name=Messages
	//@ order=4
	std::vector<Messages> _messages;

	//@ mandatory=false
	//@ name=Extension
	//@ order=5
	Poco::SharedPtr<ConfigDescriptionExtension> _extension;

};


inline const Poco::SharedPtr<ConfigDescriptionExtension>& ConfigDescription::getExtension() const
{
	return _extension;
}


inline Poco::Optional<bool> ConfigDescription::getFixed() const
{
	return _fixed;
}


inline Poco::Optional<int> ConfigDescription::getMaxInstances() const
{
	return _maxInstances;
}


inline const std::vector<Messages>& ConfigDescription::getMessages() const
{
	return _messages;
}


inline std::vector<Messages>& ConfigDescription::getMessages()
{
	return _messages;
}


inline const std::string& ConfigDescription::getName() const
{
	return _name;
}


inline const ItemListDescription& ConfigDescription::getParameters() const
{
	return _parameters;
}


inline void ConfigDescription::setExtension(const Poco::SharedPtr<ConfigDescriptionExtension>& val)
{
	_extension = val;
}


inline void ConfigDescription::setExtension(Poco::SharedPtr<ConfigDescriptionExtension>&& val)
{
	_extension = std::move(val);
}


inline void ConfigDescription::setFixed(Poco::Optional<bool> val)
{
	_fixed = val;
}


inline void ConfigDescription::setMaxInstances(Poco::Optional<int> val)
{
	_maxInstances = val;
}


inline void ConfigDescription::setMessages(const std::vector<Messages>& val)
{
	_messages = val;
}


inline void ConfigDescription::setMessages(std::vector<Messages>&& val)
{
	_messages = std::move(val);
}


inline void ConfigDescription::setName(const std::string& val)
{
	_name = val;
}


inline void ConfigDescription::setName(std::string&& val)
{
	_name = std::move(val);
}


inline void ConfigDescription::setParameters(const ItemListDescription& val)
{
	_parameters = val;
}


inline void ConfigDescription::setParameters(ItemListDescription&& val)
{
	_parameters = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/ConfigDescriptionExtension.h"


#endif // ONVIF_ConfigDescription_INCLUDED
