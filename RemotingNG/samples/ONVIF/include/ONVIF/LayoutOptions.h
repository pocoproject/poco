// 
// LayoutOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_LayoutOptions_INCLUDED
#define ONVIF_LayoutOptions_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/PaneLayoutOptions.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class LayoutOptionsExtension;
} 


namespace ONVIF {


//@ name=LayoutOptions
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API LayoutOptions
{
public:
	LayoutOptions();

	LayoutOptions(
		const std::vector<PaneLayoutOptions>& paneLayoutOptions, 
		const Poco::SharedPtr<LayoutOptionsExtension>& extension);

	virtual ~LayoutOptions();

	const Poco::SharedPtr<LayoutOptionsExtension>& getExtension() const;

	const std::vector<PaneLayoutOptions>& getPaneLayoutOptions() const;

	std::vector<PaneLayoutOptions>& getPaneLayoutOptions();

	void setExtension(const Poco::SharedPtr<LayoutOptionsExtension>& val);

	void setExtension(Poco::SharedPtr<LayoutOptionsExtension>&& val);

	void setPaneLayoutOptions(const std::vector<PaneLayoutOptions>& val);

	void setPaneLayoutOptions(std::vector<PaneLayoutOptions>&& val);

private:
	//@ name=PaneLayoutOptions
	//@ order=0
	std::vector<PaneLayoutOptions> _paneLayoutOptions;

	//@ mandatory=false
	//@ name=Extension
	//@ order=1
	Poco::SharedPtr<LayoutOptionsExtension> _extension;

};


inline const Poco::SharedPtr<LayoutOptionsExtension>& LayoutOptions::getExtension() const
{
	return _extension;
}


inline const std::vector<PaneLayoutOptions>& LayoutOptions::getPaneLayoutOptions() const
{
	return _paneLayoutOptions;
}


inline std::vector<PaneLayoutOptions>& LayoutOptions::getPaneLayoutOptions()
{
	return _paneLayoutOptions;
}


inline void LayoutOptions::setExtension(const Poco::SharedPtr<LayoutOptionsExtension>& val)
{
	_extension = val;
}


inline void LayoutOptions::setExtension(Poco::SharedPtr<LayoutOptionsExtension>&& val)
{
	_extension = std::move(val);
}


inline void LayoutOptions::setPaneLayoutOptions(const std::vector<PaneLayoutOptions>& val)
{
	_paneLayoutOptions = val;
}


inline void LayoutOptions::setPaneLayoutOptions(std::vector<PaneLayoutOptions>&& val)
{
	_paneLayoutOptions = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/LayoutOptionsExtension.h"


#endif // ONVIF_LayoutOptions_INCLUDED
