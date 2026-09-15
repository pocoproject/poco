//
// ICheckVatService.h
//
// Package: Generated
// Module:  ICheckVatService
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef ICheckVatService_INCLUDED
#define ICheckVatService_INCLUDED


#include "CheckVat/CheckVatService.h"
#include "Poco/AutoPtr.h"
#include "Poco/RefCountedObject.h"
#include "Poco/RemotingNG/Identifiable.h"


namespace CheckVat {


class ICheckVatService: public virtual Poco::RefCountedObject
{
public:
	using Ptr = Poco::AutoPtr<ICheckVatService>;

	ICheckVatService();
		/// Creates a ICheckVatService.

	virtual ~ICheckVatService();
		/// Destroys the ICheckVatService.

	virtual void checkVat(const std::string& countryCode, const std::string& vatNumber, std::string& countryCode2, std::string& vatNumber2, Poco::DateTime& requestDate, bool& valid, Poco::Nullable<std::string>& name, Poco::Nullable<std::string>& address) = 0;

	virtual void checkVatApprox(const std::string& countryCode, const std::string& vatNumber, const Poco::Optional<std::string>& traderName, const Poco::Optional<std::string>& traderCompanyType, const Poco::Optional<std::string>& traderStreet, const Poco::Optional<std::string>& traderPostcode, const Poco::Optional<std::string>& traderCity, const Poco::Optional<std::string>& requesterCountryCode, const Poco::Optional<std::string>& requesterVatNumber, std::string& countryCode2, std::string& vatNumber2, Poco::DateTime& requestDate, bool& valid, Poco::Nullable<std::string>& traderName2, Poco::Nullable<std::string>& traderCompanyType2, Poco::Optional<std::string>& traderAddress, Poco::Optional<std::string>& traderStreet2, Poco::Optional<std::string>& traderPostcode2, Poco::Optional<std::string>& traderCity2, Poco::Optional<std::string>& traderNameMatch, Poco::Optional<std::string>& traderCompanyTypeMatch, Poco::Optional<std::string>& traderStreetMatch, Poco::Optional<std::string>& traderPostcodeMatch, Poco::Optional<std::string>& traderCityMatch, std::string& requestIdentifier) = 0;

	static const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId();
		/// Returns the TypeId of the class.

};


} // namespace CheckVat


#endif // ICheckVatService_INCLUDED

