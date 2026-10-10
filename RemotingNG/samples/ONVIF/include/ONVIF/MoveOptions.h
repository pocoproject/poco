// 
// MoveOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_MoveOptions_INCLUDED
#define ONVIF_MoveOptions_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class AbsoluteFocusOptions;
class ContinuousFocusOptions;
class RelativeFocusOptions;
} 


namespace ONVIF {


//@ name=MoveOptions
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API MoveOptions
{
public:
	MoveOptions();

	MoveOptions(
		const Poco::SharedPtr<AbsoluteFocusOptions>& absolute, 
		const Poco::SharedPtr<RelativeFocusOptions>& relative, 
		const Poco::SharedPtr<ContinuousFocusOptions>& continuous);

	virtual ~MoveOptions();

	const Poco::SharedPtr<AbsoluteFocusOptions>& getAbsolute() const;

	const Poco::SharedPtr<ContinuousFocusOptions>& getContinuous() const;

	const Poco::SharedPtr<RelativeFocusOptions>& getRelative() const;

	void setAbsolute(const Poco::SharedPtr<AbsoluteFocusOptions>& val);

	void setAbsolute(Poco::SharedPtr<AbsoluteFocusOptions>&& val);

	void setContinuous(const Poco::SharedPtr<ContinuousFocusOptions>& val);

	void setContinuous(Poco::SharedPtr<ContinuousFocusOptions>&& val);

	void setRelative(const Poco::SharedPtr<RelativeFocusOptions>& val);

	void setRelative(Poco::SharedPtr<RelativeFocusOptions>&& val);

private:
	//@ mandatory=false
	//@ name=Absolute
	//@ order=0
	Poco::SharedPtr<AbsoluteFocusOptions> _absolute;

	//@ mandatory=false
	//@ name=Relative
	//@ order=1
	Poco::SharedPtr<RelativeFocusOptions> _relative;

	//@ mandatory=false
	//@ name=Continuous
	//@ order=2
	Poco::SharedPtr<ContinuousFocusOptions> _continuous;

};


inline const Poco::SharedPtr<AbsoluteFocusOptions>& MoveOptions::getAbsolute() const
{
	return _absolute;
}


inline const Poco::SharedPtr<ContinuousFocusOptions>& MoveOptions::getContinuous() const
{
	return _continuous;
}


inline const Poco::SharedPtr<RelativeFocusOptions>& MoveOptions::getRelative() const
{
	return _relative;
}


inline void MoveOptions::setAbsolute(const Poco::SharedPtr<AbsoluteFocusOptions>& val)
{
	_absolute = val;
}


inline void MoveOptions::setAbsolute(Poco::SharedPtr<AbsoluteFocusOptions>&& val)
{
	_absolute = std::move(val);
}


inline void MoveOptions::setContinuous(const Poco::SharedPtr<ContinuousFocusOptions>& val)
{
	_continuous = val;
}


inline void MoveOptions::setContinuous(Poco::SharedPtr<ContinuousFocusOptions>&& val)
{
	_continuous = std::move(val);
}


inline void MoveOptions::setRelative(const Poco::SharedPtr<RelativeFocusOptions>& val)
{
	_relative = val;
}


inline void MoveOptions::setRelative(Poco::SharedPtr<RelativeFocusOptions>&& val)
{
	_relative = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/AbsoluteFocusOptions.h"
#include "ONVIF/ContinuousFocusOptions.h"
#include "ONVIF/RelativeFocusOptions.h"


#endif // ONVIF_MoveOptions_INCLUDED
