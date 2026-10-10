// 
// Filter.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Filter_INCLUDED
#define ONVIF_Filter_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name="#Filter"
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API Filter
{
public:
	Filter();

	Filter(
		const std::string& topic, 
		const Poco::Optional<std::string>& source);

	virtual ~Filter();

	const Poco::Optional<std::string>& getSource() const;

	const std::string& getTopic() const;

	void setSource(const Poco::Optional<std::string>& val);

	void setSource(Poco::Optional<std::string>&& val);

	void setTopic(const std::string& val);

	void setTopic(std::string&& val);

private:
	//@ name=Topic
	//@ order=0
	std::string _topic;

	//@ mandatory=false
	//@ name=Source
	//@ order=1
	Poco::Optional<std::string> _source;

};


inline const Poco::Optional<std::string>& Filter::getSource() const
{
	return _source;
}


inline const std::string& Filter::getTopic() const
{
	return _topic;
}


inline void Filter::setSource(const Poco::Optional<std::string>& val)
{
	_source = val;
}


inline void Filter::setSource(Poco::Optional<std::string>&& val)
{
	_source = std::move(val);
}


inline void Filter::setTopic(const std::string& val)
{
	_topic = val;
}


inline void Filter::setTopic(std::string&& val)
{
	_topic = std::move(val);
}


} // ONVIF


#endif // ONVIF_Filter_INCLUDED
