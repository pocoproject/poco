// 
// PolygonOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_PolygonOptions_INCLUDED
#define ONVIF_PolygonOptions_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class IntRange;
} 


namespace ONVIF {


//@ name=PolygonOptions
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API PolygonOptions
{
public:
	PolygonOptions();

	PolygonOptions(
		Poco::Optional<bool> rectangleOnly, 
		const Poco::SharedPtr<IntRange>& vertexLimits);

	virtual ~PolygonOptions();

	Poco::Optional<bool> getRectangleOnly() const;

	const Poco::SharedPtr<IntRange>& getVertexLimits() const;

	void setRectangleOnly(Poco::Optional<bool> val);

	void setVertexLimits(const Poco::SharedPtr<IntRange>& val);

	void setVertexLimits(Poco::SharedPtr<IntRange>&& val);

private:
	//@ mandatory=false
	//@ name=RectangleOnly
	//@ order=0
	Poco::Optional<bool> _rectangleOnly;

	//@ mandatory=false
	//@ name=VertexLimits
	//@ order=1
	Poco::SharedPtr<IntRange> _vertexLimits;

};


inline Poco::Optional<bool> PolygonOptions::getRectangleOnly() const
{
	return _rectangleOnly;
}


inline const Poco::SharedPtr<IntRange>& PolygonOptions::getVertexLimits() const
{
	return _vertexLimits;
}


inline void PolygonOptions::setRectangleOnly(Poco::Optional<bool> val)
{
	_rectangleOnly = val;
}


inline void PolygonOptions::setVertexLimits(const Poco::SharedPtr<IntRange>& val)
{
	_vertexLimits = val;
}


inline void PolygonOptions::setVertexLimits(Poco::SharedPtr<IntRange>&& val)
{
	_vertexLimits = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/IntRange.h"


#endif // ONVIF_PolygonOptions_INCLUDED
