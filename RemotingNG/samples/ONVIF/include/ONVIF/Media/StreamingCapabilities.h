// 
// StreamingCapabilities.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_StreamingCapabilities_INCLUDED
#define ONVIF_Media_StreamingCapabilities_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name=StreamingCapabilities
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API StreamingCapabilities
{
public:
	StreamingCapabilities();

	StreamingCapabilities(
		Poco::Optional<bool> rTPMulticast, 
		Poco::Optional<bool> rTP_TCP, 
		Poco::Optional<bool> rTP_RTSP_TCP, 
		Poco::Optional<bool> nonAggregateControl, 
		Poco::Optional<bool> noRTSPStreaming);

	virtual ~StreamingCapabilities();

	Poco::Optional<bool> getNoRTSPStreaming() const;

	Poco::Optional<bool> getNonAggregateControl() const;

	Poco::Optional<bool> getRTPMulticast() const;

	Poco::Optional<bool> getRTP_RTSP_TCP() const;

	Poco::Optional<bool> getRTP_TCP() const;

	void setNoRTSPStreaming(Poco::Optional<bool> val);

	void setNonAggregateControl(Poco::Optional<bool> val);

	void setRTPMulticast(Poco::Optional<bool> val);

	void setRTP_RTSP_TCP(Poco::Optional<bool> val);

	void setRTP_TCP(Poco::Optional<bool> val);

private:
	//@ mandatory=false
	//@ name=RTPMulticast
	//@ order=0
	//@ type=attr
	Poco::Optional<bool> _rTPMulticast;

	//@ mandatory=false
	//@ name=RTP_TCP
	//@ order=1
	//@ type=attr
	Poco::Optional<bool> _rTP_TCP;

	//@ mandatory=false
	//@ name=RTP_RTSP_TCP
	//@ order=2
	//@ type=attr
	Poco::Optional<bool> _rTP_RTSP_TCP;

	//@ mandatory=false
	//@ name=NonAggregateControl
	//@ order=3
	//@ type=attr
	Poco::Optional<bool> _nonAggregateControl;

	//@ mandatory=false
	//@ name=NoRTSPStreaming
	//@ order=4
	//@ type=attr
	Poco::Optional<bool> _noRTSPStreaming;

};


inline Poco::Optional<bool> StreamingCapabilities::getNoRTSPStreaming() const
{
	return _noRTSPStreaming;
}


inline Poco::Optional<bool> StreamingCapabilities::getNonAggregateControl() const
{
	return _nonAggregateControl;
}


inline Poco::Optional<bool> StreamingCapabilities::getRTPMulticast() const
{
	return _rTPMulticast;
}


inline Poco::Optional<bool> StreamingCapabilities::getRTP_RTSP_TCP() const
{
	return _rTP_RTSP_TCP;
}


inline Poco::Optional<bool> StreamingCapabilities::getRTP_TCP() const
{
	return _rTP_TCP;
}


inline void StreamingCapabilities::setNoRTSPStreaming(Poco::Optional<bool> val)
{
	_noRTSPStreaming = val;
}


inline void StreamingCapabilities::setNonAggregateControl(Poco::Optional<bool> val)
{
	_nonAggregateControl = val;
}


inline void StreamingCapabilities::setRTPMulticast(Poco::Optional<bool> val)
{
	_rTPMulticast = val;
}


inline void StreamingCapabilities::setRTP_RTSP_TCP(Poco::Optional<bool> val)
{
	_rTP_RTSP_TCP = val;
}


inline void StreamingCapabilities::setRTP_TCP(Poco::Optional<bool> val)
{
	_rTP_TCP = val;
}


} } // ONVIF::Media


#endif // ONVIF_Media_StreamingCapabilities_INCLUDED
