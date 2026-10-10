// 
// PTControlDirection.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_PTControlDirection_INCLUDED
#define ONVIF_PTControlDirection_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class EFlip;
class PTControlDirectionExtension;
class Reverse;
} 


namespace ONVIF {


//@ name=PTControlDirection
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API PTControlDirection
{
public:
	PTControlDirection();

	PTControlDirection(
		const Poco::SharedPtr<EFlip>& eFlip, 
		const Poco::SharedPtr<Reverse>& reverse, 
		const Poco::SharedPtr<PTControlDirectionExtension>& extension);

	virtual ~PTControlDirection();

	const Poco::SharedPtr<EFlip>& getEFlip() const;

	const Poco::SharedPtr<PTControlDirectionExtension>& getExtension() const;

	const Poco::SharedPtr<Reverse>& getReverse() const;

	void setEFlip(const Poco::SharedPtr<EFlip>& val);

	void setEFlip(Poco::SharedPtr<EFlip>&& val);

	void setExtension(const Poco::SharedPtr<PTControlDirectionExtension>& val);

	void setExtension(Poco::SharedPtr<PTControlDirectionExtension>&& val);

	void setReverse(const Poco::SharedPtr<Reverse>& val);

	void setReverse(Poco::SharedPtr<Reverse>&& val);

private:
	//@ mandatory=false
	//@ name=EFlip
	//@ order=0
	Poco::SharedPtr<EFlip> _eFlip;

	//@ mandatory=false
	//@ name=Reverse
	//@ order=1
	Poco::SharedPtr<Reverse> _reverse;

	//@ mandatory=false
	//@ name=Extension
	//@ order=2
	Poco::SharedPtr<PTControlDirectionExtension> _extension;

};


inline const Poco::SharedPtr<EFlip>& PTControlDirection::getEFlip() const
{
	return _eFlip;
}


inline const Poco::SharedPtr<PTControlDirectionExtension>& PTControlDirection::getExtension() const
{
	return _extension;
}


inline const Poco::SharedPtr<Reverse>& PTControlDirection::getReverse() const
{
	return _reverse;
}


inline void PTControlDirection::setEFlip(const Poco::SharedPtr<EFlip>& val)
{
	_eFlip = val;
}


inline void PTControlDirection::setEFlip(Poco::SharedPtr<EFlip>&& val)
{
	_eFlip = std::move(val);
}


inline void PTControlDirection::setExtension(const Poco::SharedPtr<PTControlDirectionExtension>& val)
{
	_extension = val;
}


inline void PTControlDirection::setExtension(Poco::SharedPtr<PTControlDirectionExtension>&& val)
{
	_extension = std::move(val);
}


inline void PTControlDirection::setReverse(const Poco::SharedPtr<Reverse>& val)
{
	_reverse = val;
}


inline void PTControlDirection::setReverse(Poco::SharedPtr<Reverse>&& val)
{
	_reverse = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/EFlip.h"
#include "ONVIF/PTControlDirectionExtension.h"
#include "ONVIF/Reverse.h"


#endif // ONVIF_PTControlDirection_INCLUDED
