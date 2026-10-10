// 
// ReverseOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_ReverseOptions_INCLUDED
#define ONVIF_ReverseOptions_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class ReverseOptionsExtension;
} 


namespace ONVIF {


//@ name=ReverseOptions
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API ReverseOptions
{
public:
	ReverseOptions();

	ReverseOptions(
		const std::vector<std::string>& mode, 
		const Poco::SharedPtr<ReverseOptionsExtension>& extension);

	virtual ~ReverseOptions();

	const Poco::SharedPtr<ReverseOptionsExtension>& getExtension() const;

	const std::vector<std::string>& getMode() const;

	std::vector<std::string>& getMode();

	void setExtension(const Poco::SharedPtr<ReverseOptionsExtension>& val);

	void setExtension(Poco::SharedPtr<ReverseOptionsExtension>&& val);

	void setMode(const std::vector<std::string>& val);

	void setMode(std::vector<std::string>&& val);

private:
	//@ mandatory=false
	//@ name=Mode
	//@ order=0
	std::vector<std::string> _mode;

	//@ mandatory=false
	//@ name=Extension
	//@ order=1
	Poco::SharedPtr<ReverseOptionsExtension> _extension;

};


inline const Poco::SharedPtr<ReverseOptionsExtension>& ReverseOptions::getExtension() const
{
	return _extension;
}


inline const std::vector<std::string>& ReverseOptions::getMode() const
{
	return _mode;
}


inline std::vector<std::string>& ReverseOptions::getMode()
{
	return _mode;
}


inline void ReverseOptions::setExtension(const Poco::SharedPtr<ReverseOptionsExtension>& val)
{
	_extension = val;
}


inline void ReverseOptions::setExtension(Poco::SharedPtr<ReverseOptionsExtension>&& val)
{
	_extension = std::move(val);
}


inline void ReverseOptions::setMode(const std::vector<std::string>& val)
{
	_mode = val;
}


inline void ReverseOptions::setMode(std::vector<std::string>&& val)
{
	_mode = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/ReverseOptionsExtension.h"


#endif // ONVIF_ReverseOptions_INCLUDED
