// 
// StorageReferencePath.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_StorageReferencePath_INCLUDED
#define ONVIF_StorageReferencePath_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class StorageReferencePathExtension;
} 


namespace ONVIF {


//@ name=StorageReferencePath
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API StorageReferencePath
{
public:
	StorageReferencePath();

	StorageReferencePath(
		const std::string& storageToken, 
		const Poco::Optional<std::string>& relativePath, 
		const Poco::SharedPtr<StorageReferencePathExtension>& extension);

	virtual ~StorageReferencePath();

	const Poco::SharedPtr<StorageReferencePathExtension>& getExtension() const;

	const Poco::Optional<std::string>& getRelativePath() const;

	const std::string& getStorageToken() const;

	void setExtension(const Poco::SharedPtr<StorageReferencePathExtension>& val);

	void setExtension(Poco::SharedPtr<StorageReferencePathExtension>&& val);

	void setRelativePath(const Poco::Optional<std::string>& val);

	void setRelativePath(Poco::Optional<std::string>&& val);

	void setStorageToken(const std::string& val);

	void setStorageToken(std::string&& val);

private:
	//@ name=StorageToken
	//@ order=0
	std::string _storageToken;

	//@ mandatory=false
	//@ name=RelativePath
	//@ order=1
	Poco::Optional<std::string> _relativePath;

	//@ mandatory=false
	//@ name=Extension
	//@ order=2
	Poco::SharedPtr<StorageReferencePathExtension> _extension;

};


inline const Poco::SharedPtr<StorageReferencePathExtension>& StorageReferencePath::getExtension() const
{
	return _extension;
}


inline const Poco::Optional<std::string>& StorageReferencePath::getRelativePath() const
{
	return _relativePath;
}


inline const std::string& StorageReferencePath::getStorageToken() const
{
	return _storageToken;
}


inline void StorageReferencePath::setExtension(const Poco::SharedPtr<StorageReferencePathExtension>& val)
{
	_extension = val;
}


inline void StorageReferencePath::setExtension(Poco::SharedPtr<StorageReferencePathExtension>&& val)
{
	_extension = std::move(val);
}


inline void StorageReferencePath::setRelativePath(const Poco::Optional<std::string>& val)
{
	_relativePath = val;
}


inline void StorageReferencePath::setRelativePath(Poco::Optional<std::string>&& val)
{
	_relativePath = std::move(val);
}


inline void StorageReferencePath::setStorageToken(const std::string& val)
{
	_storageToken = val;
}


inline void StorageReferencePath::setStorageToken(std::string&& val)
{
	_storageToken = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/StorageReferencePathExtension.h"


#endif // ONVIF_StorageReferencePath_INCLUDED
