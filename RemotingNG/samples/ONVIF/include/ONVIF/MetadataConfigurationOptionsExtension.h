// 
// MetadataConfigurationOptionsExtension.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_MetadataConfigurationOptionsExtension_INCLUDED
#define ONVIF_MetadataConfigurationOptionsExtension_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class MetadataConfigurationOptionsExtension2;
} 


namespace ONVIF {


//@ name=MetadataConfigurationOptionsExtension
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API MetadataConfigurationOptionsExtension
{
public:
	MetadataConfigurationOptionsExtension();

	MetadataConfigurationOptionsExtension(
		const std::vector<std::string>& compressionType, 
		const Poco::SharedPtr<MetadataConfigurationOptionsExtension2>& extension);

	virtual ~MetadataConfigurationOptionsExtension();

	const std::vector<std::string>& getCompressionType() const;

	std::vector<std::string>& getCompressionType();

	const Poco::SharedPtr<MetadataConfigurationOptionsExtension2>& getExtension() const;

	void setCompressionType(const std::vector<std::string>& val);

	void setCompressionType(std::vector<std::string>&& val);

	void setExtension(const Poco::SharedPtr<MetadataConfigurationOptionsExtension2>& val);

	void setExtension(Poco::SharedPtr<MetadataConfigurationOptionsExtension2>&& val);

private:
	//@ mandatory=false
	//@ name=CompressionType
	//@ order=0
	std::vector<std::string> _compressionType;

	//@ mandatory=false
	//@ name=Extension
	//@ order=1
	Poco::SharedPtr<MetadataConfigurationOptionsExtension2> _extension;

};


inline const std::vector<std::string>& MetadataConfigurationOptionsExtension::getCompressionType() const
{
	return _compressionType;
}


inline std::vector<std::string>& MetadataConfigurationOptionsExtension::getCompressionType()
{
	return _compressionType;
}


inline const Poco::SharedPtr<MetadataConfigurationOptionsExtension2>& MetadataConfigurationOptionsExtension::getExtension() const
{
	return _extension;
}


inline void MetadataConfigurationOptionsExtension::setCompressionType(const std::vector<std::string>& val)
{
	_compressionType = val;
}


inline void MetadataConfigurationOptionsExtension::setCompressionType(std::vector<std::string>&& val)
{
	_compressionType = std::move(val);
}


inline void MetadataConfigurationOptionsExtension::setExtension(const Poco::SharedPtr<MetadataConfigurationOptionsExtension2>& val)
{
	_extension = val;
}


inline void MetadataConfigurationOptionsExtension::setExtension(Poco::SharedPtr<MetadataConfigurationOptionsExtension2>&& val)
{
	_extension = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/MetadataConfigurationOptionsExtension2.h"


#endif // ONVIF_MetadataConfigurationOptionsExtension_INCLUDED
