// 
// Polygon.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Polygon_INCLUDED
#define ONVIF_Polygon_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/Vector.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=Polygon
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API Polygon
{
public:
	Polygon();

	Polygon(const std::vector<Vector>& point);

	virtual ~Polygon();

	const std::vector<Vector>& getPoint() const;

	std::vector<Vector>& getPoint();

	void setPoint(const std::vector<Vector>& val);

	void setPoint(std::vector<Vector>&& val);

private:
	//@ name=Point
	//@ order=0
	std::vector<Vector> _point;

};


inline const std::vector<Vector>& Polygon::getPoint() const
{
	return _point;
}


inline std::vector<Vector>& Polygon::getPoint()
{
	return _point;
}


inline void Polygon::setPoint(const std::vector<Vector>& val)
{
	_point = val;
}


inline void Polygon::setPoint(std::vector<Vector>&& val)
{
	_point = std::move(val);
}


} // ONVIF


#endif // ONVIF_Polygon_INCLUDED
