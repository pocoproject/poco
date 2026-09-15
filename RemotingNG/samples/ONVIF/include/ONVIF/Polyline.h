// 
// Polyline.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Polyline_INCLUDED
#define ONVIF_Polyline_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/Vector.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=Polyline
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API Polyline
{
public:
	Polyline();

	Polyline(const std::vector<Vector>& point);

	virtual ~Polyline();

	const std::vector<Vector>& getPoint() const;

	std::vector<Vector>& getPoint();

	void setPoint(const std::vector<Vector>& val);

	void setPoint(std::vector<Vector>&& val);

private:
	//@ name=Point
	//@ order=0
	std::vector<Vector> _point;

};


inline const std::vector<Vector>& Polyline::getPoint() const
{
	return _point;
}


inline std::vector<Vector>& Polyline::getPoint()
{
	return _point;
}


inline void Polyline::setPoint(const std::vector<Vector>& val)
{
	_point = val;
}


inline void Polyline::setPoint(std::vector<Vector>&& val)
{
	_point = std::move(val);
}


} // ONVIF


#endif // ONVIF_Polyline_INCLUDED
