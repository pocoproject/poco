// 
// SetHashingAlgorithm.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_SetHashingAlgorithm_INCLUDED
#define ONVIF_Device_SetHashingAlgorithm_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#SetHashingAlgorithm"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API SetHashingAlgorithm
{
public:
	SetHashingAlgorithm();

	SetHashingAlgorithm(const std::vector<std::string>& algorithm);

	virtual ~SetHashingAlgorithm();

	const std::vector<std::string>& getAlgorithm() const;

	void setAlgorithm(const std::vector<std::string>& val);

	void setAlgorithm(std::vector<std::string>&& val);

private:
	//@ name=Algorithm
	//@ order=0
	std::vector<std::string> _algorithm;

};


inline const std::vector<std::string>& SetHashingAlgorithm::getAlgorithm() const
{
	return _algorithm;
}


inline void SetHashingAlgorithm::setAlgorithm(const std::vector<std::string>& val)
{
	_algorithm = val;
}


inline void SetHashingAlgorithm::setAlgorithm(std::vector<std::string>&& val)
{
	_algorithm = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_SetHashingAlgorithm_INCLUDED
