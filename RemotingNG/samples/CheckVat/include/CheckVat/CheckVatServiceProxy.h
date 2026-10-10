//
// CheckVatServiceProxy.h
//
// Package: Generated
// Module:  CheckVatServiceProxy
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef CheckVatServiceProxy_INCLUDED
#define CheckVatServiceProxy_INCLUDED


#include "CheckVat/ICheckVatService.h"
#include "Poco/RemotingNG/Proxy.h"


namespace CheckVat {


class CheckVatServiceProxy: public CheckVat::ICheckVatService, public Poco::RemotingNG::Proxy
{
public:
	using Ptr = Poco::AutoPtr<CheckVatServiceProxy>;

	CheckVatServiceProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid);
		/// Creates a CheckVatServiceProxy.

	virtual ~CheckVatServiceProxy();
		/// Destroys the CheckVatServiceProxy.

	virtual void checkVat(const std::string& countryCode, const std::string& vatNumber, std::string& countryCode2, std::string& vatNumber2, Poco::DateTime& requestDate, bool& valid, Poco::Nullable<std::string>& name, Poco::Nullable<std::string>& address);

	virtual void checkVatApprox(const std::string& countryCode, const std::string& vatNumber, const Poco::Optional<std::string>& traderName, const Poco::Optional<std::string>& traderCompanyType, const Poco::Optional<std::string>& traderStreet, const Poco::Optional<std::string>& traderPostcode, const Poco::Optional<std::string>& traderCity, const Poco::Optional<std::string>& requesterCountryCode, const Poco::Optional<std::string>& requesterVatNumber, std::string& countryCode2, std::string& vatNumber2, Poco::DateTime& requestDate, bool& valid, Poco::Nullable<std::string>& traderName2, Poco::Nullable<std::string>& traderCompanyType2, Poco::Optional<std::string>& traderAddress, Poco::Optional<std::string>& traderStreet2, Poco::Optional<std::string>& traderPostcode2, Poco::Optional<std::string>& traderCity2, Poco::Optional<std::string>& traderNameMatch, Poco::Optional<std::string>& traderCompanyTypeMatch, Poco::Optional<std::string>& traderStreetMatch, Poco::Optional<std::string>& traderPostcodeMatch, Poco::Optional<std::string>& traderCityMatch, std::string& requestIdentifier);

	virtual const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId() const;

private:
	static const std::string DEFAULT_NS;
};


inline const Poco::RemotingNG::Identifiable::TypeId& CheckVatServiceProxy::remoting__typeId() const
{
	return ICheckVatService::remoting__typeId();
}


} // namespace CheckVat


#endif // CheckVatServiceProxy_INCLUDED

