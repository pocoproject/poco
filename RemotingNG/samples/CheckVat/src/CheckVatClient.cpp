//
// CheckVatClient.cpp
//
// Copyright (c) 2012-2023, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "CheckVat/CheckVatServiceClientHelper.h"
#include "Poco/RemotingNG/SOAP/TransportFactory.h"
#include "Poco/RemotingNG/SOAP/Transport.h"
#include "Poco/RemotingNG/SOAP/Serializer.h"
#include "Poco/RemotingNG/SOAP/SOAPFaultException.h"
#include "Poco/RemotingNG/Proxy.h"
#ifdef CHECKVAT_ENABLE_HTTPS
#include "Poco/Net/HTTPSSessionInstantiator.h"
#endif
#include <iostream>


template <class I>
Poco::RemotingNG::SOAP::Transport& transportFromInterface(Poco::AutoPtr<I> pInterface)
{
    Poco::RemotingNG::Proxy::Ptr pProxy = pInterface.template cast<Poco::RemotingNG::Proxy>();
    if (pProxy)
    {
        return static_cast<Poco::RemotingNG::SOAP::Transport&>(pProxy->remoting__transport());
    }
    else throw Poco::BadCastException();
}


int main(int argc, char** argv)
{
	try
	{
		// register transport
		Poco::RemotingNG::SOAP::TransportFactory::registerFactory();

#ifdef CHECKVAT_ENABLE_HTTPS
		// enable HTTPS
		Poco::RemotingNG::SOAP::Transport::httpSessionFactory().registerProtocol(
			"https",
			new Poco::Net::HTTPSSessionInstantiator
		);
#endif

		// get proxy for CheckVatService web service
#ifdef CHECKVAT_ENABLE_HTTPS
		CheckVat::ICheckVatService::Ptr pVatService = CheckVat::CheckVatServiceClientHelper::find("https://ec.europa.eu/taxation_customs/vies/services/checkVatService", "soap");
#else
		CheckVat::ICheckVatService::Ptr pVatService = CheckVat::CheckVatServiceClientHelper::find("http://ec.europa.eu/taxation_customs/vies/services/checkVatService", "soap");
#endif

		// the service does not like the encodingStyle attribute
		transportFromInterface(pVatService).setSerializerOptions(Poco::RemotingNG::SOAP::Serializer::OPT_NO_ENCODINGSTYLE_ATTRIBUTE);

		std::string countryCode;
		std::string vatNumber;
		Poco::DateTime dateTime;
		bool valid;
		Poco::Nullable<std::string> name;
		Poco::Nullable<std::string> address;
		
		// invoke web service
		pVatService->checkVat("AT", "U62417138", countryCode, vatNumber, dateTime, valid, name, address);
		
		std::cout << "countryCode: " << countryCode << std::endl;
		std::cout << "vatNumber:   " << vatNumber << std::endl;
		std::cout << "valid:       " << (valid ? "true" : "false") << std::endl;
		if (!name.isNull()) 
			std::cout << "name:        " << name.value() << std::endl;
		if (!address.isNull())
			std::cout << "address:     " << address.value() << std::endl; 
	}
	catch (Poco::RemotingNG::SOAP::SOAPFaultException& exc)
	{
		std::cerr 
			<< exc.displayText() << "\n"
			<< "  Code:   " << exc.faultCode() << "\n"
			<< "  Reason: " << exc.faultReason() << std::endl;
	}
	catch (Poco::Exception& exc)
	{
		std::cerr << exc.displayText() << std::endl;
		return 1;
	}
	return 0;
}
