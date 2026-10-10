// 
// ParityBitList.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DeviceIO_ParityBitList_INCLUDED
#define ONVIF_DeviceIO_ParityBitList_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace DeviceIO {


//@ name=ParityBitList
//@ namespace="http://www.onvif.org/ver10/deviceIO/wsdl"
//@ serialize
class ONVIF_API ParityBitList
{
public:
	ParityBitList();

	ParityBitList(const std::vector<std::string>& items);

	virtual ~ParityBitList();

	const std::vector<std::string>& getItems() const;

	std::vector<std::string>& getItems();

	void setItems(const std::vector<std::string>& val);

	void setItems(std::vector<std::string>&& val);

private:
	//@ mandatory=false
	//@ name=Items
	//@ order=0
	std::vector<std::string> _items;

};


inline const std::vector<std::string>& ParityBitList::getItems() const
{
	return _items;
}


inline std::vector<std::string>& ParityBitList::getItems()
{
	return _items;
}


inline void ParityBitList::setItems(const std::vector<std::string>& val)
{
	_items = val;
}


inline void ParityBitList::setItems(std::vector<std::string>&& val)
{
	_items = std::move(val);
}


} } // ONVIF::DeviceIO


#endif // ONVIF_DeviceIO_ParityBitList_INCLUDED
