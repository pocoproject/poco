// 
// ImageStabilizationOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_ImageStabilizationOptions_INCLUDED
#define ONVIF_ImageStabilizationOptions_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class FloatRange;
class ImageStabilizationOptionsExtension;
} 


namespace ONVIF {


//@ name=ImageStabilizationOptions
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API ImageStabilizationOptions
{
public:
	ImageStabilizationOptions();

	ImageStabilizationOptions(
		const std::vector<std::string>& mode, 
		const Poco::SharedPtr<FloatRange>& level, 
		const Poco::SharedPtr<ImageStabilizationOptionsExtension>& extension);

	virtual ~ImageStabilizationOptions();

	const Poco::SharedPtr<ImageStabilizationOptionsExtension>& getExtension() const;

	const Poco::SharedPtr<FloatRange>& getLevel() const;

	const std::vector<std::string>& getMode() const;

	std::vector<std::string>& getMode();

	void setExtension(const Poco::SharedPtr<ImageStabilizationOptionsExtension>& val);

	void setExtension(Poco::SharedPtr<ImageStabilizationOptionsExtension>&& val);

	void setLevel(const Poco::SharedPtr<FloatRange>& val);

	void setLevel(Poco::SharedPtr<FloatRange>&& val);

	void setMode(const std::vector<std::string>& val);

	void setMode(std::vector<std::string>&& val);

private:
	//@ name=Mode
	//@ order=0
	std::vector<std::string> _mode;

	//@ mandatory=false
	//@ name=Level
	//@ order=1
	Poco::SharedPtr<FloatRange> _level;

	//@ mandatory=false
	//@ name=Extension
	//@ order=2
	Poco::SharedPtr<ImageStabilizationOptionsExtension> _extension;

};


inline const Poco::SharedPtr<ImageStabilizationOptionsExtension>& ImageStabilizationOptions::getExtension() const
{
	return _extension;
}


inline const Poco::SharedPtr<FloatRange>& ImageStabilizationOptions::getLevel() const
{
	return _level;
}


inline const std::vector<std::string>& ImageStabilizationOptions::getMode() const
{
	return _mode;
}


inline std::vector<std::string>& ImageStabilizationOptions::getMode()
{
	return _mode;
}


inline void ImageStabilizationOptions::setExtension(const Poco::SharedPtr<ImageStabilizationOptionsExtension>& val)
{
	_extension = val;
}


inline void ImageStabilizationOptions::setExtension(Poco::SharedPtr<ImageStabilizationOptionsExtension>&& val)
{
	_extension = std::move(val);
}


inline void ImageStabilizationOptions::setLevel(const Poco::SharedPtr<FloatRange>& val)
{
	_level = val;
}


inline void ImageStabilizationOptions::setLevel(Poco::SharedPtr<FloatRange>&& val)
{
	_level = std::move(val);
}


inline void ImageStabilizationOptions::setMode(const std::vector<std::string>& val)
{
	_mode = val;
}


inline void ImageStabilizationOptions::setMode(std::vector<std::string>&& val)
{
	_mode = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/FloatRange.h"
#include "ONVIF/ImageStabilizationOptionsExtension.h"


#endif // ONVIF_ImageStabilizationOptions_INCLUDED
