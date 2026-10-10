// 
// EFlipOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_EFlipOptions_INCLUDED
#define ONVIF_EFlipOptions_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class EFlipOptionsExtension;
} 


namespace ONVIF {


//@ name=EFlipOptions
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API EFlipOptions
{
public:
	EFlipOptions();

	EFlipOptions(
		const std::vector<std::string>& mode, 
		const Poco::SharedPtr<EFlipOptionsExtension>& extension);

	virtual ~EFlipOptions();

	const Poco::SharedPtr<EFlipOptionsExtension>& getExtension() const;

	const std::vector<std::string>& getMode() const;

	std::vector<std::string>& getMode();

	void setExtension(const Poco::SharedPtr<EFlipOptionsExtension>& val);

	void setExtension(Poco::SharedPtr<EFlipOptionsExtension>&& val);

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
	Poco::SharedPtr<EFlipOptionsExtension> _extension;

};


inline const Poco::SharedPtr<EFlipOptionsExtension>& EFlipOptions::getExtension() const
{
	return _extension;
}


inline const std::vector<std::string>& EFlipOptions::getMode() const
{
	return _mode;
}


inline std::vector<std::string>& EFlipOptions::getMode()
{
	return _mode;
}


inline void EFlipOptions::setExtension(const Poco::SharedPtr<EFlipOptionsExtension>& val)
{
	_extension = val;
}


inline void EFlipOptions::setExtension(Poco::SharedPtr<EFlipOptionsExtension>&& val)
{
	_extension = std::move(val);
}


inline void EFlipOptions::setMode(const std::vector<std::string>& val)
{
	_mode = val;
}


inline void EFlipOptions::setMode(std::vector<std::string>&& val)
{
	_mode = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/EFlipOptionsExtension.h"


#endif // ONVIF_EFlipOptions_INCLUDED
