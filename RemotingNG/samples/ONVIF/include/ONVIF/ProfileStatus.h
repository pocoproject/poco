// 
// ProfileStatus.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_ProfileStatus_INCLUDED
#define ONVIF_ProfileStatus_INCLUDED


#include "ONVIF/ActiveConnection.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class ProfileStatusExtension;
} 


namespace ONVIF {


//@ name=ProfileStatus
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API ProfileStatus
{
public:
	ProfileStatus();

	ProfileStatus(
		const std::vector<ActiveConnection>& activeConnections, 
		const Poco::SharedPtr<ProfileStatusExtension>& extension);

	virtual ~ProfileStatus();

	const std::vector<ActiveConnection>& getActiveConnections() const;

	std::vector<ActiveConnection>& getActiveConnections();

	const Poco::SharedPtr<ProfileStatusExtension>& getExtension() const;

	void setActiveConnections(const std::vector<ActiveConnection>& val);

	void setActiveConnections(std::vector<ActiveConnection>&& val);

	void setExtension(const Poco::SharedPtr<ProfileStatusExtension>& val);

	void setExtension(Poco::SharedPtr<ProfileStatusExtension>&& val);

private:
	//@ mandatory=false
	//@ name=ActiveConnections
	//@ order=0
	std::vector<ActiveConnection> _activeConnections;

	//@ mandatory=false
	//@ name=Extension
	//@ order=1
	Poco::SharedPtr<ProfileStatusExtension> _extension;

};


inline const std::vector<ActiveConnection>& ProfileStatus::getActiveConnections() const
{
	return _activeConnections;
}


inline std::vector<ActiveConnection>& ProfileStatus::getActiveConnections()
{
	return _activeConnections;
}


inline const Poco::SharedPtr<ProfileStatusExtension>& ProfileStatus::getExtension() const
{
	return _extension;
}


inline void ProfileStatus::setActiveConnections(const std::vector<ActiveConnection>& val)
{
	_activeConnections = val;
}


inline void ProfileStatus::setActiveConnections(std::vector<ActiveConnection>&& val)
{
	_activeConnections = std::move(val);
}


inline void ProfileStatus::setExtension(const Poco::SharedPtr<ProfileStatusExtension>& val)
{
	_extension = val;
}


inline void ProfileStatus::setExtension(Poco::SharedPtr<ProfileStatusExtension>&& val)
{
	_extension = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/ProfileStatusExtension.h"


#endif // ONVIF_ProfileStatus_INCLUDED
