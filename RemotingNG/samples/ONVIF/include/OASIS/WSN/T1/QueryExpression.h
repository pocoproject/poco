// 
// QueryExpression.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef OASIS_WSN_T1_QueryExpression_INCLUDED
#define OASIS_WSN_T1_QueryExpression_INCLUDED


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


//@ name=QueryExpressionType
//@ namespace="http://docs.oasis-open.org/wsn/t-1"
//@ serialize
class ONVIF_API QueryExpression
{
public:
	QueryExpression();

	QueryExpression(const Poco::URI& dialect);

	virtual ~QueryExpression();

	const Poco::URI& getDialect() const;

	void setDialect(const Poco::URI& val);

	void setDialect(Poco::URI&& val);

private:
	//@ name=Dialect
	//@ order=0
	//@ type=attr
	Poco::URI _dialect;

};


inline const Poco::URI& QueryExpression::getDialect() const
{
	return _dialect;
}


inline void QueryExpression::setDialect(const Poco::URI& val)
{
	_dialect = val;
}


inline void QueryExpression::setDialect(Poco::URI&& val)
{
	_dialect = std::move(val);
}


} } } // OASIS::WSN::T1


#endif // OASIS_WSN_T1_QueryExpression_INCLUDED
