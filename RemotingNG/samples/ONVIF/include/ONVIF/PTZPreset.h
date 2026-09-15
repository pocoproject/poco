// 
// PTZPreset.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_PTZPreset_INCLUDED
#define ONVIF_PTZPreset_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class PTZVector;
} 


namespace ONVIF {


//@ name=PTZPreset
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API PTZPreset
{
public:
	PTZPreset();

	PTZPreset(
		const Poco::Optional<std::string>& token, 
		const Poco::Optional<std::string>& name, 
		const Poco::SharedPtr<PTZVector>& pTZPosition);

	virtual ~PTZPreset();

	const Poco::Optional<std::string>& getName() const;

	const Poco::SharedPtr<PTZVector>& getPTZPosition() const;

	const Poco::Optional<std::string>& getToken() const;

	void setName(const Poco::Optional<std::string>& val);

	void setName(Poco::Optional<std::string>&& val);

	void setPTZPosition(const Poco::SharedPtr<PTZVector>& val);

	void setPTZPosition(Poco::SharedPtr<PTZVector>&& val);

	void setToken(const Poco::Optional<std::string>& val);

	void setToken(Poco::Optional<std::string>&& val);

private:
	//@ mandatory=false
	//@ name=token
	//@ order=0
	//@ type=attr
	Poco::Optional<std::string> _token;

	//@ mandatory=false
	//@ name=Name
	//@ order=1
	Poco::Optional<std::string> _name;

	//@ mandatory=false
	//@ name=PTZPosition
	//@ order=2
	Poco::SharedPtr<PTZVector> _pTZPosition;

};


inline const Poco::Optional<std::string>& PTZPreset::getName() const
{
	return _name;
}


inline const Poco::SharedPtr<PTZVector>& PTZPreset::getPTZPosition() const
{
	return _pTZPosition;
}


inline const Poco::Optional<std::string>& PTZPreset::getToken() const
{
	return _token;
}


inline void PTZPreset::setName(const Poco::Optional<std::string>& val)
{
	_name = val;
}


inline void PTZPreset::setName(Poco::Optional<std::string>&& val)
{
	_name = std::move(val);
}


inline void PTZPreset::setPTZPosition(const Poco::SharedPtr<PTZVector>& val)
{
	_pTZPosition = val;
}


inline void PTZPreset::setPTZPosition(Poco::SharedPtr<PTZVector>&& val)
{
	_pTZPosition = std::move(val);
}


inline void PTZPreset::setToken(const Poco::Optional<std::string>& val)
{
	_token = val;
}


inline void PTZPreset::setToken(Poco::Optional<std::string>&& val)
{
	_token = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/PTZVector.h"


#endif // ONVIF_PTZPreset_INCLUDED
