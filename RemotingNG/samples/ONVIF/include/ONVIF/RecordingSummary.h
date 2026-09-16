// 
// RecordingSummary.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_RecordingSummary_INCLUDED
#define ONVIF_RecordingSummary_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/DateTime.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=RecordingSummary
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API RecordingSummary
{
public:
	RecordingSummary();

	RecordingSummary(
		const Poco::DateTime& dataFrom, 
		const Poco::DateTime& dataUntil, 
		int numberRecordings);

	virtual ~RecordingSummary();

	//@ return={xsdType=dateTime}
	const Poco::DateTime& getDataFrom() const;

	//@ return={xsdType=dateTime}
	const Poco::DateTime& getDataUntil() const;

	int getNumberRecordings() const;

	//@ $val={xsdType=dateTime}
	void setDataFrom(const Poco::DateTime& val);

	//@ $val={xsdType=dateTime}
	void setDataFrom(Poco::DateTime&& val);

	//@ $val={xsdType=dateTime}
	void setDataUntil(const Poco::DateTime& val);

	//@ $val={xsdType=dateTime}
	void setDataUntil(Poco::DateTime&& val);

	void setNumberRecordings(int val);

private:
	//@ name=DataFrom
	//@ order=0
	//@ xsdType=dateTime
	Poco::DateTime _dataFrom;

	//@ name=DataUntil
	//@ order=1
	//@ xsdType=dateTime
	Poco::DateTime _dataUntil;

	//@ name=NumberRecordings
	//@ order=2
	int _numberRecordings;

};


inline const Poco::DateTime& RecordingSummary::getDataFrom() const
{
	return _dataFrom;
}


inline const Poco::DateTime& RecordingSummary::getDataUntil() const
{
	return _dataUntil;
}


inline int RecordingSummary::getNumberRecordings() const
{
	return _numberRecordings;
}


inline void RecordingSummary::setDataFrom(const Poco::DateTime& val)
{
	_dataFrom = val;
}


inline void RecordingSummary::setDataFrom(Poco::DateTime&& val)
{
	_dataFrom = std::move(val);
}


inline void RecordingSummary::setDataUntil(const Poco::DateTime& val)
{
	_dataUntil = val;
}


inline void RecordingSummary::setDataUntil(Poco::DateTime&& val)
{
	_dataUntil = std::move(val);
}


inline void RecordingSummary::setNumberRecordings(int val)
{
	_numberRecordings = val;
}


} // ONVIF


#endif // ONVIF_RecordingSummary_INCLUDED
