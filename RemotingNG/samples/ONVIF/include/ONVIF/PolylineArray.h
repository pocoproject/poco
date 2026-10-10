// 
// PolylineArray.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_PolylineArray_INCLUDED
#define ONVIF_PolylineArray_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/Polyline.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class PolylineArrayExtension;
} 


namespace ONVIF {


//@ name=PolylineArray
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API PolylineArray
{
public:
	PolylineArray();

	PolylineArray(
		const std::vector<Polyline>& segment, 
		const Poco::SharedPtr<PolylineArrayExtension>& extension);

	virtual ~PolylineArray();

	const Poco::SharedPtr<PolylineArrayExtension>& getExtension() const;

	const std::vector<Polyline>& getSegment() const;

	std::vector<Polyline>& getSegment();

	void setExtension(const Poco::SharedPtr<PolylineArrayExtension>& val);

	void setExtension(Poco::SharedPtr<PolylineArrayExtension>&& val);

	void setSegment(const std::vector<Polyline>& val);

	void setSegment(std::vector<Polyline>&& val);

private:
	//@ name=Segment
	//@ order=0
	std::vector<Polyline> _segment;

	//@ mandatory=false
	//@ name=Extension
	//@ order=1
	Poco::SharedPtr<PolylineArrayExtension> _extension;

};


inline const Poco::SharedPtr<PolylineArrayExtension>& PolylineArray::getExtension() const
{
	return _extension;
}


inline const std::vector<Polyline>& PolylineArray::getSegment() const
{
	return _segment;
}


inline std::vector<Polyline>& PolylineArray::getSegment()
{
	return _segment;
}


inline void PolylineArray::setExtension(const Poco::SharedPtr<PolylineArrayExtension>& val)
{
	_extension = val;
}


inline void PolylineArray::setExtension(Poco::SharedPtr<PolylineArrayExtension>&& val)
{
	_extension = std::move(val);
}


inline void PolylineArray::setSegment(const std::vector<Polyline>& val)
{
	_segment = val;
}


inline void PolylineArray::setSegment(std::vector<Polyline>&& val)
{
	_segment = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/PolylineArrayExtension.h"


#endif // ONVIF_PolylineArray_INCLUDED
