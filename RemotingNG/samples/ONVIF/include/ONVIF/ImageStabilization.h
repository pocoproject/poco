// 
// ImageStabilization.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_ImageStabilization_INCLUDED
#define ONVIF_ImageStabilization_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class ImageStabilizationExtension;
} 


namespace ONVIF {


//@ name=ImageStabilization
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API ImageStabilization
{
public:
	ImageStabilization();

	ImageStabilization(
		const std::string& mode, 
		Poco::Optional<float> level, 
		const Poco::SharedPtr<ImageStabilizationExtension>& extension);

	virtual ~ImageStabilization();

	const Poco::SharedPtr<ImageStabilizationExtension>& getExtension() const;

	Poco::Optional<float> getLevel() const;

	const std::string& getMode() const;

	void setExtension(const Poco::SharedPtr<ImageStabilizationExtension>& val);

	void setExtension(Poco::SharedPtr<ImageStabilizationExtension>&& val);

	void setLevel(Poco::Optional<float> val);

	void setMode(const std::string& val);

	void setMode(std::string&& val);

private:
	//@ name=Mode
	//@ order=0
	std::string _mode;

	//@ mandatory=false
	//@ name=Level
	//@ order=1
	Poco::Optional<float> _level;

	//@ mandatory=false
	//@ name=Extension
	//@ order=2
	Poco::SharedPtr<ImageStabilizationExtension> _extension;

};


inline const Poco::SharedPtr<ImageStabilizationExtension>& ImageStabilization::getExtension() const
{
	return _extension;
}


inline Poco::Optional<float> ImageStabilization::getLevel() const
{
	return _level;
}


inline const std::string& ImageStabilization::getMode() const
{
	return _mode;
}


inline void ImageStabilization::setExtension(const Poco::SharedPtr<ImageStabilizationExtension>& val)
{
	_extension = val;
}


inline void ImageStabilization::setExtension(Poco::SharedPtr<ImageStabilizationExtension>&& val)
{
	_extension = std::move(val);
}


inline void ImageStabilization::setLevel(Poco::Optional<float> val)
{
	_level = val;
}


inline void ImageStabilization::setMode(const std::string& val)
{
	_mode = val;
}


inline void ImageStabilization::setMode(std::string&& val)
{
	_mode = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/ImageStabilizationExtension.h"


#endif // ONVIF_ImageStabilization_INCLUDED
