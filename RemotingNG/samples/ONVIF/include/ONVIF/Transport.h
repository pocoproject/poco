// 
// Transport.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Transport_INCLUDED
#define ONVIF_Transport_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class Transport;
} 


namespace ONVIF {


//@ name=Transport
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API Transport
{
public:
	Transport();

	Transport(
		const std::string& protocol, 
		const Poco::SharedPtr<Transport>& tunnel);

	virtual ~Transport();

	const std::string& getProtocol() const;

	const Poco::SharedPtr<Transport>& getTunnel() const;

	void setProtocol(const std::string& val);

	void setProtocol(std::string&& val);

	void setTunnel(const Poco::SharedPtr<Transport>& val);

	void setTunnel(Poco::SharedPtr<Transport>&& val);

private:
	//@ name=Protocol
	//@ order=0
	std::string _protocol;

	//@ mandatory=false
	//@ name=Tunnel
	//@ order=1
	Poco::SharedPtr<Transport> _tunnel;

};


inline const std::string& Transport::getProtocol() const
{
	return _protocol;
}


inline const Poco::SharedPtr<Transport>& Transport::getTunnel() const
{
	return _tunnel;
}


inline void Transport::setProtocol(const std::string& val)
{
	_protocol = val;
}


inline void Transport::setProtocol(std::string&& val)
{
	_protocol = std::move(val);
}


inline void Transport::setTunnel(const Poco::SharedPtr<Transport>& val)
{
	_tunnel = val;
}


inline void Transport::setTunnel(Poco::SharedPtr<Transport>&& val)
{
	_tunnel = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/Transport.h"


#endif // ONVIF_Transport_INCLUDED
