//
// SchannelPeer.cpp
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "SchannelPeer.h"
#include "Poco/Net/AutoSecBufferDesc.h"
#include "Poco/Net/SSLException.h"
#include "Poco/Net/Utility.h"
#include "Poco/Exception.h"
#include <algorithm>


using Poco::Net::AutoSecBufferDesc;
using Poco::Net::SSLException;
using Poco::Net::Utility;


namespace
{
	constexpr std::size_t RECORD_HEADER_SIZE = 5;
	// RFC 5246 6.2.3: the fragment of a TLSCiphertext is at most 2^14 + 2048 bytes
	constexpr std::size_t MAX_RECORD_LENGTH = 16384 + 2048;
	constexpr int READ_SIZE = 32768;

	constexpr DWORD CLIENT_FLAGS = ISC_REQ_SEQUENCE_DETECT
		| ISC_REQ_REPLAY_DETECT
		| ISC_REQ_CONFIDENTIALITY
		| ISC_REQ_ALLOCATE_MEMORY
		| ISC_REQ_STREAM
		| ISC_REQ_MANUAL_CRED_VALIDATION
		| ISC_REQ_USE_SUPPLIED_CREDS;

	constexpr DWORD SERVER_FLAGS = ASC_REQ_SEQUENCE_DETECT
		| ASC_REQ_REPLAY_DETECT
		| ASC_REQ_CONFIDENTIALITY
		| ASC_REQ_EXTENDED_ERROR
		| ASC_REQ_ALLOCATE_MEMORY
		| ASC_REQ_STREAM;
}


SchannelPeer::SchannelPeer(Role role, const Poco::Net::StreamSocket& socket, Poco::Net::Context::Ptr pContext):
	_role(role),
	_socket(socket),
	_pContext(pContext),
	_pSecurityFunctions(InitSecurityInterfaceW())
{
	if (_pSecurityFunctions == nullptr)
		throw SSLException("InitSecurityInterfaceW() failed");
	SecInvalidateHandle(&_hContext);
	// The Context loads its certificate only when asked for it, and the credentials it
	// acquires on their first use are cached with the certificate loaded by then.
	if (_role == SERVER)
		static_cast<void>(_pContext->certificate());
}


SchannelPeer::~SchannelPeer()
{
	if (SecIsValidHandle(&_hContext))
		_pSecurityFunctions->DeleteSecurityContext(&_hContext);
}


std::string SchannelPeer::handshake(bool sendFinalRecords)
{
	if (_role == CLIENT)
	{
		std::string clientHello;
		const SECURITY_STATUS status = handshakeCall(std::string(), clientHello);
		if (status != SEC_I_CONTINUE_NEEDED)
			throw SSLException("Handshake failure", Utility::formatError(status));
		send(clientHello);
	}
	return handshakeLoop(sendFinalRecords || _role == CLIENT);
}


std::string SchannelPeer::encrypt(const std::string& data)
{
	SecPkgContext_StreamSizes sizes;
	SECURITY_STATUS status = _pSecurityFunctions->QueryContextAttributesW(&_hContext, SECPKG_ATTR_STREAM_SIZES, &sizes);
	if (status != SEC_E_OK)
		throw SSLException("Failed to query stream sizes", Utility::formatError(status));

	std::string records;
	std::size_t offset = 0;
	do
	{
		const std::size_t length = std::min<std::size_t>(data.size() - offset, sizes.cbMaximumMessage);
		std::string record(sizes.cbHeader + length + sizes.cbTrailer, '\0');
		record.replace(sizes.cbHeader, length, data, offset, length);

		AutoSecBufferDesc<4> message(_pSecurityFunctions, false);
		message.setSecBufferStreamHeader(0, record.data(), static_cast<int>(sizes.cbHeader));
		message.setSecBufferData(1, record.data() + sizes.cbHeader, static_cast<int>(length));
		message.setSecBufferStreamTrailer(2, record.data() + sizes.cbHeader + length, static_cast<int>(sizes.cbTrailer));
		message.setSecBufferEmpty(3);
		status = _pSecurityFunctions->EncryptMessage(&_hContext, 0, &message, 0);
		if (status != SEC_E_OK)
			throw SSLException("Failed to encrypt message", Utility::formatError(status));

		records.append(record, 0, message[0].cbBuffer + message[1].cbBuffer + message[2].cbBuffer);
		offset += length;
	}
	while (offset < data.size());
	return records;
}


std::string SchannelPeer::closeNotify()
{
	return applyControlToken(SCHANNEL_SHUTDOWN);
}


void SchannelPeer::send(const std::string& bytes)
{
	std::size_t offset = 0;
	while (offset < bytes.size())
	{
		const int sent = _socket.sendBytes(bytes.data() + offset, static_cast<int>(bytes.size() - offset));
		if (sent <= 0)
			throw Poco::IOException("Failed to send to the peer");
		offset += sent;
	}
}


std::string SchannelPeer::receive()
{
	for (;;)
	{
		if (_input.empty() && !fill())
			return std::string();

		std::string buffer(_input);
		AutoSecBufferDesc<4> message(_pSecurityFunctions, false);
		message.setSecBufferData(0, buffer.data(), static_cast<int>(buffer.size()));
		message.setSecBufferEmpty(1);
		message.setSecBufferEmpty(2);
		message.setSecBufferEmpty(3);
		const SECURITY_STATUS status = _pSecurityFunctions->DecryptMessage(&_hContext, &message, 0, nullptr);

		if (status == SEC_E_INCOMPLETE_MESSAGE)
		{
			if (!fill())
				return std::string();
		}
		else if (status == SEC_E_OK || status == SEC_I_RENEGOTIATE)
		{
			std::string data;
			_input.clear();
			for (int i = 1; i < 4; ++i)
			{
				const SecBuffer& secBuffer = message[i];
				if (secBuffer.pvBuffer == nullptr)
					continue;
				if (secBuffer.BufferType == SECBUFFER_DATA)
					data.assign(static_cast<const char*>(secBuffer.pvBuffer), secBuffer.cbBuffer);
				else if (secBuffer.BufferType == SECBUFFER_EXTRA)
					_input.assign(static_cast<const char*>(secBuffer.pvBuffer), secBuffer.cbBuffer);
			}
			// the extra bytes of a post-handshake message start with that message
			if (status == SEC_I_RENEGOTIATE)
				handshakeLoop(true);
			if (!data.empty())
				return data;
		}
		else if (status == SEC_I_CONTEXT_EXPIRED)
		{
			return std::string();
		}
		else
		{
			throw SSLException("Failed to decrypt message", Utility::formatError(status));
		}
	}
}


SECURITY_STATUS SchannelPeer::handshakeCall(const std::string& input, std::string& output)
{
	std::string token(input);
	AutoSecBufferDesc<2> inBuffer(_pSecurityFunctions, false);
	inBuffer.setSecBufferToken(0, token.data(), static_cast<int>(token.size()));
	inBuffer.setSecBufferEmpty(1);
	AutoSecBufferDesc<1> outBuffer(_pSecurityFunctions, true);
	outBuffer.setSecBufferToken(0, nullptr, 0);
	PSecBufferDesc pInput = input.empty() ? nullptr : &inBuffer;
	const bool contextExists = SecIsValidHandle(&_hContext);

	DWORD outFlags = 0;
	TimeStamp expiry;
	SECURITY_STATUS status;
	if (_role == CLIENT)
	{
		SEC_WCHAR targetName[] = L"localhost";
		status = _pSecurityFunctions->InitializeSecurityContextW(
			&_pContext->credentials(),
			contextExists ? &_hContext : nullptr,
			targetName,
			CLIENT_FLAGS,
			0,
			0,
			pInput,
			0,
			contextExists ? nullptr : &_hContext,
			&outBuffer,
			&outFlags,
			&expiry);
	}
	else
	{
		status = _pSecurityFunctions->AcceptSecurityContext(
			&_pContext->credentials(),
			contextExists ? &_hContext : nullptr,
			pInput,
			SERVER_FLAGS,
			0,
			contextExists ? nullptr : &_hContext,
			&outBuffer,
			&outFlags,
			&expiry);
	}

	if (outBuffer[0].pvBuffer != nullptr)
		output.append(static_cast<const char*>(outBuffer[0].pvBuffer), outBuffer[0].cbBuffer);

	// Schannel reports unconsumed input by BufferType and cbBuffer only: it is the end of the input.
	if (status == SEC_E_INCOMPLETE_MESSAGE)
	{
		_input.insert(0, input);
	}
	else if (inBuffer[1].BufferType == SECBUFFER_EXTRA && inBuffer[1].cbBuffer > 0)
	{
		if (inBuffer[1].cbBuffer > input.size())
			throw SSLException("Invalid extra buffer length from Schannel");
		_input.insert(0, input, input.size() - inBuffer[1].cbBuffer, inBuffer[1].cbBuffer);
	}
	return status;
}


std::string SchannelPeer::handshakeLoop(bool sendFinal)
{
	bool needData = false;
	for (;;)
	{
		while (needData || !inputEndsOnRecordBoundary())
		{
			if (!fill())
				throw SSLException("The peer closed the connection during the handshake");
			needData = false;
		}

		std::string input;
		input.swap(_input);
		std::string output;
		const SECURITY_STATUS status = handshakeCall(input, output);
		if (status == SEC_E_OK)
		{
			if (!sendFinal)
				return output;
			send(output);
			return std::string();
		}
		else if (status == SEC_I_CONTINUE_NEEDED)
		{
			send(output);
		}
		else if (status != SEC_E_INCOMPLETE_MESSAGE)
		{
			throw SSLException("Handshake failure", Utility::formatError(status));
		}
		// input that Schannel did not consume at all is not offered again without more data
		needData = _input.size() >= input.size();
	}
}


std::string SchannelPeer::applyControlToken(DWORD token)
{
	AutoSecBufferDesc<1> tokenBuffer(_pSecurityFunctions, false);
	tokenBuffer.setSecBufferToken(0, &token, static_cast<int>(sizeof(token)));
	SECURITY_STATUS status = _pSecurityFunctions->ApplyControlToken(&_hContext, &tokenBuffer);
	if (FAILED(status))
		throw SSLException("Control token refused", Utility::formatError(status));

	std::string output;
	status = handshakeCall(std::string(), output);
	if (FAILED(status))
		throw SSLException("Handshake call after the control token failed", Utility::formatError(status));
	return output;
}


bool SchannelPeer::fill()
{
	char buffer[READ_SIZE];
	const int n = _socket.receiveBytes(buffer, sizeof(buffer));
	if (n <= 0)
		return false;
	_input.append(buffer, n);
	return true;
}


bool SchannelPeer::inputEndsOnRecordBoundary() const
{
	std::size_t offset = 0;
	while (offset + RECORD_HEADER_SIZE <= _input.size())
	{
		const auto* pHeader = reinterpret_cast<const unsigned char*>(_input.data() + offset);
		const std::size_t length = (static_cast<std::size_t>(pHeader[3]) << 8) | pHeader[4];
		if (pHeader[0] < 0x14 || pHeader[0] > 0x17 || pHeader[1] != 0x03 || length > MAX_RECORD_LENGTH)
			return true;
		offset += RECORD_HEADER_SIZE + length;
	}
	return offset > 0 && offset == _input.size();
}
