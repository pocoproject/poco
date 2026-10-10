// 
// FocusMove.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_FocusMove_INCLUDED
#define ONVIF_FocusMove_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class AbsoluteFocus;
class ContinuousFocus;
class RelativeFocus;
} 


namespace ONVIF {


//@ name=FocusMove
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API FocusMove
{
public:
	FocusMove();

	FocusMove(
		const Poco::SharedPtr<AbsoluteFocus>& absolute, 
		const Poco::SharedPtr<RelativeFocus>& relative, 
		const Poco::SharedPtr<ContinuousFocus>& continuous);

	virtual ~FocusMove();

	const Poco::SharedPtr<AbsoluteFocus>& getAbsolute() const;

	const Poco::SharedPtr<ContinuousFocus>& getContinuous() const;

	const Poco::SharedPtr<RelativeFocus>& getRelative() const;

	void setAbsolute(const Poco::SharedPtr<AbsoluteFocus>& val);

	void setAbsolute(Poco::SharedPtr<AbsoluteFocus>&& val);

	void setContinuous(const Poco::SharedPtr<ContinuousFocus>& val);

	void setContinuous(Poco::SharedPtr<ContinuousFocus>&& val);

	void setRelative(const Poco::SharedPtr<RelativeFocus>& val);

	void setRelative(Poco::SharedPtr<RelativeFocus>&& val);

private:
	//@ mandatory=false
	//@ name=Absolute
	//@ order=0
	Poco::SharedPtr<AbsoluteFocus> _absolute;

	//@ mandatory=false
	//@ name=Relative
	//@ order=1
	Poco::SharedPtr<RelativeFocus> _relative;

	//@ mandatory=false
	//@ name=Continuous
	//@ order=2
	Poco::SharedPtr<ContinuousFocus> _continuous;

};


inline const Poco::SharedPtr<AbsoluteFocus>& FocusMove::getAbsolute() const
{
	return _absolute;
}


inline const Poco::SharedPtr<ContinuousFocus>& FocusMove::getContinuous() const
{
	return _continuous;
}


inline const Poco::SharedPtr<RelativeFocus>& FocusMove::getRelative() const
{
	return _relative;
}


inline void FocusMove::setAbsolute(const Poco::SharedPtr<AbsoluteFocus>& val)
{
	_absolute = val;
}


inline void FocusMove::setAbsolute(Poco::SharedPtr<AbsoluteFocus>&& val)
{
	_absolute = std::move(val);
}


inline void FocusMove::setContinuous(const Poco::SharedPtr<ContinuousFocus>& val)
{
	_continuous = val;
}


inline void FocusMove::setContinuous(Poco::SharedPtr<ContinuousFocus>&& val)
{
	_continuous = std::move(val);
}


inline void FocusMove::setRelative(const Poco::SharedPtr<RelativeFocus>& val)
{
	_relative = val;
}


inline void FocusMove::setRelative(Poco::SharedPtr<RelativeFocus>&& val)
{
	_relative = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/AbsoluteFocus.h"
#include "ONVIF/ContinuousFocus.h"
#include "ONVIF/RelativeFocus.h"


#endif // ONVIF_FocusMove_INCLUDED
