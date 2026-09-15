// 
// IOCapabilities.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_IOCapabilities_INCLUDED
#define ONVIF_IOCapabilities_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class IOCapabilitiesExtension;
} 


namespace ONVIF {


//@ name=IOCapabilities
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API IOCapabilities
{
public:
	IOCapabilities();

	IOCapabilities(
		Poco::Optional<int> inputConnectors, 
		Poco::Optional<int> relayOutputs, 
		const Poco::SharedPtr<IOCapabilitiesExtension>& extension);

	virtual ~IOCapabilities();

	const Poco::SharedPtr<IOCapabilitiesExtension>& getExtension() const;

	Poco::Optional<int> getInputConnectors() const;

	Poco::Optional<int> getRelayOutputs() const;

	void setExtension(const Poco::SharedPtr<IOCapabilitiesExtension>& val);

	void setExtension(Poco::SharedPtr<IOCapabilitiesExtension>&& val);

	void setInputConnectors(Poco::Optional<int> val);

	void setRelayOutputs(Poco::Optional<int> val);

private:
	//@ mandatory=false
	//@ name=InputConnectors
	//@ order=0
	Poco::Optional<int> _inputConnectors;

	//@ mandatory=false
	//@ name=RelayOutputs
	//@ order=1
	Poco::Optional<int> _relayOutputs;

	//@ mandatory=false
	//@ name=Extension
	//@ order=2
	Poco::SharedPtr<IOCapabilitiesExtension> _extension;

};


inline const Poco::SharedPtr<IOCapabilitiesExtension>& IOCapabilities::getExtension() const
{
	return _extension;
}


inline Poco::Optional<int> IOCapabilities::getInputConnectors() const
{
	return _inputConnectors;
}


inline Poco::Optional<int> IOCapabilities::getRelayOutputs() const
{
	return _relayOutputs;
}


inline void IOCapabilities::setExtension(const Poco::SharedPtr<IOCapabilitiesExtension>& val)
{
	_extension = val;
}


inline void IOCapabilities::setExtension(Poco::SharedPtr<IOCapabilitiesExtension>&& val)
{
	_extension = std::move(val);
}


inline void IOCapabilities::setInputConnectors(Poco::Optional<int> val)
{
	_inputConnectors = val;
}


inline void IOCapabilities::setRelayOutputs(Poco::Optional<int> val)
{
	_relayOutputs = val;
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/IOCapabilitiesExtension.h"


#endif // ONVIF_IOCapabilities_INCLUDED
