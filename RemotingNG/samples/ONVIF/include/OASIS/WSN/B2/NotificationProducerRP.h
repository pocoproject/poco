// 
// NotificationProducerRP.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef OASIS_WSN_B2_NotificationProducerRP_INCLUDED
#define OASIS_WSN_B2_NotificationProducerRP_INCLUDED


#include "OASIS/WSN/B2/TopicExpression.h"
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
class TopicSet;
} } } 


namespace OASIS {
namespace WSN {
namespace B2 {


//@ name="#NotificationProducerRP"
//@ namespace="http://docs.oasis-open.org/wsn/b-2"
//@ serialize
class ONVIF_API NotificationProducerRP
{
public:
	NotificationProducerRP();

	NotificationProducerRP(
		const std::vector<TopicExpression>& topicExpression, 
		Poco::Optional<bool> fixedTopicSet, 
		const std::vector<Poco::URI>& topicExpressionDialect, 
		const Poco::SharedPtr<OASIS::WSN::T1::TopicSet>& topicSet);

	virtual ~NotificationProducerRP();

	Poco::Optional<bool> getFixedTopicSet() const;

	const std::vector<TopicExpression>& getTopicExpression() const;

	std::vector<TopicExpression>& getTopicExpression();

	const std::vector<Poco::URI>& getTopicExpressionDialect() const;

	std::vector<Poco::URI>& getTopicExpressionDialect();

	const Poco::SharedPtr<OASIS::WSN::T1::TopicSet>& getTopicSet() const;

	void setFixedTopicSet(Poco::Optional<bool> val);

	void setTopicExpression(const std::vector<TopicExpression>& val);

	void setTopicExpression(std::vector<TopicExpression>&& val);

	void setTopicExpressionDialect(const std::vector<Poco::URI>& val);

	void setTopicExpressionDialect(std::vector<Poco::URI>&& val);

	void setTopicSet(const Poco::SharedPtr<OASIS::WSN::T1::TopicSet>& val);

	void setTopicSet(Poco::SharedPtr<OASIS::WSN::T1::TopicSet>&& val);

private:
	//@ mandatory=false
	//@ name=TopicExpression
	//@ order=0
	std::vector<TopicExpression> _topicExpression;

	//@ mandatory=false
	//@ name=FixedTopicSet
	//@ order=1
	Poco::Optional<bool> _fixedTopicSet;

	//@ mandatory=false
	//@ name=TopicExpressionDialect
	//@ order=2
	std::vector<Poco::URI> _topicExpressionDialect;

	//@ mandatory=false
	//@ name=TopicSet
	//@ namespace="http://docs.oasis-open.org/wsn/t-1"
	//@ order=3
	Poco::SharedPtr<OASIS::WSN::T1::TopicSet> _topicSet;

};


inline Poco::Optional<bool> NotificationProducerRP::getFixedTopicSet() const
{
	return _fixedTopicSet;
}


inline const std::vector<TopicExpression>& NotificationProducerRP::getTopicExpression() const
{
	return _topicExpression;
}


inline std::vector<TopicExpression>& NotificationProducerRP::getTopicExpression()
{
	return _topicExpression;
}


inline const std::vector<Poco::URI>& NotificationProducerRP::getTopicExpressionDialect() const
{
	return _topicExpressionDialect;
}


inline std::vector<Poco::URI>& NotificationProducerRP::getTopicExpressionDialect()
{
	return _topicExpressionDialect;
}


inline const Poco::SharedPtr<OASIS::WSN::T1::TopicSet>& NotificationProducerRP::getTopicSet() const
{
	return _topicSet;
}


inline void NotificationProducerRP::setFixedTopicSet(Poco::Optional<bool> val)
{
	_fixedTopicSet = val;
}


inline void NotificationProducerRP::setTopicExpression(const std::vector<TopicExpression>& val)
{
	_topicExpression = val;
}


inline void NotificationProducerRP::setTopicExpression(std::vector<TopicExpression>&& val)
{
	_topicExpression = std::move(val);
}


inline void NotificationProducerRP::setTopicExpressionDialect(const std::vector<Poco::URI>& val)
{
	_topicExpressionDialect = val;
}


inline void NotificationProducerRP::setTopicExpressionDialect(std::vector<Poco::URI>&& val)
{
	_topicExpressionDialect = std::move(val);
}


inline void NotificationProducerRP::setTopicSet(const Poco::SharedPtr<OASIS::WSN::T1::TopicSet>& val)
{
	_topicSet = val;
}


inline void NotificationProducerRP::setTopicSet(Poco::SharedPtr<OASIS::WSN::T1::TopicSet>&& val)
{
	_topicSet = std::move(val);
}


} } } // OASIS::WSN::B2


// The following headers are required for template instantiation.
#include "OASIS/WSN/T1/TopicSet.h"


#endif // OASIS_WSN_B2_NotificationProducerRP_INCLUDED
