// 
// ColorOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_ColorOptions_INCLUDED
#define ONVIF_ColorOptions_INCLUDED


#include "ONVIF/Color.h"
#include "ONVIF/ColorspaceRange.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=ColorOptions
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API ColorOptions
{
public:
	ColorOptions();

	ColorOptions(
		const std::vector<Color>& colorList, 
		const std::vector<ColorspaceRange>& colorspaceRange);

	virtual ~ColorOptions();

	const std::vector<Color>& getColorList() const;

	std::vector<Color>& getColorList();

	const std::vector<ColorspaceRange>& getColorspaceRange() const;

	std::vector<ColorspaceRange>& getColorspaceRange();

	void setColorList(const std::vector<Color>& val);

	void setColorList(std::vector<Color>&& val);

	void setColorspaceRange(const std::vector<ColorspaceRange>& val);

	void setColorspaceRange(std::vector<ColorspaceRange>&& val);

private:
	//@ mandatory=false
	//@ name=ColorList
	//@ order=0
	std::vector<Color> _colorList;

	//@ mandatory=false
	//@ name=ColorspaceRange
	//@ order=1
	std::vector<ColorspaceRange> _colorspaceRange;

};


inline const std::vector<Color>& ColorOptions::getColorList() const
{
	return _colorList;
}


inline std::vector<Color>& ColorOptions::getColorList()
{
	return _colorList;
}


inline const std::vector<ColorspaceRange>& ColorOptions::getColorspaceRange() const
{
	return _colorspaceRange;
}


inline std::vector<ColorspaceRange>& ColorOptions::getColorspaceRange()
{
	return _colorspaceRange;
}


inline void ColorOptions::setColorList(const std::vector<Color>& val)
{
	_colorList = val;
}


inline void ColorOptions::setColorList(std::vector<Color>&& val)
{
	_colorList = std::move(val);
}


inline void ColorOptions::setColorspaceRange(const std::vector<ColorspaceRange>& val)
{
	_colorspaceRange = val;
}


inline void ColorOptions::setColorspaceRange(std::vector<ColorspaceRange>&& val)
{
	_colorspaceRange = std::move(val);
}


} // ONVIF


#endif // ONVIF_ColorOptions_INCLUDED
