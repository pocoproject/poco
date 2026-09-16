// 
// ColorDescriptor.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_ColorDescriptor_INCLUDED
#define ONVIF_ColorDescriptor_INCLUDED


#include "ONVIF/ColorCluster.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=ColorDescriptor
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API ColorDescriptor
{
public:
	ColorDescriptor();

	ColorDescriptor(
		const std::vector<ColorCluster>& colorCluster, 
		const Poco::Optional<std::string>& extension);

	virtual ~ColorDescriptor();

	const std::vector<ColorCluster>& getColorCluster() const;

	std::vector<ColorCluster>& getColorCluster();

	const Poco::Optional<std::string>& getExtension() const;

	void setColorCluster(const std::vector<ColorCluster>& val);

	void setColorCluster(std::vector<ColorCluster>&& val);

	void setExtension(const Poco::Optional<std::string>& val);

	void setExtension(Poco::Optional<std::string>&& val);

private:
	//@ mandatory=false
	//@ name=ColorCluster
	//@ order=0
	std::vector<ColorCluster> _colorCluster;

	//@ mandatory=false
	//@ name=Extension
	//@ order=1
	Poco::Optional<std::string> _extension;

};


inline const std::vector<ColorCluster>& ColorDescriptor::getColorCluster() const
{
	return _colorCluster;
}


inline std::vector<ColorCluster>& ColorDescriptor::getColorCluster()
{
	return _colorCluster;
}


inline const Poco::Optional<std::string>& ColorDescriptor::getExtension() const
{
	return _extension;
}


inline void ColorDescriptor::setColorCluster(const std::vector<ColorCluster>& val)
{
	_colorCluster = val;
}


inline void ColorDescriptor::setColorCluster(std::vector<ColorCluster>&& val)
{
	_colorCluster = std::move(val);
}


inline void ColorDescriptor::setExtension(const Poco::Optional<std::string>& val)
{
	_extension = val;
}


inline void ColorDescriptor::setExtension(Poco::Optional<std::string>&& val)
{
	_extension = std::move(val);
}


} // ONVIF


#endif // ONVIF_ColorDescriptor_INCLUDED
