//
// SOAPFaultException.h
//
// Library: RemotingNG/SOAP
// Package: SOAP
// Module:  SOAPFaultException
//
// Definition of the SOAPFaultException class.
//
// Copyright (c) 2024, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties,
// copied or duplicated in any form, in whole or in part.
//


#ifndef RemotingNG_SOAP_SOAPFaultException_INCLUDED
#define RemotingNG_SOAP_SOAPFaultException_INCLUDED


#include "Poco/RemotingNG/RemotingException.h"
#include "Poco/RemotingNG/SOAP/XMLElement.h"


namespace Poco {
namespace RemotingNG {
namespace SOAP {


class RemotingNGSOAP_API SOAPFaultException: public Poco::RemotingNG::RemoteException
	/// This exception class contains detailed information from a SOAPFault
	/// received from the server.
{
public:
	SOAPFaultException(const std::string& message, const XMLElement& fault);
		/// Creates a SOAPFaultException with the given message and code.

	SOAPFaultException(const SOAPFaultException& exc);
		/// Creates a new SOAPFaultException from another one.

	~SOAPFaultException() noexcept;
		/// Destroy the exception.

	SOAPFaultException& operator = (const SOAPFaultException& exc);
		/// Assignment operator.

	const char* name() const noexcept;
		/// Returns a static string describing the exception.

	const char* className() const noexcept;
		/// Returns the name of the exception class.

	Poco::Exception* clone() const;
		/// Creates an exact copy of the exception.

	void rethrow() const;
		/// (Re)Throws the exception.

	const XMLElement& fault() const;
		/// Returns the SOAP Fault element.

	const std::string& namespaceURI() const;
		/// Returns the namespace URI of the SOAP Fault element.
		/// This can be used to distinguish between a SOAP 1.1
		/// and 1.2 message.

	std::string faultCode() const;
		/// Returns the value of the "faultcode" (SOAP 1.1) or "Fault/Code/Value" (SOAP 1.2) element.

	std::string faultReason() const;
		/// Returns the value of the "faultstring" (SOAP 1.1) or "Fault/Reason/Text" (SOAP 1.2) element.

	const XMLElement* faultDetail() const;
		/// Returns a pointer to the "detail" (SOAP 1.1) or "Detail" (SOAP 1.2) element
		/// if it exists, otherwise nullptr.

private:
	XMLElement _fault;
};


//
// inlines
//


inline const XMLElement& SOAPFaultException::fault() const
{
	return _fault;
}


inline const std::string& SOAPFaultException::namespaceURI() const
{
	return _fault.namespaceURI();
}


} } } // namespace Poco::RemotingNG::SOAP


#endif // RemotingNG_SOAP_SOAPFaultException_INCLUDED
