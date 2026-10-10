// 
// RecordingSourceInformation.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_RecordingSourceInformation_INCLUDED
#define ONVIF_RecordingSourceInformation_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/URI.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=RecordingSourceInformation
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API RecordingSourceInformation
{
public:
	RecordingSourceInformation();

	RecordingSourceInformation(
		const Poco::URI& sourceId, 
		const std::string& name, 
		const std::string& location, 
		const std::string& description, 
		const Poco::URI& address);

	virtual ~RecordingSourceInformation();

	const Poco::URI& getAddress() const;

	const std::string& getDescription() const;

	const std::string& getLocation() const;

	const std::string& getName() const;

	const Poco::URI& getSourceId() const;

	void setAddress(const Poco::URI& val);

	void setAddress(Poco::URI&& val);

	void setDescription(const std::string& val);

	void setDescription(std::string&& val);

	void setLocation(const std::string& val);

	void setLocation(std::string&& val);

	void setName(const std::string& val);

	void setName(std::string&& val);

	void setSourceId(const Poco::URI& val);

	void setSourceId(Poco::URI&& val);

private:
	//@ name=SourceId
	//@ order=0
	Poco::URI _sourceId;

	//@ name=Name
	//@ order=1
	std::string _name;

	//@ name=Location
	//@ order=2
	std::string _location;

	//@ name=Description
	//@ order=3
	std::string _description;

	//@ name=Address
	//@ order=4
	Poco::URI _address;

};


inline const Poco::URI& RecordingSourceInformation::getAddress() const
{
	return _address;
}


inline const std::string& RecordingSourceInformation::getDescription() const
{
	return _description;
}


inline const std::string& RecordingSourceInformation::getLocation() const
{
	return _location;
}


inline const std::string& RecordingSourceInformation::getName() const
{
	return _name;
}


inline const Poco::URI& RecordingSourceInformation::getSourceId() const
{
	return _sourceId;
}


inline void RecordingSourceInformation::setAddress(const Poco::URI& val)
{
	_address = val;
}


inline void RecordingSourceInformation::setAddress(Poco::URI&& val)
{
	_address = std::move(val);
}


inline void RecordingSourceInformation::setDescription(const std::string& val)
{
	_description = val;
}


inline void RecordingSourceInformation::setDescription(std::string&& val)
{
	_description = std::move(val);
}


inline void RecordingSourceInformation::setLocation(const std::string& val)
{
	_location = val;
}


inline void RecordingSourceInformation::setLocation(std::string&& val)
{
	_location = std::move(val);
}


inline void RecordingSourceInformation::setName(const std::string& val)
{
	_name = val;
}


inline void RecordingSourceInformation::setName(std::string&& val)
{
	_name = std::move(val);
}


inline void RecordingSourceInformation::setSourceId(const Poco::URI& val)
{
	_sourceId = val;
}


inline void RecordingSourceInformation::setSourceId(Poco::URI&& val)
{
	_sourceId = std::move(val);
}


} // ONVIF


#endif // ONVIF_RecordingSourceInformation_INCLUDED
