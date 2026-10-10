// 
// MotionExpression.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_MotionExpression_INCLUDED
#define ONVIF_MotionExpression_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=MotionExpression
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API MotionExpression
{
public:
	MotionExpression();

	MotionExpression(
		const Poco::Optional<std::string>& type, 
		const std::string& expression);

	virtual ~MotionExpression();

	const std::string& getExpression() const;

	const Poco::Optional<std::string>& getType() const;

	void setExpression(const std::string& val);

	void setExpression(std::string&& val);

	void setType(const Poco::Optional<std::string>& val);

	void setType(Poco::Optional<std::string>&& val);

private:
	//@ mandatory=false
	//@ name=Type
	//@ order=0
	//@ type=attr
	Poco::Optional<std::string> _type;

	//@ name=Expression
	//@ order=1
	std::string _expression;

};


inline const std::string& MotionExpression::getExpression() const
{
	return _expression;
}


inline const Poco::Optional<std::string>& MotionExpression::getType() const
{
	return _type;
}


inline void MotionExpression::setExpression(const std::string& val)
{
	_expression = val;
}


inline void MotionExpression::setExpression(std::string&& val)
{
	_expression = std::move(val);
}


inline void MotionExpression::setType(const Poco::Optional<std::string>& val)
{
	_type = val;
}


inline void MotionExpression::setType(Poco::Optional<std::string>&& val)
{
	_type = std::move(val);
}


} // ONVIF


#endif // ONVIF_MotionExpression_INCLUDED
