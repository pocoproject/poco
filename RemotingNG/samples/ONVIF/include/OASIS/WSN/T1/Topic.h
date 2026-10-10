// 
// Topic.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef OASIS_WSN_T1_Topic_INCLUDED
#define OASIS_WSN_T1_Topic_INCLUDED


#include "OASIS/WSN/T1/ExtensibleDocumented.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace OASIS {
namespace WSN {
namespace T1 {


//@ name=TopicType
//@ namespace="http://docs.oasis-open.org/wsn/t-1"
//@ serialize
class ONVIF_API Topic: public ExtensibleDocumented
{
public:
	Topic();

	Topic(
		const Poco::SharedPtr<Documentation>& documentation, 
		const Poco::Optional<std::string>& parent);

	virtual ~Topic();

	const Poco::Optional<std::string>& getParent() const;

	void setParent(const Poco::Optional<std::string>& val);

	void setParent(Poco::Optional<std::string>&& val);

private:
	//@ mandatory=false
	//@ name=parent
	//@ order=0
	//@ type=attr
	Poco::Optional<std::string> _parent;

};


inline const Poco::Optional<std::string>& Topic::getParent() const
{
	return _parent;
}


inline void Topic::setParent(const Poco::Optional<std::string>& val)
{
	_parent = val;
}


inline void Topic::setParent(Poco::Optional<std::string>&& val)
{
	_parent = std::move(val);
}


} } } // OASIS::WSN::T1


#endif // OASIS_WSN_T1_Topic_INCLUDED
