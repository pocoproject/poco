// 
// NetworkProtocol.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_NetworkProtocol_INCLUDED
#define ONVIF_NetworkProtocol_INCLUDED


#include "ONVIF/NetworkProtocol.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class NetworkProtocolExtension;
} 


namespace ONVIF {


//@ name=NetworkProtocol
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API NetworkProtocol
{
public:
	NetworkProtocol();

	NetworkProtocol(
		const Poco::SharedPtr<NetworkProtocol>& name, 
		bool enabled, 
		const std::vector<int>& port, 
		const Poco::SharedPtr<NetworkProtocolExtension>& extension);

	virtual ~NetworkProtocol();

	bool getEnabled() const;

	const Poco::SharedPtr<NetworkProtocolExtension>& getExtension() const;

	const Poco::SharedPtr<NetworkProtocol>& getName() const;

	const std::vector<int>& getPort() const;

	std::vector<int>& getPort();

	void setEnabled(bool val);

	void setExtension(const Poco::SharedPtr<NetworkProtocolExtension>& val);

	void setExtension(Poco::SharedPtr<NetworkProtocolExtension>&& val);

	void setName(const Poco::SharedPtr<NetworkProtocol>& val);

	void setName(Poco::SharedPtr<NetworkProtocol>&& val);

	void setPort(const std::vector<int>& val);

private:
	//@ name=Name
	//@ order=0
	Poco::SharedPtr<NetworkProtocol> _name;

	//@ name=Enabled
	//@ order=1
	bool _enabled;

	//@ name=Port
	//@ order=2
	std::vector<int> _port;

	//@ mandatory=false
	//@ name=Extension
	//@ order=3
	Poco::SharedPtr<NetworkProtocolExtension> _extension;

};


inline bool NetworkProtocol::getEnabled() const
{
	return _enabled;
}


inline const Poco::SharedPtr<NetworkProtocolExtension>& NetworkProtocol::getExtension() const
{
	return _extension;
}


inline const Poco::SharedPtr<NetworkProtocol>& NetworkProtocol::getName() const
{
	return _name;
}


inline const std::vector<int>& NetworkProtocol::getPort() const
{
	return _port;
}


inline std::vector<int>& NetworkProtocol::getPort()
{
	return _port;
}


inline void NetworkProtocol::setEnabled(bool val)
{
	_enabled = val;
}


inline void NetworkProtocol::setExtension(const Poco::SharedPtr<NetworkProtocolExtension>& val)
{
	_extension = val;
}


inline void NetworkProtocol::setExtension(Poco::SharedPtr<NetworkProtocolExtension>&& val)
{
	_extension = std::move(val);
}


inline void NetworkProtocol::setName(const Poco::SharedPtr<NetworkProtocol>& val)
{
	_name = val;
}


inline void NetworkProtocol::setName(Poco::SharedPtr<NetworkProtocol>&& val)
{
	_name = std::move(val);
}


inline void NetworkProtocol::setPort(const std::vector<int>& val)
{
	_port = val;
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/NetworkProtocolExtension.h"


#endif // ONVIF_NetworkProtocol_INCLUDED
