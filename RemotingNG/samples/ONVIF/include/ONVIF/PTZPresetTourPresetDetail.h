// 
// PTZPresetTourPresetDetail.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_PTZPresetTourPresetDetail_INCLUDED
#define ONVIF_PTZPresetTourPresetDetail_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class PTZPresetTourTypeExtension;
class PTZVector;
} 


namespace ONVIF {


//@ name=PTZPresetTourPresetDetail
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API PTZPresetTourPresetDetail
{
public:
	PTZPresetTourPresetDetail();

	PTZPresetTourPresetDetail(
		const Poco::Optional<std::string>& presetToken, 
		Poco::Optional<bool> home, 
		const Poco::SharedPtr<PTZVector>& pTZPosition, 
		const Poco::SharedPtr<PTZPresetTourTypeExtension>& typeExtension);

	virtual ~PTZPresetTourPresetDetail();

	Poco::Optional<bool> getHome() const;

	const Poco::SharedPtr<PTZVector>& getPTZPosition() const;

	const Poco::Optional<std::string>& getPresetToken() const;

	const Poco::SharedPtr<PTZPresetTourTypeExtension>& getTypeExtension() const;

	void setHome(Poco::Optional<bool> val);

	void setPTZPosition(const Poco::SharedPtr<PTZVector>& val);

	void setPTZPosition(Poco::SharedPtr<PTZVector>&& val);

	void setPresetToken(const Poco::Optional<std::string>& val);

	void setPresetToken(Poco::Optional<std::string>&& val);

	void setTypeExtension(const Poco::SharedPtr<PTZPresetTourTypeExtension>& val);

	void setTypeExtension(Poco::SharedPtr<PTZPresetTourTypeExtension>&& val);

private:
	//@ mandatory=false
	//@ name=PresetToken
	//@ order=0
	Poco::Optional<std::string> _presetToken;

	//@ mandatory=false
	//@ name=Home
	//@ order=1
	Poco::Optional<bool> _home;

	//@ mandatory=false
	//@ name=PTZPosition
	//@ order=2
	Poco::SharedPtr<PTZVector> _pTZPosition;

	//@ mandatory=false
	//@ name=TypeExtension
	//@ order=3
	Poco::SharedPtr<PTZPresetTourTypeExtension> _typeExtension;

};


inline Poco::Optional<bool> PTZPresetTourPresetDetail::getHome() const
{
	return _home;
}


inline const Poco::SharedPtr<PTZVector>& PTZPresetTourPresetDetail::getPTZPosition() const
{
	return _pTZPosition;
}


inline const Poco::Optional<std::string>& PTZPresetTourPresetDetail::getPresetToken() const
{
	return _presetToken;
}


inline const Poco::SharedPtr<PTZPresetTourTypeExtension>& PTZPresetTourPresetDetail::getTypeExtension() const
{
	return _typeExtension;
}


inline void PTZPresetTourPresetDetail::setHome(Poco::Optional<bool> val)
{
	_home = val;
}


inline void PTZPresetTourPresetDetail::setPTZPosition(const Poco::SharedPtr<PTZVector>& val)
{
	_pTZPosition = val;
}


inline void PTZPresetTourPresetDetail::setPTZPosition(Poco::SharedPtr<PTZVector>&& val)
{
	_pTZPosition = std::move(val);
}


inline void PTZPresetTourPresetDetail::setPresetToken(const Poco::Optional<std::string>& val)
{
	_presetToken = val;
}


inline void PTZPresetTourPresetDetail::setPresetToken(Poco::Optional<std::string>&& val)
{
	_presetToken = std::move(val);
}


inline void PTZPresetTourPresetDetail::setTypeExtension(const Poco::SharedPtr<PTZPresetTourTypeExtension>& val)
{
	_typeExtension = val;
}


inline void PTZPresetTourPresetDetail::setTypeExtension(Poco::SharedPtr<PTZPresetTourTypeExtension>&& val)
{
	_typeExtension = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/PTZPresetTourTypeExtension.h"
#include "ONVIF/PTZVector.h"


#endif // ONVIF_PTZPresetTourPresetDetail_INCLUDED
