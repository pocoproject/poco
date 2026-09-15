// 
// PaneLayoutOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_PaneLayoutOptions_INCLUDED
#define ONVIF_PaneLayoutOptions_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/Rectangle.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class PaneOptionExtension;
} 


namespace ONVIF {


//@ name=PaneLayoutOptions
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API PaneLayoutOptions
{
public:
	PaneLayoutOptions();

	PaneLayoutOptions(
		const std::vector<Rectangle>& area, 
		const Poco::SharedPtr<PaneOptionExtension>& extension);

	virtual ~PaneLayoutOptions();

	const std::vector<Rectangle>& getArea() const;

	std::vector<Rectangle>& getArea();

	const Poco::SharedPtr<PaneOptionExtension>& getExtension() const;

	void setArea(const std::vector<Rectangle>& val);

	void setArea(std::vector<Rectangle>&& val);

	void setExtension(const Poco::SharedPtr<PaneOptionExtension>& val);

	void setExtension(Poco::SharedPtr<PaneOptionExtension>&& val);

private:
	//@ name=Area
	//@ order=0
	std::vector<Rectangle> _area;

	//@ mandatory=false
	//@ name=Extension
	//@ order=1
	Poco::SharedPtr<PaneOptionExtension> _extension;

};


inline const std::vector<Rectangle>& PaneLayoutOptions::getArea() const
{
	return _area;
}


inline std::vector<Rectangle>& PaneLayoutOptions::getArea()
{
	return _area;
}


inline const Poco::SharedPtr<PaneOptionExtension>& PaneLayoutOptions::getExtension() const
{
	return _extension;
}


inline void PaneLayoutOptions::setArea(const std::vector<Rectangle>& val)
{
	_area = val;
}


inline void PaneLayoutOptions::setArea(std::vector<Rectangle>&& val)
{
	_area = std::move(val);
}


inline void PaneLayoutOptions::setExtension(const Poco::SharedPtr<PaneOptionExtension>& val)
{
	_extension = val;
}


inline void PaneLayoutOptions::setExtension(Poco::SharedPtr<PaneOptionExtension>&& val)
{
	_extension = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/PaneOptionExtension.h"


#endif // ONVIF_PaneLayoutOptions_INCLUDED
