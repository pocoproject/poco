// 
// GetAccessPolicyResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_GetAccessPolicyResponse_INCLUDED
#define ONVIF_Device_GetAccessPolicyResponse_INCLUDED


#include "ONVIF/BinaryData.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#GetAccessPolicyResponse"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API GetAccessPolicyResponse
{
public:
	GetAccessPolicyResponse();

	GetAccessPolicyResponse(const ONVIF::BinaryData& policyFile);

	virtual ~GetAccessPolicyResponse();

	const ONVIF::BinaryData& getPolicyFile() const;

	void setPolicyFile(const ONVIF::BinaryData& val);

	void setPolicyFile(ONVIF::BinaryData&& val);

private:
	//@ name=PolicyFile
	//@ order=0
	ONVIF::BinaryData _policyFile;

};


inline const ONVIF::BinaryData& GetAccessPolicyResponse::getPolicyFile() const
{
	return _policyFile;
}


inline void GetAccessPolicyResponse::setPolicyFile(const ONVIF::BinaryData& val)
{
	_policyFile = val;
}


inline void GetAccessPolicyResponse::setPolicyFile(ONVIF::BinaryData&& val)
{
	_policyFile = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_GetAccessPolicyResponse_INCLUDED
