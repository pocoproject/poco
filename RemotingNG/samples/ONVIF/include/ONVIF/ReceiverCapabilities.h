// 
// ReceiverCapabilities.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_ReceiverCapabilities_INCLUDED
#define ONVIF_ReceiverCapabilities_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/URI.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=ReceiverCapabilities
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API ReceiverCapabilities
{
public:
	ReceiverCapabilities();

	ReceiverCapabilities(
		const Poco::URI& xAddr, 
		bool rTP_Multicast, 
		bool rTP_TCP, 
		bool rTP_RTSP_TCP, 
		int supportedReceivers, 
		int maximumRTSPURILength);

	virtual ~ReceiverCapabilities();

	int getMaximumRTSPURILength() const;

	bool getRTP_Multicast() const;

	bool getRTP_RTSP_TCP() const;

	bool getRTP_TCP() const;

	int getSupportedReceivers() const;

	const Poco::URI& getXAddr() const;

	void setMaximumRTSPURILength(int val);

	void setRTP_Multicast(bool val);

	void setRTP_RTSP_TCP(bool val);

	void setRTP_TCP(bool val);

	void setSupportedReceivers(int val);

	void setXAddr(const Poco::URI& val);

	void setXAddr(Poco::URI&& val);

private:
	//@ name=XAddr
	//@ order=0
	Poco::URI _xAddr;

	//@ name=RTP_Multicast
	//@ order=1
	bool _rTP_Multicast;

	//@ name=RTP_TCP
	//@ order=2
	bool _rTP_TCP;

	//@ name=RTP_RTSP_TCP
	//@ order=3
	bool _rTP_RTSP_TCP;

	//@ name=SupportedReceivers
	//@ order=4
	int _supportedReceivers;

	//@ name=MaximumRTSPURILength
	//@ order=5
	int _maximumRTSPURILength;

};


inline int ReceiverCapabilities::getMaximumRTSPURILength() const
{
	return _maximumRTSPURILength;
}


inline bool ReceiverCapabilities::getRTP_Multicast() const
{
	return _rTP_Multicast;
}


inline bool ReceiverCapabilities::getRTP_RTSP_TCP() const
{
	return _rTP_RTSP_TCP;
}


inline bool ReceiverCapabilities::getRTP_TCP() const
{
	return _rTP_TCP;
}


inline int ReceiverCapabilities::getSupportedReceivers() const
{
	return _supportedReceivers;
}


inline const Poco::URI& ReceiverCapabilities::getXAddr() const
{
	return _xAddr;
}


inline void ReceiverCapabilities::setMaximumRTSPURILength(int val)
{
	_maximumRTSPURILength = val;
}


inline void ReceiverCapabilities::setRTP_Multicast(bool val)
{
	_rTP_Multicast = val;
}


inline void ReceiverCapabilities::setRTP_RTSP_TCP(bool val)
{
	_rTP_RTSP_TCP = val;
}


inline void ReceiverCapabilities::setRTP_TCP(bool val)
{
	_rTP_TCP = val;
}


inline void ReceiverCapabilities::setSupportedReceivers(int val)
{
	_supportedReceivers = val;
}


inline void ReceiverCapabilities::setXAddr(const Poco::URI& val)
{
	_xAddr = val;
}


inline void ReceiverCapabilities::setXAddr(Poco::URI&& val)
{
	_xAddr = std::move(val);
}


} // ONVIF


#endif // ONVIF_ReceiverCapabilities_INCLUDED
