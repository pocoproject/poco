// 
// MotionExpressionConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_MotionExpressionConfiguration_INCLUDED
#define ONVIF_MotionExpressionConfiguration_INCLUDED


#include "ONVIF/MotionExpression.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=MotionExpressionConfiguration
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API MotionExpressionConfiguration
{
public:
	MotionExpressionConfiguration();

	MotionExpressionConfiguration(const MotionExpression& motionExpression);

	virtual ~MotionExpressionConfiguration();

	const MotionExpression& getMotionExpression() const;

	void setMotionExpression(const MotionExpression& val);

	void setMotionExpression(MotionExpression&& val);

private:
	//@ name=MotionExpression
	//@ order=0
	MotionExpression _motionExpression;

};


inline const MotionExpression& MotionExpressionConfiguration::getMotionExpression() const
{
	return _motionExpression;
}


inline void MotionExpressionConfiguration::setMotionExpression(const MotionExpression& val)
{
	_motionExpression = val;
}


inline void MotionExpressionConfiguration::setMotionExpression(MotionExpression&& val)
{
	_motionExpression = std::move(val);
}


} // ONVIF


#endif // ONVIF_MotionExpressionConfiguration_INCLUDED
