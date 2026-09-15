// 
// PaneLayout.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_PaneLayout_INCLUDED
#define ONVIF_PaneLayout_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/Rectangle.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=PaneLayout
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API PaneLayout
{
public:
	PaneLayout();

	PaneLayout(
		const std::string& pane, 
		const Rectangle& area);

	virtual ~PaneLayout();

	const Rectangle& getArea() const;

	const std::string& getPane() const;

	void setArea(const Rectangle& val);

	void setArea(Rectangle&& val);

	void setPane(const std::string& val);

	void setPane(std::string&& val);

private:
	//@ name=Pane
	//@ order=0
	std::string _pane;

	//@ name=Area
	//@ order=1
	Rectangle _area;

};


inline const Rectangle& PaneLayout::getArea() const
{
	return _area;
}


inline const std::string& PaneLayout::getPane() const
{
	return _pane;
}


inline void PaneLayout::setArea(const Rectangle& val)
{
	_area = val;
}


inline void PaneLayout::setArea(Rectangle&& val)
{
	_area = std::move(val);
}


inline void PaneLayout::setPane(const std::string& val)
{
	_pane = val;
}


inline void PaneLayout::setPane(std::string&& val)
{
	_pane = std::move(val);
}


} // ONVIF


#endif // ONVIF_PaneLayout_INCLUDED
