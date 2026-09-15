// 
// Layout.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Layout_INCLUDED
#define ONVIF_Layout_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/PaneLayout.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class LayoutExtension;
} 


namespace ONVIF {


//@ name=Layout
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API Layout
{
public:
	Layout();

	Layout(
		const std::vector<PaneLayout>& paneLayout, 
		const Poco::SharedPtr<LayoutExtension>& extension);

	virtual ~Layout();

	const Poco::SharedPtr<LayoutExtension>& getExtension() const;

	const std::vector<PaneLayout>& getPaneLayout() const;

	std::vector<PaneLayout>& getPaneLayout();

	void setExtension(const Poco::SharedPtr<LayoutExtension>& val);

	void setExtension(Poco::SharedPtr<LayoutExtension>&& val);

	void setPaneLayout(const std::vector<PaneLayout>& val);

	void setPaneLayout(std::vector<PaneLayout>&& val);

private:
	//@ name=PaneLayout
	//@ order=0
	std::vector<PaneLayout> _paneLayout;

	//@ mandatory=false
	//@ name=Extension
	//@ order=1
	Poco::SharedPtr<LayoutExtension> _extension;

};


inline const Poco::SharedPtr<LayoutExtension>& Layout::getExtension() const
{
	return _extension;
}


inline const std::vector<PaneLayout>& Layout::getPaneLayout() const
{
	return _paneLayout;
}


inline std::vector<PaneLayout>& Layout::getPaneLayout()
{
	return _paneLayout;
}


inline void Layout::setExtension(const Poco::SharedPtr<LayoutExtension>& val)
{
	_extension = val;
}


inline void Layout::setExtension(Poco::SharedPtr<LayoutExtension>&& val)
{
	_extension = std::move(val);
}


inline void Layout::setPaneLayout(const std::vector<PaneLayout>& val)
{
	_paneLayout = val;
}


inline void Layout::setPaneLayout(std::vector<PaneLayout>&& val)
{
	_paneLayout = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/LayoutExtension.h"


#endif // ONVIF_Layout_INCLUDED
