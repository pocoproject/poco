// 
// RealTimeStreamingCapabilities.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_RealTimeStreamingCapabilities_INCLUDED
#define ONVIF_RealTimeStreamingCapabilities_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class RealTimeStreamingCapabilitiesExtension;
} 


namespace ONVIF {


//@ name=RealTimeStreamingCapabilities
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API RealTimeStreamingCapabilities
{
public:
	RealTimeStreamingCapabilities();

	RealTimeStreamingCapabilities(
		Poco::Optional<bool> rTPMulticast, 
		Poco::Optional<bool> rTP_TCP, 
		Poco::Optional<bool> rTP_RTSP_TCP, 
		const Poco::SharedPtr<RealTimeStreamingCapabilitiesExtension>& extension);

	virtual ~RealTimeStreamingCapabilities();

	const Poco::SharedPtr<RealTimeStreamingCapabilitiesExtension>& getExtension() const;

	Poco::Optional<bool> getRTPMulticast() const;

	Poco::Optional<bool> getRTP_RTSP_TCP() const;

	Poco::Optional<bool> getRTP_TCP() const;

	void setExtension(const Poco::SharedPtr<RealTimeStreamingCapabilitiesExtension>& val);

	void setExtension(Poco::SharedPtr<RealTimeStreamingCapabilitiesExtension>&& val);

	void setRTPMulticast(Poco::Optional<bool> val);

	void setRTP_RTSP_TCP(Poco::Optional<bool> val);

	void setRTP_TCP(Poco::Optional<bool> val);

private:
	//@ mandatory=false
	//@ name=RTPMulticast
	//@ order=0
	Poco::Optional<bool> _rTPMulticast;

	//@ mandatory=false
	//@ name=RTP_TCP
	//@ order=1
	Poco::Optional<bool> _rTP_TCP;

	//@ mandatory=false
	//@ name=RTP_RTSP_TCP
	//@ order=2
	Poco::Optional<bool> _rTP_RTSP_TCP;

	//@ mandatory=false
	//@ name=Extension
	//@ order=3
	Poco::SharedPtr<RealTimeStreamingCapabilitiesExtension> _extension;

};


inline const Poco::SharedPtr<RealTimeStreamingCapabilitiesExtension>& RealTimeStreamingCapabilities::getExtension() const
{
	return _extension;
}


inline Poco::Optional<bool> RealTimeStreamingCapabilities::getRTPMulticast() const
{
	return _rTPMulticast;
}


inline Poco::Optional<bool> RealTimeStreamingCapabilities::getRTP_RTSP_TCP() const
{
	return _rTP_RTSP_TCP;
}


inline Poco::Optional<bool> RealTimeStreamingCapabilities::getRTP_TCP() const
{
	return _rTP_TCP;
}


inline void RealTimeStreamingCapabilities::setExtension(const Poco::SharedPtr<RealTimeStreamingCapabilitiesExtension>& val)
{
	_extension = val;
}


inline void RealTimeStreamingCapabilities::setExtension(Poco::SharedPtr<RealTimeStreamingCapabilitiesExtension>&& val)
{
	_extension = std::move(val);
}


inline void RealTimeStreamingCapabilities::setRTPMulticast(Poco::Optional<bool> val)
{
	_rTPMulticast = val;
}


inline void RealTimeStreamingCapabilities::setRTP_RTSP_TCP(Poco::Optional<bool> val)
{
	_rTP_RTSP_TCP = val;
}


inline void RealTimeStreamingCapabilities::setRTP_TCP(Poco::Optional<bool> val)
{
	_rTP_TCP = val;
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/RealTimeStreamingCapabilitiesExtension.h"


#endif // ONVIF_RealTimeStreamingCapabilities_INCLUDED
