// 
// VideoResolution.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_VideoResolution_INCLUDED
#define ONVIF_VideoResolution_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=VideoResolution
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API VideoResolution
{
public:
	VideoResolution();

	VideoResolution(
		int width, 
		int height);

	virtual ~VideoResolution();

	int getHeight() const;

	int getWidth() const;

	void setHeight(int val);

	void setWidth(int val);

private:
	//@ name=Width
	//@ order=0
	int _width;

	//@ name=Height
	//@ order=1
	int _height;

};


inline int VideoResolution::getHeight() const
{
	return _height;
}


inline int VideoResolution::getWidth() const
{
	return _width;
}


inline void VideoResolution::setHeight(int val)
{
	_height = val;
}


inline void VideoResolution::setWidth(int val)
{
	_width = val;
}


} // ONVIF


#endif // ONVIF_VideoResolution_INCLUDED
