// 
// FocusOptions20Extension.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_FocusOptions20Extension_INCLUDED
#define ONVIF_FocusOptions20Extension_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=FocusOptions20Extension
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API FocusOptions20Extension
{
public:
	FocusOptions20Extension();

	FocusOptions20Extension(const Poco::Optional<std::vector<std::string>>& aFModes);

	virtual ~FocusOptions20Extension();

	const Poco::Optional<std::vector<std::string>>& getAFModes() const;

	void setAFModes(const Poco::Optional<std::vector<std::string>>& val);

	void setAFModes(Poco::Optional<std::vector<std::string>>&& val);

private:
	//@ mandatory=false
	//@ name=AFModes
	//@ order=0
	Poco::Optional<std::vector<std::string>> _aFModes;

};


inline const Poco::Optional<std::vector<std::string>>& FocusOptions20Extension::getAFModes() const
{
	return _aFModes;
}


inline void FocusOptions20Extension::setAFModes(const Poco::Optional<std::vector<std::string>>& val)
{
	_aFModes = val;
}


inline void FocusOptions20Extension::setAFModes(Poco::Optional<std::vector<std::string>>&& val)
{
	_aFModes = std::move(val);
}


} // ONVIF


#endif // ONVIF_FocusOptions20Extension_INCLUDED
