//
// SSPINTLMCredentials.cpp
//
// Library: Net
// Package: NTLM
// Module:	SSPINTLMCredentials
//
// Copyright (c) 2019, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/SSPINTLMCredentials.h"


#if POCO_OS == POCO_OS_WINDOWS_NT


#include "Poco/SharedLibrary.h"
#include "Poco/UnicodeConverter.h"
#include <vector>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define SECURITY_WIN32
#include <security.h>


namespace Poco {
namespace Net {


struct NTLMContextImpl
{
	NTLMContextImpl():
		maxTokenSize(0)
	{
		SecInvalidateHandle(&credentials);
		SecInvalidateHandle(&context);
	}

	std::size_t  maxTokenSize;
	CredHandle   credentials;
	CtxtHandle   context;
	std::wstring spn;
};


class SSPINTLMProvider
{
public:
	SSPINTLMProvider():
		_securityLib("security.dll"),
		_pSecFunTable(0)
	{
		InitSecurityInterfaceW pInitSecurityInterface = reinterpret_cast<InitSecurityInterfaceW>(_securityLib.getSymbol("InitSecurityInterfaceW"));
		if (pInitSecurityInterface)
		{
			_pSecFunTable = pInitSecurityInterface();
		}
		if (!_pSecFunTable) throw Poco::SystemException("Failed to initialize SSPI");
	}

	~SSPINTLMProvider()
	{
	}

	bool available()
	{
		PSecPkgInfoW pSecPkgInfo;
		SECURITY_STATUS status = _pSecFunTable->QuerySecurityPackageInfoW(L"NTLM", &pSecPkgInfo);
		if (status == SEC_E_OK)
		{
			_pSecFunTable->FreeContextBuffer(pSecPkgInfo);
			return true;
		}
		else return false;
	}

	Poco::SharedPtr<NTLMContext> createNTLMContext(const std::string& host, const std::string& service)
	{
		PSecPkgInfoW pSecPkgInfo;
		SECURITY_STATUS status = _pSecFunTable->QuerySecurityPackageInfoW(L"NTLM", &pSecPkgInfo);
		if (status != SEC_E_OK) throw Poco::SystemException("NTLM SSPI not available", status);

		std::size_t maxTokenSize = pSecPkgInfo->cbMaxToken;
		_pSecFunTable->FreeContextBuffer(pSecPkgInfo);

		Poco::SharedPtr<NTLMContext> pContext = new NTLMContext(new NTLMContextImpl);
		pContext->_pImpl->maxTokenSize = maxTokenSize;

		TimeStamp expiry;
		status = _pSecFunTable->AcquireCredentialsHandleW(
			NULL,
			L"NTLM",
			SECPKG_CRED_OUTBOUND,
			NULL,
			NULL,
			NULL,
			NULL,
			&pContext->_pImpl->credentials,
			&expiry);

		if (status != SEC_E_OK) throw Poco::SystemException("Failed to acquire NTLM credentials", status);

		std::string spn = service;
		spn += '/';
		spn += host;
		Poco::UnicodeConverter::convert(spn, pContext->_pImpl->spn);

		return pContext;
	}

	std::vector<unsigned char> negotiate(NTLMContext& context)
	{
		std::vector<unsigned char> buffer(context._pImpl->maxTokenSize);

		SecBuffer msgBuffer;
		msgBuffer.BufferType = SECBUFFER_TOKEN;
		msgBuffer.pvBuffer   = &buffer[0];
		msgBuffer.cbBuffer   = static_cast<unsigned long>(buffer.size());

		SecBufferDesc msgBufferDesc;
		msgBufferDesc.ulVersion = SECBUFFER_VERSION;
		msgBufferDesc.cBuffers  = 1;
		msgBufferDesc.pBuffers  = &msgBuffer;

		unsigned long attrs;
		TimeStamp expiry;
		SECURITY_STATUS status = _pSecFunTable->InitializeSecurityContextW(
			&context._pImpl->credentials,
			NULL,
			const_cast<SEC_WCHAR*>(context._pImpl->spn.c_str()),
			0,
			0,
			SECURITY_NETWORK_DREP,
			NULL,
			0,
			&context._pImpl->context,
			&msgBufferDesc,
			&attrs,
			&expiry);

		if (status == SEC_I_COMPLETE_NEEDED || status == SEC_I_COMPLETE_AND_CONTINUE)
		{
			_pSecFunTable->CompleteAuthToken(&context._pImpl->context, &msgBufferDesc);
		}
		else if (status != SEC_E_OK && status != SEC_I_CONTINUE_NEEDED)
		{
			throw Poco::SystemException("Failed to initialize NTLM security context", status);
		}

		buffer.resize(msgBuffer.cbBuffer);
		return buffer;
	}

	std::vector<unsigned char> authenticate(NTLMContext& context, const std::vector<unsigned char>& challenge)
	{
		std::vector<unsigned char> response(context._pImpl->maxTokenSize);

		SecBuffer responseBuffer;
		responseBuffer.BufferType = SECBUFFER_TOKEN;
		responseBuffer.pvBuffer   = &response[0];
		responseBuffer.cbBuffer   = static_cast<unsigned long>(response.size());

		SecBufferDesc responseBufferDesc;
		responseBufferDesc.ulVersion = SECBUFFER_VERSION;
		responseBufferDesc.cBuffers  = 1;
		responseBufferDesc.pBuffers  = &responseBuffer;

		SecBuffer challengeBuffer;
		challengeBuffer.BufferType = SECBUFFER_TOKEN;
		challengeBuffer.pvBuffer   = const_cast<unsigned char*>(&challenge[0]);
		challengeBuffer.cbBuffer   = static_cast<unsigned long>(challenge.size());

		SecBufferDesc challengeBufferDesc;
		challengeBufferDesc.ulVersion = SECBUFFER_VERSION;
		challengeBufferDesc.cBuffers  = 1;
		challengeBufferDesc.pBuffers  = &challengeBuffer;

		unsigned long attrs;
		TimeStamp expiry;
		SECURITY_STATUS status = _pSecFunTable->InitializeSecurityContextW(
			&context._pImpl->credentials,
			&context._pImpl->context,
			const_cast<SEC_WCHAR*>(context._pImpl->spn.c_str()),
			0,
			0,
			SECURITY_NETWORK_DREP,
			&challengeBufferDesc,
			0,
			&context._pImpl->context,
			&responseBufferDesc,
			&attrs,
			&expiry);

		if (status != SEC_E_OK)
		{
			throw Poco::SystemException("Failed to create NTLM authenticate message", status);
		}

		response.resize(responseBuffer.cbBuffer);
		return response;
	}

	void clearNTLMContext(NTLMContext& ctx)
	{
		if (SecIsValidHandle(&ctx._pImpl->context))
		{
			_pSecFunTable->DeleteSecurityContext(&ctx._pImpl->context);
		}
		if (SecIsValidHandle(&ctx._pImpl->credentials))
		{
			_pSecFunTable->FreeCredentialsHandle(&ctx._pImpl->credentials);
		}
	}

	static SSPINTLMProvider& instance();

private:
	typedef PSecurityFunctionTableW(APIENTRY *InitSecurityInterfaceW)(VOID);

	Poco::SharedLibrary _securityLib;
	PSecurityFunctionTableW _pSecFunTable;
};


SSPINTLMProvider& SSPINTLMProvider::instance()
{
	static SSPINTLMProvider p;
	return p;
}


} } // namespace Poco::Net


#endif // POCO_OS == POCO_OS_WINDOWS_NT


namespace Poco {
namespace Net {


const std::string SSPINTLMCredentials::SERVICE_HTTP("HTTP");
const std::string SSPINTLMCredentials::SERVICE_SMTP("SMTP");


NTLMContext::NTLMContext(NTLMContextImpl* pImpl):
	_pImpl(pImpl)
{
}


NTLMContext::~NTLMContext()
{
#if POCO_OS == POCO_OS_WINDOWS_NT
	SSPINTLMProvider::instance().clearNTLMContext(*this);
	delete _pImpl;
#endif
}


bool SSPINTLMCredentials::available()
{
#if POCO_OS == POCO_OS_WINDOWS_NT
	try
	{
		return SSPINTLMProvider::instance().available();
	}
	catch (...)
	{
		return false;
	}
#else
	return false;
#endif
}


Poco::SharedPtr<NTLMContext> SSPINTLMCredentials::createNTLMContext(const std::string& workstation, const std::string& service)
{
#if POCO_OS == POCO_OS_WINDOWS_NT
	return SSPINTLMProvider::instance().createNTLMContext(workstation, service);
#else
	throw Poco::NotImplementedException("SSPINTLMCredentials::createNTLMContext() is only available on Windows");
#endif
}


std::vector<unsigned char> SSPINTLMCredentials::negotiate(NTLMContext& context)
{
#if POCO_OS == POCO_OS_WINDOWS_NT
	return SSPINTLMProvider::instance().negotiate(context);
#else
	throw Poco::NotImplementedException("SSPINTLMCredentials::negotiate() is only available on Windows");
#endif
}


std::vector<unsigned char> SSPINTLMCredentials::authenticate(NTLMContext& context, const std::vector<unsigned char>& challenge)
{
#if POCO_OS == POCO_OS_WINDOWS_NT
	return SSPINTLMProvider::instance().authenticate(context, challenge);
#else
	throw Poco::NotImplementedException("SSPINTLMCredentials::authenticate() is only available on Windows");
#endif
}


} } // namespace Poco::Net
