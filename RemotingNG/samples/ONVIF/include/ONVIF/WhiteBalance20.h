// 
// WhiteBalance20.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_WhiteBalance20_INCLUDED
#define ONVIF_WhiteBalance20_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class WhiteBalance20Extension;
} 


namespace ONVIF {


//@ name=WhiteBalance20
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API WhiteBalance20
{
public:
	WhiteBalance20();

	WhiteBalance20(
		const std::string& mode, 
		Poco::Optional<float> crGain, 
		Poco::Optional<float> cbGain, 
		const Poco::SharedPtr<WhiteBalance20Extension>& extension);

	virtual ~WhiteBalance20();

	Poco::Optional<float> getCbGain() const;

	Poco::Optional<float> getCrGain() const;

	const Poco::SharedPtr<WhiteBalance20Extension>& getExtension() const;

	const std::string& getMode() const;

	void setCbGain(Poco::Optional<float> val);

	void setCrGain(Poco::Optional<float> val);

	void setExtension(const Poco::SharedPtr<WhiteBalance20Extension>& val);

	void setExtension(Poco::SharedPtr<WhiteBalance20Extension>&& val);

	void setMode(const std::string& val);

	void setMode(std::string&& val);

private:
	//@ name=Mode
	//@ order=0
	std::string _mode;

	//@ mandatory=false
	//@ name=CrGain
	//@ order=1
	Poco::Optional<float> _crGain;

	//@ mandatory=false
	//@ name=CbGain
	//@ order=2
	Poco::Optional<float> _cbGain;

	//@ mandatory=false
	//@ name=Extension
	//@ order=3
	Poco::SharedPtr<WhiteBalance20Extension> _extension;

};


inline Poco::Optional<float> WhiteBalance20::getCbGain() const
{
	return _cbGain;
}


inline Poco::Optional<float> WhiteBalance20::getCrGain() const
{
	return _crGain;
}


inline const Poco::SharedPtr<WhiteBalance20Extension>& WhiteBalance20::getExtension() const
{
	return _extension;
}


inline const std::string& WhiteBalance20::getMode() const
{
	return _mode;
}


inline void WhiteBalance20::setCbGain(Poco::Optional<float> val)
{
	_cbGain = val;
}


inline void WhiteBalance20::setCrGain(Poco::Optional<float> val)
{
	_crGain = val;
}


inline void WhiteBalance20::setExtension(const Poco::SharedPtr<WhiteBalance20Extension>& val)
{
	_extension = val;
}


inline void WhiteBalance20::setExtension(Poco::SharedPtr<WhiteBalance20Extension>&& val)
{
	_extension = std::move(val);
}


inline void WhiteBalance20::setMode(const std::string& val)
{
	_mode = val;
}


inline void WhiteBalance20::setMode(std::string&& val)
{
	_mode = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/WhiteBalance20Extension.h"


#endif // ONVIF_WhiteBalance20_INCLUDED
