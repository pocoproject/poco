// 
// MetadataAttributes.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_MetadataAttributes_INCLUDED
#define ONVIF_MetadataAttributes_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=MetadataAttributes
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API MetadataAttributes
{
public:
	MetadataAttributes();

	MetadataAttributes(
		const std::vector<std::vector<std::string>>& ptzSpaces, 
		bool canContainPTZ, 
		bool canContainAnalytics, 
		bool canContainNotifications);

	virtual ~MetadataAttributes();

	bool getCanContainAnalytics() const;

	bool getCanContainNotifications() const;

	bool getCanContainPTZ() const;

	const std::vector<std::vector<std::string>>& getPtzSpaces() const;

	std::vector<std::vector<std::string>>& getPtzSpaces();

	void setCanContainAnalytics(bool val);

	void setCanContainNotifications(bool val);

	void setCanContainPTZ(bool val);

	void setPtzSpaces(const std::vector<std::vector<std::string>>& val);

	void setPtzSpaces(std::vector<std::vector<std::string>>&& val);

private:
	//@ mandatory=false
	//@ name=PtzSpaces
	//@ order=0
	//@ type=attr
	std::vector<std::vector<std::string>> _ptzSpaces;

	//@ name=CanContainPTZ
	//@ order=1
	bool _canContainPTZ;

	//@ name=CanContainAnalytics
	//@ order=2
	bool _canContainAnalytics;

	//@ name=CanContainNotifications
	//@ order=3
	bool _canContainNotifications;

};


inline bool MetadataAttributes::getCanContainAnalytics() const
{
	return _canContainAnalytics;
}


inline bool MetadataAttributes::getCanContainNotifications() const
{
	return _canContainNotifications;
}


inline bool MetadataAttributes::getCanContainPTZ() const
{
	return _canContainPTZ;
}


inline const std::vector<std::vector<std::string>>& MetadataAttributes::getPtzSpaces() const
{
	return _ptzSpaces;
}


inline std::vector<std::vector<std::string>>& MetadataAttributes::getPtzSpaces()
{
	return _ptzSpaces;
}


inline void MetadataAttributes::setCanContainAnalytics(bool val)
{
	_canContainAnalytics = val;
}


inline void MetadataAttributes::setCanContainNotifications(bool val)
{
	_canContainNotifications = val;
}


inline void MetadataAttributes::setCanContainPTZ(bool val)
{
	_canContainPTZ = val;
}


inline void MetadataAttributes::setPtzSpaces(const std::vector<std::vector<std::string>>& val)
{
	_ptzSpaces = val;
}


inline void MetadataAttributes::setPtzSpaces(std::vector<std::vector<std::string>>&& val)
{
	_ptzSpaces = std::move(val);
}


} // ONVIF


#endif // ONVIF_MetadataAttributes_INCLUDED
