// 
// BinaryData.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_BinaryData_INCLUDED
#define ONVIF_BinaryData_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=BinaryData
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API BinaryData
{
public:
	BinaryData();

	BinaryData(
		const Poco::Optional<std::string>& contentType, 
		const std::vector<char>& data);

	virtual ~BinaryData();

	const Poco::Optional<std::string>& getContentType() const;

	const std::vector<char>& getData() const;

	void setContentType(const Poco::Optional<std::string>& val);

	void setContentType(Poco::Optional<std::string>&& val);

	void setData(const std::vector<char>& val);

	void setData(std::vector<char>&& val);

private:
	//@ mandatory=false
	//@ name=contentType
	//@ namespace="http://www.w3.org/2005/05/xmlmime"
	//@ order=0
	//@ type=attr
	Poco::Optional<std::string> _contentType;

	//@ name=Data
	//@ order=1
	std::vector<char> _data;

};


inline const Poco::Optional<std::string>& BinaryData::getContentType() const
{
	return _contentType;
}


inline const std::vector<char>& BinaryData::getData() const
{
	return _data;
}


inline void BinaryData::setContentType(const Poco::Optional<std::string>& val)
{
	_contentType = val;
}


inline void BinaryData::setContentType(Poco::Optional<std::string>&& val)
{
	_contentType = std::move(val);
}


inline void BinaryData::setData(const std::vector<char>& val)
{
	_data = val;
}


inline void BinaryData::setData(std::vector<char>&& val)
{
	_data = std::move(val);
}


} // ONVIF


#endif // ONVIF_BinaryData_INCLUDED
