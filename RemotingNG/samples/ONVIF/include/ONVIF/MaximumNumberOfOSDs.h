// 
// MaximumNumberOfOSDs.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_MaximumNumberOfOSDs_INCLUDED
#define ONVIF_MaximumNumberOfOSDs_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=MaximumNumberOfOSDs
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API MaximumNumberOfOSDs
{
public:
	MaximumNumberOfOSDs();

	MaximumNumberOfOSDs(
		int total, 
		Poco::Optional<int> image, 
		Poco::Optional<int> plainText, 
		Poco::Optional<int> date, 
		Poco::Optional<int> time, 
		Poco::Optional<int> dateAndTime);

	virtual ~MaximumNumberOfOSDs();

	Poco::Optional<int> getDate() const;

	Poco::Optional<int> getDateAndTime() const;

	Poco::Optional<int> getImage() const;

	Poco::Optional<int> getPlainText() const;

	Poco::Optional<int> getTime() const;

	int getTotal() const;

	void setDate(Poco::Optional<int> val);

	void setDateAndTime(Poco::Optional<int> val);

	void setImage(Poco::Optional<int> val);

	void setPlainText(Poco::Optional<int> val);

	void setTime(Poco::Optional<int> val);

	void setTotal(int val);

private:
	//@ name=Total
	//@ order=0
	//@ type=attr
	int _total;

	//@ mandatory=false
	//@ name=Image
	//@ order=1
	//@ type=attr
	Poco::Optional<int> _image;

	//@ mandatory=false
	//@ name=PlainText
	//@ order=2
	//@ type=attr
	Poco::Optional<int> _plainText;

	//@ mandatory=false
	//@ name=Date
	//@ order=3
	//@ type=attr
	Poco::Optional<int> _date;

	//@ mandatory=false
	//@ name=Time
	//@ order=4
	//@ type=attr
	Poco::Optional<int> _time;

	//@ mandatory=false
	//@ name=DateAndTime
	//@ order=5
	//@ type=attr
	Poco::Optional<int> _dateAndTime;

};


inline Poco::Optional<int> MaximumNumberOfOSDs::getDate() const
{
	return _date;
}


inline Poco::Optional<int> MaximumNumberOfOSDs::getDateAndTime() const
{
	return _dateAndTime;
}


inline Poco::Optional<int> MaximumNumberOfOSDs::getImage() const
{
	return _image;
}


inline Poco::Optional<int> MaximumNumberOfOSDs::getPlainText() const
{
	return _plainText;
}


inline Poco::Optional<int> MaximumNumberOfOSDs::getTime() const
{
	return _time;
}


inline int MaximumNumberOfOSDs::getTotal() const
{
	return _total;
}


inline void MaximumNumberOfOSDs::setDate(Poco::Optional<int> val)
{
	_date = val;
}


inline void MaximumNumberOfOSDs::setDateAndTime(Poco::Optional<int> val)
{
	_dateAndTime = val;
}


inline void MaximumNumberOfOSDs::setImage(Poco::Optional<int> val)
{
	_image = val;
}


inline void MaximumNumberOfOSDs::setPlainText(Poco::Optional<int> val)
{
	_plainText = val;
}


inline void MaximumNumberOfOSDs::setTime(Poco::Optional<int> val)
{
	_time = val;
}


inline void MaximumNumberOfOSDs::setTotal(int val)
{
	_total = val;
}


} // ONVIF


#endif // ONVIF_MaximumNumberOfOSDs_INCLUDED
