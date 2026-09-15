// 
// SetAccessPolicy.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_SetAccessPolicy_INCLUDED
#define ONVIF_Device_SetAccessPolicy_INCLUDED


#include "ONVIF/BinaryData.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#SetAccessPolicy"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API SetAccessPolicy
{
public:
	SetAccessPolicy();

	SetAccessPolicy(const ONVIF::BinaryData& policyFile);

	virtual ~SetAccessPolicy();

	const ONVIF::BinaryData& getPolicyFile() const;

	void setPolicyFile(const ONVIF::BinaryData& val);

	void setPolicyFile(ONVIF::BinaryData&& val);

private:
	//@ name=PolicyFile
	//@ order=0
	ONVIF::BinaryData _policyFile;

};


inline const ONVIF::BinaryData& SetAccessPolicy::getPolicyFile() const
{
	return _policyFile;
}


inline void SetAccessPolicy::setPolicyFile(const ONVIF::BinaryData& val)
{
	_policyFile = val;
}


inline void SetAccessPolicy::setPolicyFile(ONVIF::BinaryData&& val)
{
	_policyFile = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_SetAccessPolicy_INCLUDED
