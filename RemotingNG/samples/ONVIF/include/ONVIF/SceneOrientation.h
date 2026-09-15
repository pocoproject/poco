// 
// SceneOrientation.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_SceneOrientation_INCLUDED
#define ONVIF_SceneOrientation_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=SceneOrientation
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API SceneOrientation
{
public:
	SceneOrientation();

	SceneOrientation(
		const std::string& mode, 
		const Poco::Optional<std::string>& orientation);

	virtual ~SceneOrientation();

	const std::string& getMode() const;

	const Poco::Optional<std::string>& getOrientation() const;

	void setMode(const std::string& val);

	void setMode(std::string&& val);

	void setOrientation(const Poco::Optional<std::string>& val);

	void setOrientation(Poco::Optional<std::string>&& val);

private:
	//@ name=Mode
	//@ order=0
	std::string _mode;

	//@ mandatory=false
	//@ name=Orientation
	//@ order=1
	Poco::Optional<std::string> _orientation;

};


inline const std::string& SceneOrientation::getMode() const
{
	return _mode;
}


inline const Poco::Optional<std::string>& SceneOrientation::getOrientation() const
{
	return _orientation;
}


inline void SceneOrientation::setMode(const std::string& val)
{
	_mode = val;
}


inline void SceneOrientation::setMode(std::string&& val)
{
	_mode = std::move(val);
}


inline void SceneOrientation::setOrientation(const Poco::Optional<std::string>& val)
{
	_orientation = val;
}


inline void SceneOrientation::setOrientation(Poco::Optional<std::string>&& val)
{
	_orientation = std::move(val);
}


} // ONVIF


#endif // ONVIF_SceneOrientation_INCLUDED
