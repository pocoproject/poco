// 
// OSDImgOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_OSDImgOptions_INCLUDED
#define ONVIF_OSDImgOptions_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/URI.h"
#include <string>
#include <vector>


namespace ONVIF {
class OSDImgOptionsExtension;
} 


namespace ONVIF {


//@ name=OSDImgOptions
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API OSDImgOptions
{
public:
	OSDImgOptions();

	OSDImgOptions(
		const std::vector<std::vector<std::string>>& formatsSupported, 
		Poco::Optional<int> maxSize, 
		Poco::Optional<int> maxWidth, 
		Poco::Optional<int> maxHeight, 
		const std::vector<Poco::URI>& imagePath, 
		const Poco::SharedPtr<OSDImgOptionsExtension>& extension);

	virtual ~OSDImgOptions();

	const Poco::SharedPtr<OSDImgOptionsExtension>& getExtension() const;

	const std::vector<std::vector<std::string>>& getFormatsSupported() const;

	std::vector<std::vector<std::string>>& getFormatsSupported();

	const std::vector<Poco::URI>& getImagePath() const;

	std::vector<Poco::URI>& getImagePath();

	Poco::Optional<int> getMaxHeight() const;

	Poco::Optional<int> getMaxSize() const;

	Poco::Optional<int> getMaxWidth() const;

	void setExtension(const Poco::SharedPtr<OSDImgOptionsExtension>& val);

	void setExtension(Poco::SharedPtr<OSDImgOptionsExtension>&& val);

	void setFormatsSupported(const std::vector<std::vector<std::string>>& val);

	void setFormatsSupported(std::vector<std::vector<std::string>>&& val);

	void setImagePath(const std::vector<Poco::URI>& val);

	void setImagePath(std::vector<Poco::URI>&& val);

	void setMaxHeight(Poco::Optional<int> val);

	void setMaxSize(Poco::Optional<int> val);

	void setMaxWidth(Poco::Optional<int> val);

private:
	//@ mandatory=false
	//@ name=FormatsSupported
	//@ order=0
	//@ type=attr
	std::vector<std::vector<std::string>> _formatsSupported;

	//@ mandatory=false
	//@ name=MaxSize
	//@ order=1
	//@ type=attr
	Poco::Optional<int> _maxSize;

	//@ mandatory=false
	//@ name=MaxWidth
	//@ order=2
	//@ type=attr
	Poco::Optional<int> _maxWidth;

	//@ mandatory=false
	//@ name=MaxHeight
	//@ order=3
	//@ type=attr
	Poco::Optional<int> _maxHeight;

	//@ name=ImagePath
	//@ order=4
	std::vector<Poco::URI> _imagePath;

	//@ mandatory=false
	//@ name=Extension
	//@ order=5
	Poco::SharedPtr<OSDImgOptionsExtension> _extension;

};


inline const Poco::SharedPtr<OSDImgOptionsExtension>& OSDImgOptions::getExtension() const
{
	return _extension;
}


inline const std::vector<std::vector<std::string>>& OSDImgOptions::getFormatsSupported() const
{
	return _formatsSupported;
}


inline std::vector<std::vector<std::string>>& OSDImgOptions::getFormatsSupported()
{
	return _formatsSupported;
}


inline const std::vector<Poco::URI>& OSDImgOptions::getImagePath() const
{
	return _imagePath;
}


inline std::vector<Poco::URI>& OSDImgOptions::getImagePath()
{
	return _imagePath;
}


inline Poco::Optional<int> OSDImgOptions::getMaxHeight() const
{
	return _maxHeight;
}


inline Poco::Optional<int> OSDImgOptions::getMaxSize() const
{
	return _maxSize;
}


inline Poco::Optional<int> OSDImgOptions::getMaxWidth() const
{
	return _maxWidth;
}


inline void OSDImgOptions::setExtension(const Poco::SharedPtr<OSDImgOptionsExtension>& val)
{
	_extension = val;
}


inline void OSDImgOptions::setExtension(Poco::SharedPtr<OSDImgOptionsExtension>&& val)
{
	_extension = std::move(val);
}


inline void OSDImgOptions::setFormatsSupported(const std::vector<std::vector<std::string>>& val)
{
	_formatsSupported = val;
}


inline void OSDImgOptions::setFormatsSupported(std::vector<std::vector<std::string>>&& val)
{
	_formatsSupported = std::move(val);
}


inline void OSDImgOptions::setImagePath(const std::vector<Poco::URI>& val)
{
	_imagePath = val;
}


inline void OSDImgOptions::setImagePath(std::vector<Poco::URI>&& val)
{
	_imagePath = std::move(val);
}


inline void OSDImgOptions::setMaxHeight(Poco::Optional<int> val)
{
	_maxHeight = val;
}


inline void OSDImgOptions::setMaxSize(Poco::Optional<int> val)
{
	_maxSize = val;
}


inline void OSDImgOptions::setMaxWidth(Poco::Optional<int> val)
{
	_maxWidth = val;
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/OSDImgOptionsExtension.h"


#endif // ONVIF_OSDImgOptions_INCLUDED
