// 
// PTZPresetTourSupported.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_PTZPresetTourSupported_INCLUDED
#define ONVIF_PTZPresetTourSupported_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class PTZPresetTourSupportedExtension;
} 


namespace ONVIF {


//@ name=PTZPresetTourSupported
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API PTZPresetTourSupported
{
public:
	PTZPresetTourSupported();

	PTZPresetTourSupported(
		int maximumNumberOfPresetTours, 
		const std::vector<std::string>& pTZPresetTourOperation, 
		const Poco::SharedPtr<PTZPresetTourSupportedExtension>& extension);

	virtual ~PTZPresetTourSupported();

	const Poco::SharedPtr<PTZPresetTourSupportedExtension>& getExtension() const;

	int getMaximumNumberOfPresetTours() const;

	const std::vector<std::string>& getPTZPresetTourOperation() const;

	std::vector<std::string>& getPTZPresetTourOperation();

	void setExtension(const Poco::SharedPtr<PTZPresetTourSupportedExtension>& val);

	void setExtension(Poco::SharedPtr<PTZPresetTourSupportedExtension>&& val);

	void setMaximumNumberOfPresetTours(int val);

	void setPTZPresetTourOperation(const std::vector<std::string>& val);

	void setPTZPresetTourOperation(std::vector<std::string>&& val);

private:
	//@ name=MaximumNumberOfPresetTours
	//@ order=0
	int _maximumNumberOfPresetTours;

	//@ mandatory=false
	//@ name=PTZPresetTourOperation
	//@ order=1
	std::vector<std::string> _pTZPresetTourOperation;

	//@ mandatory=false
	//@ name=Extension
	//@ order=2
	Poco::SharedPtr<PTZPresetTourSupportedExtension> _extension;

};


inline const Poco::SharedPtr<PTZPresetTourSupportedExtension>& PTZPresetTourSupported::getExtension() const
{
	return _extension;
}


inline int PTZPresetTourSupported::getMaximumNumberOfPresetTours() const
{
	return _maximumNumberOfPresetTours;
}


inline const std::vector<std::string>& PTZPresetTourSupported::getPTZPresetTourOperation() const
{
	return _pTZPresetTourOperation;
}


inline std::vector<std::string>& PTZPresetTourSupported::getPTZPresetTourOperation()
{
	return _pTZPresetTourOperation;
}


inline void PTZPresetTourSupported::setExtension(const Poco::SharedPtr<PTZPresetTourSupportedExtension>& val)
{
	_extension = val;
}


inline void PTZPresetTourSupported::setExtension(Poco::SharedPtr<PTZPresetTourSupportedExtension>&& val)
{
	_extension = std::move(val);
}


inline void PTZPresetTourSupported::setMaximumNumberOfPresetTours(int val)
{
	_maximumNumberOfPresetTours = val;
}


inline void PTZPresetTourSupported::setPTZPresetTourOperation(const std::vector<std::string>& val)
{
	_pTZPresetTourOperation = val;
}


inline void PTZPresetTourSupported::setPTZPresetTourOperation(std::vector<std::string>&& val)
{
	_pTZPresetTourOperation = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/PTZPresetTourSupportedExtension.h"


#endif // ONVIF_PTZPresetTourSupported_INCLUDED
