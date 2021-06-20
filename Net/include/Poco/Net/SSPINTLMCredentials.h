//
// SSPINTLMCredentials.h
//
// Library: Net
// Package: NTLM
// Module:	SSPINTLMCredentials
//
// Definition of the SSPINTLMCredentials class.
//
// Copyright (c) 2019, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/Net.h"
#include <vector>


#ifndef Net_SSPINTLMCredentials_INCLUDED
#define Net_SSPINTLMCredentials_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Net/NTLMCredentials.h"
#include "Poco/SharedPtr.h"


namespace Poco {
namespace Net {


struct NTLMContextImpl;


class Net_API NTLMContext
	/// An opaque context class for working with SSPI NTLM authentication.
{
public:
	~NTLMContext();

protected:
	NTLMContext(NTLMContextImpl* pImpl);

private:
	NTLMContextImpl* _pImpl;

	NTLMContext();
	NTLMContext(const NTLMContext&);
	NTLMContext& operator = (const NTLMContext&);

	friend class SSPINTLMProvider;
};


class Net_API SSPINTLMCredentials
	/// Support for NTLM authentication using credentials of the currently
	/// logged in user via SSPI.
{
public:
	static bool available();
		/// Returns true if SSPI NTLM support is available.

	static Poco::SharedPtr<NTLMContext> createNTLMContext(const std::string& host, const std::string& service);
		/// Creates an NTLMContext structure for use with negotiate()
		/// and authenticate().

	static std::vector<unsigned char> negotiate(NTLMContext& context);
		/// Creates the NTLM Type 1 Negotiate message used for initiating NTLM authentication from the client.

	static std::vector<unsigned char> authenticate(NTLMContext& context, const std::vector<unsigned char>& challenge);
		/// Creates the NTLM Type 3 Authenticate message used for sending the response to the challenge.

	static const std::string SERVICE_HTTP;
	static const std::string SERVICE_SMTP;
};


} } // namespace Poco::Net


#endif // Net_SSPINTLMCredentials_INCLUDED
