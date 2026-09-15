// 
// OSDColor.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_OSDColor_INCLUDED
#define ONVIF_OSDColor_INCLUDED


#include "ONVIF/Color.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=OSDColor
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API OSDColor
{
public:
	OSDColor();

	OSDColor(
		Poco::Optional<int> transparent, 
		const Color& color);

	virtual ~OSDColor();

	const Color& getColor() const;

	Poco::Optional<int> getTransparent() const;

	void setColor(const Color& val);

	void setColor(Color&& val);

	void setTransparent(Poco::Optional<int> val);

private:
	//@ mandatory=false
	//@ name=Transparent
	//@ order=0
	//@ type=attr
	Poco::Optional<int> _transparent;

	//@ name=Color
	//@ order=1
	Color _color;

};


inline const Color& OSDColor::getColor() const
{
	return _color;
}


inline Poco::Optional<int> OSDColor::getTransparent() const
{
	return _transparent;
}


inline void OSDColor::setColor(const Color& val)
{
	_color = val;
}


inline void OSDColor::setColor(Color&& val)
{
	_color = std::move(val);
}


inline void OSDColor::setTransparent(Poco::Optional<int> val)
{
	_transparent = val;
}


} // ONVIF


#endif // ONVIF_OSDColor_INCLUDED
