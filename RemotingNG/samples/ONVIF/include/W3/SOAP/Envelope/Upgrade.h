// 
// Upgrade.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef W3_SOAP_Envelope_Upgrade_INCLUDED
#define W3_SOAP_Envelope_Upgrade_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "W3/SOAP/Envelope/SupportedEnv.h"
#include <string>
#include <vector>


namespace W3 {
namespace SOAP {
namespace Envelope {


//@ name=UpgradeType
//@ namespace="http://www.w3.org/2003/05/soap-envelope"
//@ serialize
class ONVIF_API Upgrade
{
public:
	Upgrade();

	Upgrade(const std::vector<SupportedEnv>& supportedEnvelope);

	virtual ~Upgrade();

	const std::vector<SupportedEnv>& getSupportedEnvelope() const;

	std::vector<SupportedEnv>& getSupportedEnvelope();

	void setSupportedEnvelope(const std::vector<SupportedEnv>& val);

	void setSupportedEnvelope(std::vector<SupportedEnv>&& val);

private:
	//@ name=SupportedEnvelope
	//@ order=0
	std::vector<SupportedEnv> _supportedEnvelope;

};


inline const std::vector<SupportedEnv>& Upgrade::getSupportedEnvelope() const
{
	return _supportedEnvelope;
}


inline std::vector<SupportedEnv>& Upgrade::getSupportedEnvelope()
{
	return _supportedEnvelope;
}


inline void Upgrade::setSupportedEnvelope(const std::vector<SupportedEnv>& val)
{
	_supportedEnvelope = val;
}


inline void Upgrade::setSupportedEnvelope(std::vector<SupportedEnv>&& val)
{
	_supportedEnvelope = std::move(val);
}


} } } // W3::SOAP::Envelope


#endif // W3_SOAP_Envelope_Upgrade_INCLUDED
