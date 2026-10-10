// 
// TopicNamespace.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef OASIS_WSN_T1_TopicNamespace_INCLUDED
#define OASIS_WSN_T1_TopicNamespace_INCLUDED


#include "OASIS/WSN/T1/ExtensibleDocumented.h"
#include "OASIS/WSN/T1/Topic.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/URI.h"
#include <string>
#include <vector>


namespace OASIS {
namespace WSN {
namespace T1 {


//@ name=TopicNamespaceType
//@ namespace="http://docs.oasis-open.org/wsn/t-1"
//@ serialize
class ONVIF_API TopicNamespace: public ExtensibleDocumented
{
public:
	TopicNamespace();

	TopicNamespace(
		const Poco::SharedPtr<Documentation>& documentation, 
		const Poco::Optional<std::string>& name, 
		const Poco::URI& targetNamespace, 
		Poco::Optional<bool> final, 
		const std::vector<Topic>& topic);

	virtual ~TopicNamespace();

	Poco::Optional<bool> getFinal() const;

	const Poco::Optional<std::string>& getName() const;

	const Poco::URI& getTargetNamespace() const;

	const std::vector<Topic>& getTopic() const;

	std::vector<Topic>& getTopic();

	void setFinal(Poco::Optional<bool> val);

	void setName(const Poco::Optional<std::string>& val);

	void setName(Poco::Optional<std::string>&& val);

	void setTargetNamespace(const Poco::URI& val);

	void setTargetNamespace(Poco::URI&& val);

	void setTopic(const std::vector<Topic>& val);

	void setTopic(std::vector<Topic>&& val);

private:
	//@ mandatory=false
	//@ name=name
	//@ order=0
	//@ type=attr
	Poco::Optional<std::string> _name;

	//@ name=targetNamespace
	//@ order=1
	//@ type=attr
	Poco::URI _targetNamespace;

	//@ mandatory=false
	//@ name=final
	//@ order=2
	//@ type=attr
	Poco::Optional<bool> _final;

	//@ mandatory=false
	//@ name=Topic
	//@ order=3
	std::vector<Topic> _topic;

};


inline Poco::Optional<bool> TopicNamespace::getFinal() const
{
	return _final;
}


inline const Poco::Optional<std::string>& TopicNamespace::getName() const
{
	return _name;
}


inline const Poco::URI& TopicNamespace::getTargetNamespace() const
{
	return _targetNamespace;
}


inline const std::vector<Topic>& TopicNamespace::getTopic() const
{
	return _topic;
}


inline std::vector<Topic>& TopicNamespace::getTopic()
{
	return _topic;
}


inline void TopicNamespace::setFinal(Poco::Optional<bool> val)
{
	_final = val;
}


inline void TopicNamespace::setName(const Poco::Optional<std::string>& val)
{
	_name = val;
}


inline void TopicNamespace::setName(Poco::Optional<std::string>&& val)
{
	_name = std::move(val);
}


inline void TopicNamespace::setTargetNamespace(const Poco::URI& val)
{
	_targetNamespace = val;
}


inline void TopicNamespace::setTargetNamespace(Poco::URI&& val)
{
	_targetNamespace = std::move(val);
}


inline void TopicNamespace::setTopic(const std::vector<Topic>& val)
{
	_topic = val;
}


inline void TopicNamespace::setTopic(std::vector<Topic>&& val)
{
	_topic = std::move(val);
}


} } } // OASIS::WSN::T1


#endif // OASIS_WSN_T1_TopicNamespace_INCLUDED
