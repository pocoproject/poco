// 
// AnalyticsEngineInputInfo.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_AnalyticsEngineInputInfo_INCLUDED
#define ONVIF_AnalyticsEngineInputInfo_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class AnalyticsEngineInputInfoExtension;
class Config;
} 


namespace ONVIF {


//@ name=AnalyticsEngineInputInfo
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API AnalyticsEngineInputInfo
{
public:
	AnalyticsEngineInputInfo();

	AnalyticsEngineInputInfo(
		const Poco::SharedPtr<Config>& inputInfo, 
		const Poco::SharedPtr<AnalyticsEngineInputInfoExtension>& extension);

	virtual ~AnalyticsEngineInputInfo();

	const Poco::SharedPtr<AnalyticsEngineInputInfoExtension>& getExtension() const;

	const Poco::SharedPtr<Config>& getInputInfo() const;

	void setExtension(const Poco::SharedPtr<AnalyticsEngineInputInfoExtension>& val);

	void setExtension(Poco::SharedPtr<AnalyticsEngineInputInfoExtension>&& val);

	void setInputInfo(const Poco::SharedPtr<Config>& val);

	void setInputInfo(Poco::SharedPtr<Config>&& val);

private:
	//@ mandatory=false
	//@ name=InputInfo
	//@ order=0
	Poco::SharedPtr<Config> _inputInfo;

	//@ mandatory=false
	//@ name=Extension
	//@ order=1
	Poco::SharedPtr<AnalyticsEngineInputInfoExtension> _extension;

};


inline const Poco::SharedPtr<AnalyticsEngineInputInfoExtension>& AnalyticsEngineInputInfo::getExtension() const
{
	return _extension;
}


inline const Poco::SharedPtr<Config>& AnalyticsEngineInputInfo::getInputInfo() const
{
	return _inputInfo;
}


inline void AnalyticsEngineInputInfo::setExtension(const Poco::SharedPtr<AnalyticsEngineInputInfoExtension>& val)
{
	_extension = val;
}


inline void AnalyticsEngineInputInfo::setExtension(Poco::SharedPtr<AnalyticsEngineInputInfoExtension>&& val)
{
	_extension = std::move(val);
}


inline void AnalyticsEngineInputInfo::setInputInfo(const Poco::SharedPtr<Config>& val)
{
	_inputInfo = val;
}


inline void AnalyticsEngineInputInfo::setInputInfo(Poco::SharedPtr<Config>&& val)
{
	_inputInfo = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/AnalyticsEngineInputInfoExtension.h"
#include "ONVIF/Config.h"


#endif // ONVIF_AnalyticsEngineInputInfo_INCLUDED
