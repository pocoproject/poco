// 
// RotateOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_RotateOptions_INCLUDED
#define ONVIF_RotateOptions_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class IntItems;
class RotateOptionsExtension;
} 


namespace ONVIF {


//@ name=RotateOptions
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API RotateOptions
{
public:
	RotateOptions();

	RotateOptions(
		Poco::Optional<bool> reboot, 
		const std::vector<std::string>& mode, 
		const Poco::SharedPtr<IntItems>& degreeList, 
		const Poco::SharedPtr<RotateOptionsExtension>& extension);

	virtual ~RotateOptions();

	const Poco::SharedPtr<IntItems>& getDegreeList() const;

	const Poco::SharedPtr<RotateOptionsExtension>& getExtension() const;

	const std::vector<std::string>& getMode() const;

	std::vector<std::string>& getMode();

	Poco::Optional<bool> getReboot() const;

	void setDegreeList(const Poco::SharedPtr<IntItems>& val);

	void setDegreeList(Poco::SharedPtr<IntItems>&& val);

	void setExtension(const Poco::SharedPtr<RotateOptionsExtension>& val);

	void setExtension(Poco::SharedPtr<RotateOptionsExtension>&& val);

	void setMode(const std::vector<std::string>& val);

	void setMode(std::vector<std::string>&& val);

	void setReboot(Poco::Optional<bool> val);

private:
	//@ mandatory=false
	//@ name=Reboot
	//@ order=0
	//@ type=attr
	Poco::Optional<bool> _reboot;

	//@ name=Mode
	//@ order=1
	std::vector<std::string> _mode;

	//@ mandatory=false
	//@ name=DegreeList
	//@ order=2
	Poco::SharedPtr<IntItems> _degreeList;

	//@ mandatory=false
	//@ name=Extension
	//@ order=3
	Poco::SharedPtr<RotateOptionsExtension> _extension;

};


inline const Poco::SharedPtr<IntItems>& RotateOptions::getDegreeList() const
{
	return _degreeList;
}


inline const Poco::SharedPtr<RotateOptionsExtension>& RotateOptions::getExtension() const
{
	return _extension;
}


inline const std::vector<std::string>& RotateOptions::getMode() const
{
	return _mode;
}


inline std::vector<std::string>& RotateOptions::getMode()
{
	return _mode;
}


inline Poco::Optional<bool> RotateOptions::getReboot() const
{
	return _reboot;
}


inline void RotateOptions::setDegreeList(const Poco::SharedPtr<IntItems>& val)
{
	_degreeList = val;
}


inline void RotateOptions::setDegreeList(Poco::SharedPtr<IntItems>&& val)
{
	_degreeList = std::move(val);
}


inline void RotateOptions::setExtension(const Poco::SharedPtr<RotateOptionsExtension>& val)
{
	_extension = val;
}


inline void RotateOptions::setExtension(Poco::SharedPtr<RotateOptionsExtension>&& val)
{
	_extension = std::move(val);
}


inline void RotateOptions::setMode(const std::vector<std::string>& val)
{
	_mode = val;
}


inline void RotateOptions::setMode(std::vector<std::string>&& val)
{
	_mode = std::move(val);
}


inline void RotateOptions::setReboot(Poco::Optional<bool> val)
{
	_reboot = val;
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/IntItems.h"
#include "ONVIF/RotateOptionsExtension.h"


#endif // ONVIF_RotateOptions_INCLUDED
