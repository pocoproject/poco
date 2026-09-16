// 
// VideoAttributes.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_VideoAttributes_INCLUDED
#define ONVIF_VideoAttributes_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=VideoAttributes
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API VideoAttributes
{
public:
	VideoAttributes();

	VideoAttributes(
		Poco::Optional<int> bitrate, 
		int width, 
		int height, 
		const std::string& encoding, 
		float framerate);

	virtual ~VideoAttributes();

	Poco::Optional<int> getBitrate() const;

	const std::string& getEncoding() const;

	float getFramerate() const;

	int getHeight() const;

	int getWidth() const;

	void setBitrate(Poco::Optional<int> val);

	void setEncoding(const std::string& val);

	void setEncoding(std::string&& val);

	void setFramerate(float val);

	void setHeight(int val);

	void setWidth(int val);

private:
	//@ mandatory=false
	//@ name=Bitrate
	//@ order=0
	Poco::Optional<int> _bitrate;

	//@ name=Width
	//@ order=1
	int _width;

	//@ name=Height
	//@ order=2
	int _height;

	//@ name=Encoding
	//@ order=3
	std::string _encoding;

	//@ name=Framerate
	//@ order=4
	float _framerate;

};


inline Poco::Optional<int> VideoAttributes::getBitrate() const
{
	return _bitrate;
}


inline const std::string& VideoAttributes::getEncoding() const
{
	return _encoding;
}


inline float VideoAttributes::getFramerate() const
{
	return _framerate;
}


inline int VideoAttributes::getHeight() const
{
	return _height;
}


inline int VideoAttributes::getWidth() const
{
	return _width;
}


inline void VideoAttributes::setBitrate(Poco::Optional<int> val)
{
	_bitrate = val;
}


inline void VideoAttributes::setEncoding(const std::string& val)
{
	_encoding = val;
}


inline void VideoAttributes::setEncoding(std::string&& val)
{
	_encoding = std::move(val);
}


inline void VideoAttributes::setFramerate(float val)
{
	_framerate = val;
}


inline void VideoAttributes::setHeight(int val)
{
	_height = val;
}


inline void VideoAttributes::setWidth(int val)
{
	_width = val;
}


} // ONVIF


#endif // ONVIF_VideoAttributes_INCLUDED
