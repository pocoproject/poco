// 
// Rotate.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Rotate_INCLUDED
#define ONVIF_Rotate_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class RotateExtension;
} 


namespace ONVIF {


//@ name=Rotate
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API Rotate
{
public:
	Rotate();

	Rotate(
		const std::string& mode, 
		Poco::Optional<int> degree, 
		const Poco::SharedPtr<RotateExtension>& extension);

	virtual ~Rotate();

	Poco::Optional<int> getDegree() const;

	const Poco::SharedPtr<RotateExtension>& getExtension() const;

	const std::string& getMode() const;

	void setDegree(Poco::Optional<int> val);

	void setExtension(const Poco::SharedPtr<RotateExtension>& val);

	void setExtension(Poco::SharedPtr<RotateExtension>&& val);

	void setMode(const std::string& val);

	void setMode(std::string&& val);

private:
	//@ name=Mode
	//@ order=0
	std::string _mode;

	//@ mandatory=false
	//@ name=Degree
	//@ order=1
	Poco::Optional<int> _degree;

	//@ mandatory=false
	//@ name=Extension
	//@ order=2
	Poco::SharedPtr<RotateExtension> _extension;

};


inline Poco::Optional<int> Rotate::getDegree() const
{
	return _degree;
}


inline const Poco::SharedPtr<RotateExtension>& Rotate::getExtension() const
{
	return _extension;
}


inline const std::string& Rotate::getMode() const
{
	return _mode;
}


inline void Rotate::setDegree(Poco::Optional<int> val)
{
	_degree = val;
}


inline void Rotate::setExtension(const Poco::SharedPtr<RotateExtension>& val)
{
	_extension = val;
}


inline void Rotate::setExtension(Poco::SharedPtr<RotateExtension>&& val)
{
	_extension = std::move(val);
}


inline void Rotate::setMode(const std::string& val)
{
	_mode = val;
}


inline void Rotate::setMode(std::string&& val)
{
	_mode = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/RotateExtension.h"


#endif // ONVIF_Rotate_INCLUDED
