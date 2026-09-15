// 
// ConvertTempResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef WebServiceX_ConvertTempResponse_INCLUDED
#define WebServiceX_ConvertTempResponse_INCLUDED


#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"


namespace WebServiceX {


//@ name="#ConvertTempResponse"
//@ namespace="http://www.webserviceX.NET/"
//@ serialize
class ConvertTempResponse
{
public:
	ConvertTempResponse();

	ConvertTempResponse(double convertTempResult);

	virtual ~ConvertTempResponse();

	double getConvertTempResult() const;

	void setConvertTempResult(double val);

private:
	//@ name=ConvertTempResult
	//@ order=0
	double _convertTempResult;

};


inline double ConvertTempResponse::getConvertTempResult() const
{
	return _convertTempResult;
}


inline void ConvertTempResponse::setConvertTempResult(double val)
{
	_convertTempResult = val;
}


} // WebServiceX


#endif // WebServiceX_ConvertTempResponse_INCLUDED
