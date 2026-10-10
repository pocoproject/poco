// 
// TrackInformation.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_TrackInformation_INCLUDED
#define ONVIF_TrackInformation_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/DateTime.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=TrackInformation
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API TrackInformation
{
public:
	TrackInformation();

	TrackInformation(
		const std::string& trackToken, 
		const std::string& trackType, 
		const std::string& description, 
		const Poco::DateTime& dataFrom, 
		const Poco::DateTime& dataTo);

	virtual ~TrackInformation();

	//@ return={xsdType=dateTime}
	const Poco::DateTime& getDataFrom() const;

	//@ return={xsdType=dateTime}
	const Poco::DateTime& getDataTo() const;

	const std::string& getDescription() const;

	const std::string& getTrackToken() const;

	const std::string& getTrackType() const;

	//@ $val={xsdType=dateTime}
	void setDataFrom(const Poco::DateTime& val);

	//@ $val={xsdType=dateTime}
	void setDataFrom(Poco::DateTime&& val);

	//@ $val={xsdType=dateTime}
	void setDataTo(const Poco::DateTime& val);

	//@ $val={xsdType=dateTime}
	void setDataTo(Poco::DateTime&& val);

	void setDescription(const std::string& val);

	void setDescription(std::string&& val);

	void setTrackToken(const std::string& val);

	void setTrackToken(std::string&& val);

	void setTrackType(const std::string& val);

	void setTrackType(std::string&& val);

private:
	//@ name=TrackToken
	//@ order=0
	std::string _trackToken;

	//@ name=TrackType
	//@ order=1
	std::string _trackType;

	//@ name=Description
	//@ order=2
	std::string _description;

	//@ name=DataFrom
	//@ order=3
	//@ xsdType=dateTime
	Poco::DateTime _dataFrom;

	//@ name=DataTo
	//@ order=4
	//@ xsdType=dateTime
	Poco::DateTime _dataTo;

};


inline const Poco::DateTime& TrackInformation::getDataFrom() const
{
	return _dataFrom;
}


inline const Poco::DateTime& TrackInformation::getDataTo() const
{
	return _dataTo;
}


inline const std::string& TrackInformation::getDescription() const
{
	return _description;
}


inline const std::string& TrackInformation::getTrackToken() const
{
	return _trackToken;
}


inline const std::string& TrackInformation::getTrackType() const
{
	return _trackType;
}


inline void TrackInformation::setDataFrom(const Poco::DateTime& val)
{
	_dataFrom = val;
}


inline void TrackInformation::setDataFrom(Poco::DateTime&& val)
{
	_dataFrom = std::move(val);
}


inline void TrackInformation::setDataTo(const Poco::DateTime& val)
{
	_dataTo = val;
}


inline void TrackInformation::setDataTo(Poco::DateTime&& val)
{
	_dataTo = std::move(val);
}


inline void TrackInformation::setDescription(const std::string& val)
{
	_description = val;
}


inline void TrackInformation::setDescription(std::string&& val)
{
	_description = std::move(val);
}


inline void TrackInformation::setTrackToken(const std::string& val)
{
	_trackToken = val;
}


inline void TrackInformation::setTrackToken(std::string&& val)
{
	_trackToken = std::move(val);
}


inline void TrackInformation::setTrackType(const std::string& val)
{
	_trackType = val;
}


inline void TrackInformation::setTrackType(std::string&& val)
{
	_trackType = std::move(val);
}


} // ONVIF


#endif // ONVIF_TrackInformation_INCLUDED
