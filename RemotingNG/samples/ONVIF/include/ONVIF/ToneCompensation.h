// 
// ToneCompensation.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_ToneCompensation_INCLUDED
#define ONVIF_ToneCompensation_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class ToneCompensationExtension;
} 


namespace ONVIF {


//@ name=ToneCompensation
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API ToneCompensation
{
public:
	ToneCompensation();

	ToneCompensation(
		const std::string& mode, 
		Poco::Optional<float> level, 
		const Poco::SharedPtr<ToneCompensationExtension>& extension);

	virtual ~ToneCompensation();

	const Poco::SharedPtr<ToneCompensationExtension>& getExtension() const;

	Poco::Optional<float> getLevel() const;

	const std::string& getMode() const;

	void setExtension(const Poco::SharedPtr<ToneCompensationExtension>& val);

	void setExtension(Poco::SharedPtr<ToneCompensationExtension>&& val);

	void setLevel(Poco::Optional<float> val);

	void setMode(const std::string& val);

	void setMode(std::string&& val);

private:
	//@ name=Mode
	//@ order=0
	std::string _mode;

	//@ mandatory=false
	//@ name=Level
	//@ order=1
	Poco::Optional<float> _level;

	//@ mandatory=false
	//@ name=Extension
	//@ order=2
	Poco::SharedPtr<ToneCompensationExtension> _extension;

};


inline const Poco::SharedPtr<ToneCompensationExtension>& ToneCompensation::getExtension() const
{
	return _extension;
}


inline Poco::Optional<float> ToneCompensation::getLevel() const
{
	return _level;
}


inline const std::string& ToneCompensation::getMode() const
{
	return _mode;
}


inline void ToneCompensation::setExtension(const Poco::SharedPtr<ToneCompensationExtension>& val)
{
	_extension = val;
}


inline void ToneCompensation::setExtension(Poco::SharedPtr<ToneCompensationExtension>&& val)
{
	_extension = std::move(val);
}


inline void ToneCompensation::setLevel(Poco::Optional<float> val)
{
	_level = val;
}


inline void ToneCompensation::setMode(const std::string& val)
{
	_mode = val;
}


inline void ToneCompensation::setMode(std::string&& val)
{
	_mode = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/ToneCompensationExtension.h"


#endif // ONVIF_ToneCompensation_INCLUDED
