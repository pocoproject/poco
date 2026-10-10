// 
// CreatePullPointResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef OASIS_WSN_B2_CreatePullPointResponse_INCLUDED
#define OASIS_WSN_B2_CreatePullPointResponse_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "W3/Addressing/EndpointReference.h"
#include <string>
#include <vector>


namespace OASIS {
namespace WSN {
namespace B2 {


//@ name="#CreatePullPointResponse"
//@ namespace="http://docs.oasis-open.org/wsn/b-2"
//@ serialize
class ONVIF_API CreatePullPointResponse
{
public:
	CreatePullPointResponse();

	CreatePullPointResponse(const W3::Addressing::EndpointReference& pullPoint);

	virtual ~CreatePullPointResponse();

	const W3::Addressing::EndpointReference& getPullPoint() const;

	void setPullPoint(const W3::Addressing::EndpointReference& val);

	void setPullPoint(W3::Addressing::EndpointReference&& val);

private:
	//@ name=PullPoint
	//@ order=0
	W3::Addressing::EndpointReference _pullPoint;

};


inline const W3::Addressing::EndpointReference& CreatePullPointResponse::getPullPoint() const
{
	return _pullPoint;
}


inline void CreatePullPointResponse::setPullPoint(const W3::Addressing::EndpointReference& val)
{
	_pullPoint = val;
}


inline void CreatePullPointResponse::setPullPoint(W3::Addressing::EndpointReference&& val)
{
	_pullPoint = std::move(val);
}


} } } // OASIS::WSN::B2


#endif // OASIS_WSN_B2_CreatePullPointResponse_INCLUDED
