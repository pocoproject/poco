// 
// SerialData.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DeviceIO_SerialData_INCLUDED
#define ONVIF_DeviceIO_SerialData_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace DeviceIO {


//@ name=SerialData
//@ namespace="http://www.onvif.org/ver10/deviceIO/wsdl"
//@ serialize
class ONVIF_API SerialData
{
public:
	SerialData();

	SerialData(
		const Poco::Optional<std::vector<char>>& binary, 
		const Poco::Optional<std::string>& string);

	virtual ~SerialData();

	const Poco::Optional<std::vector<char>>& getBinary() const;

	const Poco::Optional<std::string>& getString() const;

	void setBinary(const Poco::Optional<std::vector<char>>& val);

	void setBinary(Poco::Optional<std::vector<char>>&& val);

	void setString(const Poco::Optional<std::string>& val);

	void setString(Poco::Optional<std::string>&& val);

private:
	//@ mandatory=false
	//@ name=Binary
	//@ order=0
	Poco::Optional<std::vector<char>> _binary;

	//@ mandatory=false
	//@ name=String
	//@ order=1
	Poco::Optional<std::string> _string;

};


inline const Poco::Optional<std::vector<char>>& SerialData::getBinary() const
{
	return _binary;
}


inline const Poco::Optional<std::string>& SerialData::getString() const
{
	return _string;
}


inline void SerialData::setBinary(const Poco::Optional<std::vector<char>>& val)
{
	_binary = val;
}


inline void SerialData::setBinary(Poco::Optional<std::vector<char>>&& val)
{
	_binary = std::move(val);
}


inline void SerialData::setString(const Poco::Optional<std::string>& val)
{
	_string = val;
}


inline void SerialData::setString(Poco::Optional<std::string>&& val)
{
	_string = std::move(val);
}


} } // ONVIF::DeviceIO


#endif // ONVIF_DeviceIO_SerialData_INCLUDED
