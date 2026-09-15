// 
// OSDColorOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_OSDColorOptions_INCLUDED
#define ONVIF_OSDColorOptions_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class ColorOptions;
class IntRange;
class OSDColorOptionsExtension;
} 


namespace ONVIF {


//@ name=OSDColorOptions
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API OSDColorOptions
{
public:
	OSDColorOptions();

	OSDColorOptions(
		const Poco::SharedPtr<ColorOptions>& color, 
		const Poco::SharedPtr<IntRange>& transparent, 
		const Poco::SharedPtr<OSDColorOptionsExtension>& extension);

	virtual ~OSDColorOptions();

	const Poco::SharedPtr<ColorOptions>& getColor() const;

	const Poco::SharedPtr<OSDColorOptionsExtension>& getExtension() const;

	const Poco::SharedPtr<IntRange>& getTransparent() const;

	void setColor(const Poco::SharedPtr<ColorOptions>& val);

	void setColor(Poco::SharedPtr<ColorOptions>&& val);

	void setExtension(const Poco::SharedPtr<OSDColorOptionsExtension>& val);

	void setExtension(Poco::SharedPtr<OSDColorOptionsExtension>&& val);

	void setTransparent(const Poco::SharedPtr<IntRange>& val);

	void setTransparent(Poco::SharedPtr<IntRange>&& val);

private:
	//@ mandatory=false
	//@ name=Color
	//@ order=0
	Poco::SharedPtr<ColorOptions> _color;

	//@ mandatory=false
	//@ name=Transparent
	//@ order=1
	Poco::SharedPtr<IntRange> _transparent;

	//@ mandatory=false
	//@ name=Extension
	//@ order=2
	Poco::SharedPtr<OSDColorOptionsExtension> _extension;

};


inline const Poco::SharedPtr<ColorOptions>& OSDColorOptions::getColor() const
{
	return _color;
}


inline const Poco::SharedPtr<OSDColorOptionsExtension>& OSDColorOptions::getExtension() const
{
	return _extension;
}


inline const Poco::SharedPtr<IntRange>& OSDColorOptions::getTransparent() const
{
	return _transparent;
}


inline void OSDColorOptions::setColor(const Poco::SharedPtr<ColorOptions>& val)
{
	_color = val;
}


inline void OSDColorOptions::setColor(Poco::SharedPtr<ColorOptions>&& val)
{
	_color = std::move(val);
}


inline void OSDColorOptions::setExtension(const Poco::SharedPtr<OSDColorOptionsExtension>& val)
{
	_extension = val;
}


inline void OSDColorOptions::setExtension(Poco::SharedPtr<OSDColorOptionsExtension>&& val)
{
	_extension = std::move(val);
}


inline void OSDColorOptions::setTransparent(const Poco::SharedPtr<IntRange>& val)
{
	_transparent = val;
}


inline void OSDColorOptions::setTransparent(Poco::SharedPtr<IntRange>&& val)
{
	_transparent = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/ColorOptions.h"
#include "ONVIF/IntRange.h"
#include "ONVIF/OSDColorOptionsExtension.h"


#endif // ONVIF_OSDColorOptions_INCLUDED
