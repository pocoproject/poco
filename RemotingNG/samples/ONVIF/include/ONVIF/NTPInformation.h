// 
// NTPInformation.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_NTPInformation_INCLUDED
#define ONVIF_NTPInformation_INCLUDED


#include "ONVIF/NetworkHost.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class NTPInformationExtension;
} 


namespace ONVIF {


//@ name=NTPInformation
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API NTPInformation
{
public:
	NTPInformation();

	NTPInformation(
		bool fromDHCP, 
		const std::vector<NetworkHost>& nTPFromDHCP, 
		const std::vector<NetworkHost>& nTPManual, 
		const Poco::SharedPtr<NTPInformationExtension>& extension);

	virtual ~NTPInformation();

	const Poco::SharedPtr<NTPInformationExtension>& getExtension() const;

	bool getFromDHCP() const;

	const std::vector<NetworkHost>& getNTPFromDHCP() const;

	std::vector<NetworkHost>& getNTPFromDHCP();

	const std::vector<NetworkHost>& getNTPManual() const;

	std::vector<NetworkHost>& getNTPManual();

	void setExtension(const Poco::SharedPtr<NTPInformationExtension>& val);

	void setExtension(Poco::SharedPtr<NTPInformationExtension>&& val);

	void setFromDHCP(bool val);

	void setNTPFromDHCP(const std::vector<NetworkHost>& val);

	void setNTPFromDHCP(std::vector<NetworkHost>&& val);

	void setNTPManual(const std::vector<NetworkHost>& val);

	void setNTPManual(std::vector<NetworkHost>&& val);

private:
	//@ name=FromDHCP
	//@ order=0
	bool _fromDHCP;

	//@ mandatory=false
	//@ name=NTPFromDHCP
	//@ order=1
	std::vector<NetworkHost> _nTPFromDHCP;

	//@ mandatory=false
	//@ name=NTPManual
	//@ order=2
	std::vector<NetworkHost> _nTPManual;

	//@ mandatory=false
	//@ name=Extension
	//@ order=3
	Poco::SharedPtr<NTPInformationExtension> _extension;

};


inline const Poco::SharedPtr<NTPInformationExtension>& NTPInformation::getExtension() const
{
	return _extension;
}


inline bool NTPInformation::getFromDHCP() const
{
	return _fromDHCP;
}


inline const std::vector<NetworkHost>& NTPInformation::getNTPFromDHCP() const
{
	return _nTPFromDHCP;
}


inline std::vector<NetworkHost>& NTPInformation::getNTPFromDHCP()
{
	return _nTPFromDHCP;
}


inline const std::vector<NetworkHost>& NTPInformation::getNTPManual() const
{
	return _nTPManual;
}


inline std::vector<NetworkHost>& NTPInformation::getNTPManual()
{
	return _nTPManual;
}


inline void NTPInformation::setExtension(const Poco::SharedPtr<NTPInformationExtension>& val)
{
	_extension = val;
}


inline void NTPInformation::setExtension(Poco::SharedPtr<NTPInformationExtension>&& val)
{
	_extension = std::move(val);
}


inline void NTPInformation::setFromDHCP(bool val)
{
	_fromDHCP = val;
}


inline void NTPInformation::setNTPFromDHCP(const std::vector<NetworkHost>& val)
{
	_nTPFromDHCP = val;
}


inline void NTPInformation::setNTPFromDHCP(std::vector<NetworkHost>&& val)
{
	_nTPFromDHCP = std::move(val);
}


inline void NTPInformation::setNTPManual(const std::vector<NetworkHost>& val)
{
	_nTPManual = val;
}


inline void NTPInformation::setNTPManual(std::vector<NetworkHost>&& val)
{
	_nTPManual = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/NTPInformationExtension.h"


#endif // ONVIF_NTPInformation_INCLUDED
