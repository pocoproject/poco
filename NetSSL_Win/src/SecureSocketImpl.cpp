//
// SecureSocketImpl.cpp
//
// Library: NetSSL_Win
// Package: SSLSockets
// Module:  SecureSocketImpl
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/SecureSocketImpl.h"
#include "Poco/Net/SSLException.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Net/Utility.h"
#include "Poco/Net/SecureStreamSocketImpl.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/StreamSocketImpl.h"
#include "Poco/Format.h"
#include "Poco/UnicodeConverter.h"
#include <wininet.h>
#include <cstring>


namespace Poco {
namespace Net {


class StateMachine
{
public:
	typedef void (SecureSocketImpl::*StateImpl)(void);

	StateMachine();
	~StateMachine();

	static StateMachine& instance();

	bool execute(SecureSocketImpl* pSock);

private:
	StateMachine(const StateMachine&) = delete;
	StateMachine& operator = (const StateMachine&) = delete;

	std::vector<StateImpl> _states;
};


SecureSocketImpl::SecureSocketImpl(Poco::AutoPtr<SocketImpl> pSocketImpl, Context::Ptr pContext):
	_pSocket(pSocketImpl),
	_pContext(pContext),
	_mode(pContext->isForServerUse() ? MODE_SERVER : MODE_CLIENT),
	_shutdownFlags(0),
	_clientAuthRequired(pContext->verificationMode() >= Context::VERIFY_STRICT),
	_securityFunctions(SSLManager::instance().securityFunctions()),
	_pOwnCertificate(0),
	_pPeerCertificate(0),
	_hCreds(),
	_hContext(),
	_contextFlags(0),
	_overflowBuffer(0),
	_sendBuffer(0),
	_sendBufferOffset(0),
	_sendBufferPending(0),
	_recvBuffer(IO_BUFFER_SIZE),
	_recvBufferOffset(0),
	_ioBufferSize(0),
	_streamSizes(),
	_outSecBuffer(&_securityFunctions, true),
	_inSecBuffer(&_securityFunctions, false),
	_extraSecBuffer(),
	_securityStatus(SEC_E_INCOMPLETE_MESSAGE),
	_state(ST_INITIAL),
	_needData(true),
	_needHandshake(false)
{
	_hCreds.dwLower = 0;
	_hCreds.dwUpper = 0;

	_hContext.dwLower = 0;
	_hContext.dwUpper = 0;

	_streamSizes.cbBlockSize      = 0;
	_streamSizes.cbHeader         = 0;
	_streamSizes.cbMaximumMessage = 0;
	_streamSizes.cbTrailer        = 0;

	_overflowBuffer.resize(0);

	initCommon();
}


SecureSocketImpl::~SecureSocketImpl()
{
	cleanup();
}


void SecureSocketImpl::initCommon()
{
	_contextFlags = ISC_REQ_SEQUENCE_DETECT
	              | ISC_REQ_REPLAY_DETECT
	              | ISC_REQ_CONFIDENTIALITY
	              | ISC_RET_EXTENDED_ERROR
	              | ISC_REQ_ALLOCATE_MEMORY
	              | ISC_REQ_STREAM;

	if (_pContext->verificationMode() == Context::VERIFY_NONE)
	{
		_contextFlags |= ISC_REQ_MANUAL_CRED_VALIDATION;
	}
	else if (_pContext->verificationMode() == Context::VERIFY_RELAXED)
	{
		_contextFlags |= ISC_REQ_INTEGRITY;
	}
	else if (_pContext->verificationMode() >= Context::VERIFY_STRICT)
	{
		_contextFlags |= ISC_REQ_INTEGRITY;
	}

	if (_pContext->verificationMode() == Context::VERIFY_RELAXED)
	{
		_contextFlags |= ISC_REQ_MANUAL_CRED_VALIDATION;
	}

	if (_mode == MODE_CLIENT)
	{
		// If we do not set this, in some cases the InitializeSecurityContext() will return SEC_I_INCOMPLETE_CREDENTIALS. 
		// That case is not handled in the handshake, it will try to continue reading, thus blocking until timeout.
		// The handling of this case would be to repeat the InitializeSecurityContext once again, or as we do here,
		// inform to use a client cert if available. 
		_contextFlags |= ISC_REQ_USE_SUPPLIED_CREDS;
	}
}


void SecureSocketImpl::cleanup()
{
	_peerHostName.clear();

	_hCreds.dwLower = 0;
	_hCreds.dwUpper = 0;

	if (_hContext.dwLower != 0 && _hContext.dwUpper != 0)
	{
		_securityFunctions.DeleteSecurityContext(&_hContext);
		_hContext.dwLower = 0;
		_hContext.dwUpper = 0;
	}

	if (_pOwnCertificate)
	{
		CertFreeCertificateContext(_pOwnCertificate);
		_pOwnCertificate = 0;
	}

	if (_pPeerCertificate)
	{
		CertFreeCertificateContext(_pPeerCertificate);
		_pPeerCertificate = 0;
	}

	_outSecBuffer.release();
	_inSecBuffer.release();

	_overflowBuffer.resize(0);
}


SocketImpl* SecureSocketImpl::acceptConnection(SocketAddress& clientAddr)
{
	StreamSocket ss = _pSocket->acceptConnection(clientAddr);
	Poco::AutoPtr<SecureStreamSocketImpl> pSecureStreamSocketImpl = new SecureStreamSocketImpl(static_cast<StreamSocketImpl*>(ss.impl()), _pContext);
	pSecureStreamSocketImpl->acceptSSL();
	pSecureStreamSocketImpl->duplicate();
	return pSecureStreamSocketImpl;
}


void SecureSocketImpl::connect(const SocketAddress& address, bool performHandshake)
{
	setState(ST_ERROR);
	_pSocket->connect(address);
	connectSSL(performHandshake);
	setState(ST_DONE);
}


void SecureSocketImpl::connect(const SocketAddress& address, const Poco::Timespan& timeout, bool performHandshake)
{
	setState(ST_ERROR);
	_pSocket->connect(address, timeout);
	connectSSL(performHandshake);
	setState(ST_DONE);
}


void SecureSocketImpl::connectNB(const SocketAddress& address)
{
	try
	{
		setState(ST_CONNECTING);
		_pSocket->connectNB(address);
	}
	catch (...)
	{
		setState(ST_ERROR);
	}
}


void SecureSocketImpl::bind(const SocketAddress& address, bool reuseAddress)
{
	poco_check_ptr (_pSocket);

	_pSocket->bind(address, reuseAddress);
}


void SecureSocketImpl::bind(const SocketAddress& address, bool reuseAddress, bool reusePort)
{
	poco_check_ptr (_pSocket);

	_pSocket->bind(address, reuseAddress, reusePort);
}


void SecureSocketImpl::bind6(const SocketAddress& address, bool reuseAddress, bool ipV6Only)
{
	poco_check_ptr (_pSocket);

	_pSocket->bind6(address, reuseAddress, ipV6Only);
}


void SecureSocketImpl::bind6(const SocketAddress& address, bool reuseAddress, bool reusePort, bool ipV6Only)
{
	poco_check_ptr (_pSocket);

	_pSocket->bind6(address, reuseAddress, reusePort, ipV6Only);
}


void SecureSocketImpl::listen(int backlog)
{
	_mode = MODE_SERVER;

	if (_hCreds.dwLower == 0 && _hCreds.dwUpper == 0)
	{
		initServerContext();
	}
	_pSocket->listen(backlog);
}


int SecureSocketImpl::shutdown()
{
	int rc = 0;
	if ((_shutdownFlags & TLS_SHUTDOWN_SENT) == 0)
	{
		if (_mode == MODE_SERVER)
		{
			rc = serverShutdown(&_hCreds, &_hContext);
		}
		else
		{
			rc = clientShutdown(&_hCreds, &_hContext);
		}
		if (rc >= 0)
		{
			_pSocket->shutdownSend();
			_shutdownFlags |= TLS_SHUTDOWN_SENT;
		}
	}
	return (_shutdownFlags & TLS_SHUTDOWN_RECEIVED) ? 1 : 0;
}


void SecureSocketImpl::close()
{
	try
	{
		shutdown();
	}
	catch (Poco::Exception&)
	{
	}
	_pSocket->close();
	cleanup();
}


void SecureSocketImpl::abort()
{
	_pSocket->shutdown();
	cleanup();
}


int SecureSocketImpl::available() const
{
	return static_cast<int>(_overflowBuffer.size() + _recvBufferOffset);
}


void SecureSocketImpl::acceptSSL()
{
	setState(ST_DONE);
	initServerContext();
	_needHandshake = true;
}


void SecureSocketImpl::verifyPeerCertificate()
{
	if (_peerHostName.empty())
		_peerHostName = _pSocket->peerAddress().host().toString();

	verifyPeerCertificate(_peerHostName);
}


void SecureSocketImpl::verifyPeerCertificate(const std::string& hostName)
{
	Context::VerificationMode mode = _pContext->verificationMode();
	if (mode == Context::VERIFY_NONE || !_pContext->extendedCertificateVerificationEnabled() ||
	    (mode != Context::VERIFY_STRICT && isLocalHost(hostName)))
	{
		return;
	}


	if (_mode == MODE_SERVER)
		serverVerifyCertificate();
	else
		clientVerifyCertificate(hostName);
}


bool SecureSocketImpl::isLocalHost(const std::string& hostName)
{
	SocketAddress addr(hostName, 0);
	return addr.host().isLoopback();
}


int SecureSocketImpl::sendRawBytes(const void* buffer, int length, int flags)
{
	return _pSocket->sendBytes(buffer, length, flags);
}


int SecureSocketImpl::receiveRawBytes(void* buffer, int length, int flags)
{
	return _pSocket->receiveBytes(buffer, length, flags);
}


int SecureSocketImpl::sendBytes(const void* buffer, int length, int flags)
{
	if (_needHandshake)
	{
		completeHandshake();
		_needHandshake = false;
	}

	if (_state == ST_ERROR) return 0;

	if (_state != ST_DONE)
	{
		bool establish = _pSocket->getBlocking();
		if (establish)
		{
			while (_state != ST_DONE)
			{
				stateMachine();
			}
		}
		else
		{
			stateMachine();
			if (_state != ST_DONE) return -1;
		}
	}

	if (_sendBufferPending > 0)
	{
		int sent = sendRawBytes(_sendBuffer.begin() + _sendBufferOffset, _sendBufferPending, flags);
		if (sent > 0)
		{
			_sendBufferOffset += sent;
			_sendBufferPending -= sent;
		}
		return _sendBufferPending == 0 ? length : -1;
	}

	int dataToSend = length;
	int dataSent = 0;
	const char* pBuffer = reinterpret_cast<const char*>(buffer);

	_sendBuffer.resize(length + 1024);
	_sendBufferOffset = 0;
	_sendBufferPending = 0;
	while (dataToSend > 0)
	{
		AutoSecBufferDesc<4> msg(&_securityFunctions, false);
		int dataSize = dataToSend;
		if (dataSize > _streamSizes.cbMaximumMessage)
			dataSize = _streamSizes.cbMaximumMessage;
		SecBuffer* pDataBuffer  = 0;
		SecBuffer* pExtraBuffer = 0;

		if (_sendBuffer.size() < _sendBufferPending + dataSize +  _streamSizes.cbHeader + _streamSizes.cbTrailer)
		{
			_sendBuffer.resize(_sendBufferPending + dataSize + _streamSizes.cbHeader + _streamSizes.cbTrailer, true);
		}

		std::memcpy(_sendBuffer.begin() + _sendBufferPending + _streamSizes.cbHeader, pBuffer + dataSent, dataSize);

		msg.setSecBufferStreamHeader(0, _sendBuffer.begin() + _sendBufferPending, _streamSizes.cbHeader);
		msg.setSecBufferData(1, _sendBuffer.begin() + _sendBufferPending + _streamSizes.cbHeader, dataSize);
		msg.setSecBufferStreamTrailer(2, _sendBuffer.begin() + _sendBufferPending + _streamSizes.cbHeader + dataSize, _streamSizes.cbTrailer);
		msg.setSecBufferEmpty(3);

		SECURITY_STATUS securityStatus = _securityFunctions.EncryptMessage(&_hContext, 0, &msg, 0);

		if (FAILED(securityStatus) && securityStatus != SEC_E_CONTEXT_EXPIRED)
			throw SSLException("Failed to encrypt message", Utility::formatError(securityStatus));

		poco_assert_dbg (_streamSizes.cbHeader == msg[0].cbBuffer);
		poco_assert_dbg (_streamSizes.cbTrailer == msg[2].cbBuffer);
		poco_assert_dbg (dataSize == msg[1].cbBuffer);

		_sendBufferPending += msg[0].cbBuffer + msg[1].cbBuffer + msg[2].cbBuffer;
		dataToSend -= dataSize;
		dataSent += dataSize;

		poco_assert (_sendBufferPending <= _sendBuffer.size());
	}

	int sent = sendRawBytes(_sendBuffer.begin(), _sendBufferPending, flags);
	if (sent >= 0)
	{
		_sendBufferOffset += sent;
		_sendBufferPending -= sent;
		if (_sendBufferPending > 0)
			return -1;
		else
			return dataSent;
	}
	else
	{
		return -1;
	}
}


int SecureSocketImpl::receiveBytes(void* buffer, int length, int flags)
{
	if (_needHandshake)
	{
		completeHandshake();
		_needHandshake = false;
	}

	if (_state == ST_ERROR) return 0;
	if (_state != ST_DONE)
	{
		bool establish = _pSocket->getBlocking();
		if (establish)
		{
			while (_state != ST_DONE)
			{
				stateMachine();
			}
		}
		else
		{
			stateMachine();
			if (_state != ST_DONE) return -1;
		}
	}

	int rc(0);
	std::size_t overflowSize = _overflowBuffer.size();
	if (overflowSize > 0) // any remaining data from previous calls?
	{
		if (static_cast<DWORD>(length) < overflowSize)
		{
			rc = length;
			std::memcpy(buffer, _overflowBuffer.begin(), rc);
			if ((flags & MSG_PEEK) == 0)
			{
				std::memmove(_overflowBuffer.begin(), _overflowBuffer.begin() + rc, overflowSize - rc);
				_overflowBuffer.resize(overflowSize - rc);
			}
		}
		else
		{
			rc = static_cast<int>(overflowSize);
			std::memcpy(buffer, _overflowBuffer.begin(), rc);
			if ((flags & MSG_PEEK) == 0)
			{
				_overflowBuffer.resize(0);
			}
		}
	}
	else
	{
		// adjust buffer size to optimize memory usage
		if (_ioBufferSize != _recvBuffer.capacity() && _recvBufferOffset < _ioBufferSize)
		{
			_recvBuffer.setCapacity(_ioBufferSize);
		}
		bool needData = _recvBufferOffset == 0;
		bool cont = true;
		do
		{
			if (needData)
			{
				int numBytes = receiveRawBytes(_recvBuffer.begin() + _recvBufferOffset, _ioBufferSize - _recvBufferOffset);

				if (numBytes == -1)
					return -1;
				else if (numBytes == 0)
					break;
				else
					_recvBufferOffset += numBytes;
			}
			else needData = true;

			int bytesDecoded = 0;
			_extraSecBuffer.pvBuffer = 0;
			_extraSecBuffer.cbBuffer = 0;
			SECURITY_STATUS securityStatus = decodeBufferFull(_recvBuffer.begin(), _recvBufferOffset, reinterpret_cast<char*>(buffer), length, bytesDecoded);
			if (_extraSecBuffer.cbBuffer > 0)
			{
				std::memmove(_recvBuffer.begin(), _extraSecBuffer.pvBuffer, _extraSecBuffer.cbBuffer);
				_recvBufferOffset = _extraSecBuffer.cbBuffer;
			}
			else
			{
				_recvBufferOffset = 0;
				cont = false;
			}

			_extraSecBuffer.pvBuffer = 0;
			_extraSecBuffer.cbBuffer = 0;

			if (bytesDecoded > 0)
			{
				// bytesDecoded contains everything including overflow data
				rc = bytesDecoded;
				if (rc > length)
					rc = length;

				if (flags & MSG_PEEK)
				{
					// prepend returned data to overflow buffer
					std::size_t oldSize = _overflowBuffer.size();
					_overflowBuffer.resize(oldSize + rc);
					std::memmove(_overflowBuffer.begin() + rc, _overflowBuffer.begin(), oldSize);
					std::memcpy(_overflowBuffer.begin(), buffer, rc);
				}

				return rc;
			}

			if (securityStatus == SEC_E_INCOMPLETE_MESSAGE)
			{
				continue;
			}

			if (securityStatus == SEC_I_CONTEXT_EXPIRED)
			{
				_shutdownFlags |= TLS_SHUTDOWN_RECEIVED;
				break;
			}

			if (securityStatus != SEC_E_OK && securityStatus != SEC_I_RENEGOTIATE)
			{
				break;
			}

			if (securityStatus == SEC_I_RENEGOTIATE)
			{
				_needData = false;
				setState(ST_CLIENT_HSK_LOOP_INIT);
				if (!_pSocket->getBlocking())
					return -1;

				securityStatus = performClientHandshakeLoop();

				if (securityStatus != SEC_E_OK)
					break;

				if (_extraSecBuffer.pvBuffer)
				{
					std::memmove(_recvBuffer.begin(), _extraSecBuffer.pvBuffer, _extraSecBuffer.cbBuffer);
					_recvBufferOffset = _extraSecBuffer.cbBuffer;
				}

				_extraSecBuffer.pvBuffer = 0;
				_extraSecBuffer.cbBuffer = 0;
			}
		}
		while (cont);
	}

	return rc;
}


SECURITY_STATUS SecureSocketImpl::decodeMessage(BYTE* pBuffer, DWORD bufSize, AutoSecBufferDesc<4>& msg, SecBuffer*& pDataBuffer, SecBuffer*& pExtraBuffer)
{
	msg.setSecBufferData(0, pBuffer, bufSize);
	msg.setSecBufferEmpty(1);
	msg.setSecBufferEmpty(2);
	msg.setSecBufferEmpty(3);
	pDataBuffer  = 0;
	pExtraBuffer = 0;

	SECURITY_STATUS securityStatus = _securityFunctions.DecryptMessage(&_hContext, &msg, 0, 0);
	// TODO: when decrypting the close_notify alert, returns SEC_E_DECRYPT_FAILURE

	if (securityStatus == SEC_E_OK || securityStatus == SEC_I_RENEGOTIATE)
	{
		for (int i = 1; i < 4; ++i)
		{
			if (!pDataBuffer && msg[i].BufferType == SECBUFFER_DATA)
				pDataBuffer = &msg[i];

			if (!pExtraBuffer && msg[i].BufferType == SECBUFFER_EXTRA)
				pExtraBuffer = &msg[i];
		}
	}

	return securityStatus;
}


SECURITY_STATUS SecureSocketImpl::decodeBufferFull(BYTE* pBuffer, DWORD bufSize, char* pOutBuffer, int outLength, int& bytesDecoded)
{
	poco_check_ptr (pBuffer);
	poco_assert (bufSize > 0);
	poco_check_ptr (pOutBuffer);
	poco_assert (outLength > 0);

	_extraSecBuffer.pvBuffer = 0;
	_extraSecBuffer.cbBuffer = 0;

	SecBuffer* pDataBuffer = 0;
	SecBuffer* pExtraBuffer = 0;
	int bytes = 0;
	bytesDecoded = 0;

	Poco::Buffer<BYTE> overflowBuffer(0);
	int overflowOffset = 0;
	SECURITY_STATUS securityStatus = SEC_E_OK;
	do
	{
		AutoSecBufferDesc<4> msg(&_securityFunctions, false);
		securityStatus = decodeMessage(pBuffer, bufSize, msg, pDataBuffer, pExtraBuffer);
		if (pDataBuffer && pDataBuffer->cbBuffer > 0)
		{
			bytes = pDataBuffer->cbBuffer;
			bytesDecoded += bytes;
			// do we have room for more data in pOutBuffer?
			if (bytes <= outLength) // yes, everything fits in there
			{
				outLength -= bytes;
				std::memcpy(pOutBuffer, pDataBuffer->pvBuffer, bytes);
				pOutBuffer += bytes;
			}
			else
			{
				// not enough room in pOutBuffer, write overflow data
				// simply reserve bufSize bytes (is large enough even in worst case scenario, no need to re-increase)
				overflowBuffer.resize(bufSize);
				if (outLength > 0)
				{
					// make pOutBuffer full
					std::memcpy(pOutBuffer, pDataBuffer->pvBuffer, outLength);
					// no longer valid to write to pOutBuffer
					pOutBuffer = 0;
					// copy the rest to ptrOverflow
					std::memcpy(overflowBuffer.begin(), reinterpret_cast<char*>(pDataBuffer->pvBuffer) + outLength, bytes - outLength);
					overflowOffset = bytes - outLength;
					outLength = 0;
				}
				else
				{
					// append to overflowBuffer
					poco_assert_dbg (overflowOffset + bytes <= overflowBuffer.capacity());
					std::memcpy(overflowBuffer.begin() + overflowOffset, pDataBuffer->pvBuffer, bytes);
					overflowOffset += bytes;
				}
			}
		}
		if (pExtraBuffer && pExtraBuffer->cbBuffer > 0)
		{
			// we have potentially more data to decode
			// decode as much as possible
			pBuffer = reinterpret_cast<BYTE*>(pExtraBuffer->pvBuffer);
			bufSize = pExtraBuffer->cbBuffer;
		}
		else
		{
			if (securityStatus == SEC_E_OK)
			{
				// everything decoded
				pBuffer = 0;
				bufSize = 0;
			}
			else if (securityStatus != SEC_E_INCOMPLETE_MESSAGE && securityStatus != SEC_I_RENEGOTIATE && securityStatus != SEC_I_CONTEXT_EXPIRED)
			{
				return securityStatus;
			}
		}

		if (securityStatus == SEC_I_RENEGOTIATE)
		{
			_needData = false;
			securityStatus = performClientHandshakeLoop();
			if (securityStatus != SEC_E_OK)
				break;
		}
	}
	while (securityStatus == SEC_E_OK && pBuffer);

	if (overflowOffset > 0)
	{
		_overflowBuffer.resize(overflowOffset);
		std::memcpy(_overflowBuffer.begin(), overflowBuffer.begin(), overflowOffset);
	}
	if (bufSize > 0)
	{
		_extraSecBuffer.cbBuffer = bufSize;
		_extraSecBuffer.pvBuffer = pBuffer;
	}

	if (pBuffer == 0) securityStatus = SEC_E_OK;
	return securityStatus;
}


void SecureSocketImpl::setPeerHostName(const std::string& peerHostName)
{
	_peerHostName = peerHostName;
}


PCCERT_CONTEXT SecureSocketImpl::loadCertificate(bool mustFindCertificate)
{
	try
	{
		Poco::Net::X509Certificate cert = _pContext->certificate();
		PCCERT_CONTEXT pCert = cert.system();
		CertDuplicateCertificateContext(pCert);
		return pCert;
	}
	catch (...)
	{
		if (mustFindCertificate)
			throw;
		else
			return 0;
	}
}


void SecureSocketImpl::connectSSL(bool completeHandshake)
{
	poco_assert_dbg(_pPeerCertificate == 0);

	if (_peerHostName.empty())
	{
		_peerHostName = _pSocket->peerAddress().host().toString();
	}

	initClientContext();
	if (completeHandshake)
	{
		performClientHandshake();
		_needHandshake = false;
	}
	else
	{
		_needHandshake = true;
	}
}


void SecureSocketImpl::completeHandshake()
{
	if (_mode == MODE_SERVER)
		performServerHandshake();
	else
		performClientHandshake();
}


void SecureSocketImpl::clientConnectVerify()
{
	poco_assert_dbg(!_pPeerCertificate);
	poco_assert_dbg(!_peerHostName.empty());

	try
	{
		SECURITY_STATUS securityStatus = _securityFunctions.QueryContextAttributesW(&_hContext, SECPKG_ATTR_REMOTE_CERT_CONTEXT, (PVOID) &_pPeerCertificate);
		if (securityStatus != SEC_E_OK)
			throw SSLException("Failed to obtain peer certificate", Utility::formatError(securityStatus));

		clientVerifyCertificate(_peerHostName);

		securityStatus = _securityFunctions.QueryContextAttributesW(&_hContext, SECPKG_ATTR_STREAM_SIZES, &_streamSizes);
		if (securityStatus != SEC_E_OK)
			throw SSLException("Failed to query stream sizes", Utility::formatError(securityStatus));

		_ioBufferSize = _streamSizes.cbHeader + _streamSizes.cbMaximumMessage + _streamSizes.cbTrailer;
		setState(ST_DONE);
	}
	catch (...)
	{
		if (_pPeerCertificate)
		{
			CertFreeCertificateContext(_pPeerCertificate);
			_pPeerCertificate = 0;
		}
		throw;
	}
}


void SecureSocketImpl::initClientContext()
{
	_pOwnCertificate = loadCertificate(false);
	_hCreds = _pContext->credentials();
}


void SecureSocketImpl::performClientHandshake()
{
	performInitialClientHandshake();
	performClientHandshakeSendToken();
	performClientHandshakeLoop();
	clientConnectVerify();
}


void SecureSocketImpl::performInitialClientHandshake()
{
	// get initial security token
	_outSecBuffer.reset(true);
	_outSecBuffer.setSecBufferToken(0, 0, 0);

	TimeStamp ts;
	DWORD contextAttributes(0);
	std::wstring whostName;
	Poco::UnicodeConverter::convert(_peerHostName, whostName);
	_securityStatus = _securityFunctions.InitializeSecurityContextW(
						&_hCreds,
						0,
						const_cast<SEC_WCHAR*>(whostName.c_str()),
						_contextFlags,
						0,
						0,
						0,
						0,
						&_hContext,
						&_outSecBuffer,
						&contextAttributes,
						&ts);

	if (_securityStatus != SEC_E_OK)
	{
		if (_securityStatus == SEC_I_INCOMPLETE_CREDENTIALS)
		{
			// the server is asking for client credentials, we didn't send one because we were not configured to do so, abort
			throw SSLException("Handshake failed: No client credentials configured");
		}
		else if (_securityStatus != SEC_I_CONTINUE_NEEDED)
		{
			throw SSLException("Handshake failed", Utility::formatError(_securityStatus));
		}
	}

	_extraSecBuffer.pvBuffer = 0;
	_extraSecBuffer.cbBuffer = 0;
	_needData = true;

	if (_outSecBuffer[0].cbBuffer && _outSecBuffer[0].pvBuffer)
	{
		setState(ST_CLIENT_HSK_SEND_TOKEN);
	}
	else if (_securityStatus == SEC_E_OK)
	{
		setState(ST_DONE);
	}
}


void SecureSocketImpl::performClientHandshakeSendToken()
{
	poco_assert_dbg (_outSecBuffer[0].cbBuffer && _outSecBuffer[0].pvBuffer);

	int sent = sendRawBytes(_outSecBuffer[0].pvBuffer, _outSecBuffer[0].cbBuffer);
	if (sent < 0)
	{
		return;
	}
	else if (sent < _outSecBuffer[0].cbBuffer)
	{
		_outSecBuffer[0].cbBuffer -= sent;
		std::memmove(_outSecBuffer[0].pvBuffer, reinterpret_cast<char*>(_outSecBuffer[0].pvBuffer) + sent, _outSecBuffer[0].cbBuffer);
		return;
	}

	if (_securityStatus == SEC_E_OK)
	{
		// The security context was successfully initialized.
		// There is no need for another InitializeSecurityContext (Schannel) call.
		setState(ST_DONE);
	}
	else
	{
		setState(ST_CLIENT_HSK_LOOP_INIT);
		_securityStatus = SEC_E_INCOMPLETE_MESSAGE;
	}
}


SECURITY_STATUS SecureSocketImpl::performClientHandshakeLoop()
{
	_recvBufferOffset = 0;
	_securityStatus = SEC_E_INCOMPLETE_MESSAGE;

	while (_securityStatus == SEC_I_CONTINUE_NEEDED || _securityStatus == SEC_E_INCOMPLETE_MESSAGE || _securityStatus == SEC_I_INCOMPLETE_CREDENTIALS)
	{
		performClientHandshakeLoopCondReceive();

		if (_securityStatus == SEC_E_OK)
		{
			performClientHandshakeLoopDone();
			performClientHandshakeSendFinal();
		}
		else if (_securityStatus == SEC_I_CONTINUE_NEEDED)
		{
			performClientHandshakeLoopContinue();
			performClientHandshakeLoopSend();
		}
		else if (_securityStatus == SEC_E_INCOMPLETE_MESSAGE)
		{
			performClientHandshakeLoopIncompleteMessage();
		}
		else if (FAILED(_securityStatus))
		{
			if (_outFlags & ISC_RET_EXTENDED_ERROR)
			{
				performClientHandshakeSendError();
			}
			else
			{
				performClientHandshakeError();
			}
		}
		else
		{
			performClientHandshakeLoopIncompleteMessage();
		}
	}

	if (FAILED(_securityStatus))
	{
		performClientHandshakeError();
	}

	return _securityStatus;
}


void SecureSocketImpl::performClientHandshakeSendError()
{
	poco_assert_dbg (FAILED(_securityStatus));

	sendOutSecBufferAndAdvanceState(ST_ERROR);
}


void SecureSocketImpl::performClientHandshakeError()
{
	poco_assert_dbg (FAILED(_securityStatus));
	cleanup();
	setState(ST_ERROR);
	throw SSLException("Error during handshake", Utility::formatError(_securityStatus));
}


void SecureSocketImpl::performClientHandshakeExtraBuffer()
{
	if (_inSecBuffer[1].BufferType == SECBUFFER_EXTRA)
	{
		std::memmove(_recvBuffer.begin(), _recvBuffer.begin() + (_recvBufferOffset - _inSecBuffer[1].cbBuffer), _inSecBuffer[1].cbBuffer);
		_recvBufferOffset = _inSecBuffer[1].cbBuffer;
	}
	else _recvBufferOffset = 0;
}


void SecureSocketImpl::performClientHandshakeLoopDone()
{
	poco_assert_dbg(_securityStatus == SEC_E_OK);

	performClientHandshakeExtraBuffer();
	if (_outSecBuffer[0].cbBuffer && _outSecBuffer[0].pvBuffer)
	{
		setState(ST_CLIENT_HSK_SEND_FINAL);
	}
	else
	{
		setState(ST_CLIENT_VERIFY);
	}
}


void SecureSocketImpl::performClientHandshakeSendFinal()
{
	sendOutSecBufferAndAdvanceState(ST_CLIENT_VERIFY);
}


void SecureSocketImpl::performClientHandshakeLoopInit()
{
	poco_assert_dbg (_securityStatus == SEC_E_INCOMPLETE_MESSAGE || SEC_I_CONTINUE_NEEDED);

	_inSecBuffer.reset(false);
	_outSecBuffer.reset(true);

	if (_needData)
	{
		if (_recvBuffer.capacity() != IO_BUFFER_SIZE)
		{
			_recvBuffer.setCapacity(IO_BUFFER_SIZE);
		}
		setState(ST_CLIENT_HSK_LOOP_RECV);
	}
	else
	{
		setState(ST_CLIENT_HSK_LOOP_PROCESS);
	}
}


void SecureSocketImpl::performClientHandshakeLoopRecv()
{
	poco_assert_dbg (_needData);
	poco_assert (IO_BUFFER_SIZE > _recvBufferOffset);

	int n = receiveRawBytes(_recvBuffer.begin() + _recvBufferOffset, IO_BUFFER_SIZE - _recvBufferOffset);
	if (n == 0) throw SSLException("Connection unexpectedly closed during handshake");
	if (n > 0) 
	{
		_recvBufferOffset += n;
		setState(ST_CLIENT_HSK_LOOP_PROCESS);
	}
}


void SecureSocketImpl::performClientHandshakeLoopProcess()
{
	_inSecBuffer.setSecBufferToken(0, _recvBuffer.begin(), _recvBufferOffset);
	// inbuffer 1 should be empty
	_inSecBuffer.setSecBufferEmpty(1);

	// outBuffer[0] should be empty
	_outSecBuffer.setSecBufferToken(0, 0, 0);

	_outFlags = 0;
	TimeStamp ts;
	_securityStatus = _securityFunctions.InitializeSecurityContextW(
								&_hCreds,
								&_hContext,
								0,
								_contextFlags,
								0,
								0,
								&_inSecBuffer,
								0,
								0,
								&_outSecBuffer,
								&_outFlags,
								&ts);

	if (_securityStatus == SEC_E_OK)
	{
		setState(ST_CLIENT_HSK_LOOP_DONE);
	}
	else if (_securityStatus == SEC_I_CONTINUE_NEEDED)
	{
		setState(ST_CLIENT_HSK_LOOP_CONTINUE);
	}
	else if (FAILED(_securityStatus))
	{
		if (_outFlags & ISC_RET_EXTENDED_ERROR)
			setState(ST_CLIENT_HSK_SEND_ERROR);
		else
			setState(ST_ERROR);
	}
	else
	{
		setState(ST_CLIENT_HSK_LOOP_INCOMPLETE);
	}
}


void SecureSocketImpl::performClientHandshakeLoopCondReceive()
{
	poco_assert_dbg (_securityStatus == SEC_E_INCOMPLETE_MESSAGE || SEC_I_CONTINUE_NEEDED);

	performClientHandshakeLoopInit();

	if (_state == ST_CLIENT_HSK_LOOP_RECV)
	{
		performClientHandshakeLoopRecv();
	}

	performClientHandshakeLoopProcess();
}


void SecureSocketImpl::performClientHandshakeLoopContinue()
{
	performClientHandshakeExtraBuffer();
	setState(ST_CLIENT_HSK_LOOP_SEND);
}


void SecureSocketImpl::performClientHandshakeLoopSend()
{
	sendOutSecBufferAndAdvanceState(ST_CLIENT_HSK_LOOP_INIT);
}


void SecureSocketImpl::sendOutSecBufferAndAdvanceState(State state)
{
	if (_outSecBuffer[0].cbBuffer && _outSecBuffer[0].pvBuffer)
	{
		int sent = sendRawBytes(_outSecBuffer[0].pvBuffer, _outSecBuffer[0].cbBuffer);
		if (sent < 0) return;
		if (sent < _outSecBuffer[0].cbBuffer)
		{
			_outSecBuffer[0].cbBuffer -= sent;
			std::memmove(_outSecBuffer[0].pvBuffer, reinterpret_cast<char*>(_outSecBuffer[0].pvBuffer) + sent, _outSecBuffer[0].cbBuffer);
		}
		else
		{
			_outSecBuffer.release(0);
			setState(state);
		}
	}
	else
	{
		setState(state);
	}
}


void SecureSocketImpl::performClientHandshakeLoopIncompleteMessage()
{
	_needData = true;
	setState(ST_CLIENT_HSK_LOOP_INIT);
}


void SecureSocketImpl::initServerContext()
{
	_pOwnCertificate = loadCertificate(true);
	_hCreds = _pContext->credentials();
}


void SecureSocketImpl::performServerHandshake()
{
	serverHandshakeLoop(&_hContext, &_hCreds, _clientAuthRequired, true, true);

	SECURITY_STATUS securityStatus;
	if (_clientAuthRequired)
	{
		poco_assert_dbg (!_pPeerCertificate);
		securityStatus = _securityFunctions.QueryContextAttributesW(&_hContext, SECPKG_ATTR_REMOTE_CERT_CONTEXT, &_pPeerCertificate);

		if (securityStatus != SEC_E_OK)
		{
			if (_pPeerCertificate)
			{
				CertFreeCertificateContext(_pPeerCertificate);
				_pPeerCertificate = 0;
			}
			throw SSLException("Cannot obtain client certificate", Utility::formatError(securityStatus));
		}
		else
		{
			serverVerifyCertificate();
		}
	}

	securityStatus = _securityFunctions.QueryContextAttributesW(&_hContext,SECPKG_ATTR_STREAM_SIZES, &_streamSizes);
	if (securityStatus != SEC_E_OK) throw SSLException("Cannot query stream sizes", Utility::formatError(securityStatus));

	_ioBufferSize = _streamSizes.cbHeader + _streamSizes.cbMaximumMessage + _streamSizes.cbTrailer;
}


bool SecureSocketImpl::serverHandshakeLoop(PCtxtHandle phContext, PCredHandle phCred, bool requireClientAuth, bool doInitialRead, bool newContext)
{
	TimeStamp tsExpiry;
	int n = 0;
	bool doRead = doInitialRead;
	bool initContext = newContext;
	DWORD outFlags;
	SECURITY_STATUS securityStatus = SEC_E_INCOMPLETE_MESSAGE;

	while (securityStatus == SEC_I_CONTINUE_NEEDED || securityStatus == SEC_E_INCOMPLETE_MESSAGE || securityStatus == SEC_I_INCOMPLETE_CREDENTIALS)
	{
		if (securityStatus == SEC_E_INCOMPLETE_MESSAGE)
		{
			if (doRead)
			{
				n = receiveRawBytes(_recvBuffer.begin() + _recvBufferOffset, IO_BUFFER_SIZE - _recvBufferOffset);

				if (n <= 0)
					throw SSLException("Failed to receive data in handshake");
				else
					_recvBufferOffset += n;
			}
			else doRead = true;
		}

		AutoSecBufferDesc<2> inBuffer(&_securityFunctions, false);
		AutoSecBufferDesc<1> outBuffer(&_securityFunctions, true);
		inBuffer.setSecBufferToken(0, _recvBuffer.begin(), _recvBufferOffset);
		inBuffer.setSecBufferEmpty(1);
		outBuffer.setSecBufferToken(0, 0, 0);

		securityStatus = _securityFunctions.AcceptSecurityContext(
						phCred,
						initContext ? NULL : phContext,
						&inBuffer,
						_contextFlags,
						0,
						initContext ? phContext : NULL,
						&outBuffer,
						&outFlags,
						&tsExpiry);

		initContext = false;

		if (securityStatus == SEC_E_OK || securityStatus == SEC_I_CONTINUE_NEEDED || (FAILED(securityStatus) && (0 != (outFlags & ISC_RET_EXTENDED_ERROR))))
		{
			if (outBuffer[0].cbBuffer != 0 && outBuffer[0].pvBuffer != 0)
			{
				n = sendRawBytes(outBuffer[0].pvBuffer, outBuffer[0].cbBuffer);
				outBuffer.release(0);
			}
		}

		if (securityStatus == SEC_E_OK )
		{
			if (inBuffer[1].BufferType == SECBUFFER_EXTRA)
			{
				std::memmove(_recvBuffer.begin(), _recvBuffer.begin() + (_recvBufferOffset - inBuffer[1].cbBuffer), inBuffer[1].cbBuffer);
				_recvBufferOffset = inBuffer[1].cbBuffer;
			}
			else
			{
				_recvBufferOffset = 0;
			}
			return true;
		}
		else if (FAILED(securityStatus) && securityStatus != SEC_E_INCOMPLETE_MESSAGE)
		{
			throw SSLException("Handshake failure:", Utility::formatError(securityStatus));
		}

		if (securityStatus != SEC_E_INCOMPLETE_MESSAGE && securityStatus != SEC_I_INCOMPLETE_CREDENTIALS)
		{
			if (inBuffer[1].BufferType == SECBUFFER_EXTRA)
			{
				std::memmove(_recvBuffer.begin(), _recvBuffer.begin() + (_recvBufferOffset - inBuffer[1].cbBuffer), inBuffer[1].cbBuffer);
				_recvBufferOffset = inBuffer[1].cbBuffer;
			}
			else
			{
				_recvBufferOffset = 0;
			}
		}
	}

	return false;
}


void SecureSocketImpl::clientVerifyCertificate(const std::string& hostName)
{
	if (_pContext->verificationMode() == Context::VERIFY_NONE) return;
	if (!_pPeerCertificate) throw SSLException("No Server certificate");
	if (hostName.empty()) throw SSLException("Server name not set");

	X509Certificate cert(_pPeerCertificate, true);

	if (!cert.verify(hostName))
	{
		VerificationErrorArgs args(cert, 0, SEC_E_CERT_EXPIRED, "The certificate host names do not match the server host name");
		SSLManager::instance().ClientVerificationError(this, args);
		if (!args.getIgnoreError())
			throw InvalidCertificateException("Host name verification failed");
	}

	verifyCertificateChainClient(_pPeerCertificate);
}


void SecureSocketImpl::verifyCertificateChainClient(PCCERT_CONTEXT pServerCert)
{
	X509Certificate cert(pServerCert, true);

	CERT_CHAIN_PARA chainPara;
	PCCERT_CHAIN_CONTEXT pChainContext = NULL;
	std::memset(&chainPara, 0, sizeof(chainPara));
	chainPara.cbSize = sizeof(chainPara);

	if (!CertGetCertificateChain(
							NULL,
							_pPeerCertificate,
							NULL,
							NULL,
							&chainPara,
							0,
							NULL,
							&pChainContext))
	{
		throw SSLException("Cannot get certificate chain", GetLastError());
	}

	HTTPSPolicyCallbackData polHttps;
	std::memset(&polHttps, 0, sizeof(HTTPSPolicyCallbackData));
	polHttps.cbStruct = sizeof(HTTPSPolicyCallbackData);
	polHttps.dwAuthType = AUTHTYPE_SERVER;
	polHttps.fdwChecks = SECURITY_FLAG_IGNORE_UNKNOWN_CA; // we do our own check later on
	polHttps.pwszServerName = 0;

	CERT_CHAIN_POLICY_PARA polPara;
	std::memset(&polPara, 0, sizeof(polPara));
	polPara.cbSize = sizeof(polPara);
	polPara.pvExtraPolicyPara = &polHttps;

	CERT_CHAIN_POLICY_STATUS polStatus;
	std::memset(&polStatus, 0, sizeof(polStatus));
	polStatus.cbSize = sizeof(polStatus);

	if (!CertVerifyCertificateChainPolicy(
						CERT_CHAIN_POLICY_SSL,
						pChainContext,
						&polPara,
						&polStatus))
	{
		VerificationErrorArgs args(cert, 0, GetLastError(), "Failed to verify certificate chain");
		SSLManager::instance().ClientVerificationError(this, args);
		if (!args.getIgnoreError())
		{
			CertFreeCertificateChain(pChainContext);
			throw SSLException("Cannot verify certificate chain");
		}
		else return;
	}
	else if (polStatus.dwError)
	{
		VerificationErrorArgs args(cert, polStatus.lElementIndex, polStatus.dwError, Utility::formatError(polStatus.dwError));
		SSLManager::instance().ClientVerificationError(this, args);
		CertFreeCertificateChain(pChainContext);
		if (!args.getIgnoreError())
		{
			throw SSLException("Failed to verify certificate chain");
		}
		else return;
	}

	// now verify CA's
	HCERTSTORE trustedCerts = _pContext->certificateStore();
	for (DWORD i = 0; i < pChainContext->cChain; i++)
	{
		std::vector<PCCERT_CONTEXT> certs;
		for (DWORD k = 0; k < pChainContext->rgpChain[i]->cElement; k++)
		{
			certs.push_back(pChainContext->rgpChain[i]->rgpElement[k]->pCertContext);
		}
		// verify that the root of the chain can be found in the trusted store
		PCCERT_CONTEXT pResult = CertFindCertificateInStore(trustedCerts, certs.back()->dwCertEncodingType, 0, CERT_FIND_ISSUER_OF, certs.back(), 0);
		if (!pResult)
		{
			poco_assert_dbg (GetLastError() == CRYPT_E_NOT_FOUND);
			VerificationErrorArgs args(cert, i, 0, "Certificate Authority not trusted");
			SSLManager::instance().ClientVerificationError(this, args);
			CertFreeCertificateChain(pChainContext);
			if (!args.getIgnoreError())
				throw CertificateValidationException("Failed to verify certificate chain: CA not trusted");
			else
				return;
		}
		CertFreeCertificateContext(pResult);

		// check if cert is revoked
		if (_pContext->options() & Context::OPT_PERFORM_REVOCATION_CHECK)
		{
			CERT_REVOCATION_STATUS revStat;
			revStat.cbSize = sizeof(CERT_REVOCATION_STATUS);

			BOOL ok = CertVerifyRevocation(
					X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
					CERT_CONTEXT_REVOCATION_TYPE,
					static_cast<DWORD>(certs.size()),
					(void**) &certs[0],
					CERT_VERIFY_REV_CHAIN_FLAG,
					NULL,
					&revStat);

			// Revocation check of the root certificate may fail due to missing CRL points, etc.
			// We ignore all errors checking the root certificate except CRYPT_E_REVOKED.
			if (!ok && revStat.dwIndex < certs.size() - 1 && revStat.dwError == CRYPT_E_REVOKED)
			{
				VerificationErrorArgs args(cert, revStat.dwIndex, revStat.dwReason, Utility::formatError(revStat.dwError));
				SSLManager::instance().ClientVerificationError(this, args);
				if (!args.getIgnoreError())
				{
					CertFreeCertificateChain(pChainContext);
					throw SSLException("Failed to verify revoked certificate chain");
				}
			}
			else break;
		}
	}
	CertFreeCertificateChain(pChainContext);
}


void SecureSocketImpl::serverVerifyCertificate()
{
	if (_pContext->verificationMode() < Context::VERIFY_STRICT) return;

	// we are now in Strict mode
	if (!_pPeerCertificate) throw SSLException("No client certificate");

	DWORD status = SEC_E_OK;
	X509Certificate cert(_pPeerCertificate, true);

	PCCERT_CHAIN_CONTEXT pChainContext = NULL;
	CERT_CHAIN_PARA chainPara;
	std::memset(&chainPara, 0, sizeof(chainPara));
	chainPara.cbSize = sizeof(chainPara);

	if (!CertGetCertificateChain(
							NULL,
							_pPeerCertificate,
							NULL,
							NULL,
							&chainPara,
							CERT_CHAIN_REVOCATION_CHECK_CHAIN,
							NULL,
							&pChainContext))
	{
		throw SSLException("Cannot get certificate chain", GetLastError());
	}

	HTTPSPolicyCallbackData polHttps;
	std::memset(&polHttps, 0, sizeof(HTTPSPolicyCallbackData));
	polHttps.cbStruct       = sizeof(HTTPSPolicyCallbackData);
	polHttps.dwAuthType     = AUTHTYPE_CLIENT;
	polHttps.fdwChecks      = 0;
	polHttps.pwszServerName = 0;

	CERT_CHAIN_POLICY_PARA policyPara;
	std::memset(&policyPara, 0, sizeof(policyPara));
	policyPara.cbSize = sizeof(policyPara);
	policyPara.pvExtraPolicyPara = &polHttps;

	CERT_CHAIN_POLICY_STATUS policyStatus;
	std::memset(&policyStatus, 0, sizeof(policyStatus));
	policyStatus.cbSize = sizeof(policyStatus);

	if (!CertVerifyCertificateChainPolicy(CERT_CHAIN_POLICY_SSL, pChainContext, &policyPara, &policyStatus))
	{
		VerificationErrorArgs args(cert, 0, GetLastError(), "Failed to verify certificate chain");
		SSLManager::instance().ServerVerificationError(this, args);
		CertFreeCertificateChain(pChainContext);
		if (!args.getIgnoreError())
			throw SSLException("Cannot verify certificate chain");
		else
			return;
	}
	else if (policyStatus.dwError)
	{
		VerificationErrorArgs args(cert, policyStatus.lElementIndex, status, Utility::formatError(policyStatus.dwError));
		SSLManager::instance().ServerVerificationError(this, args);
		CertFreeCertificateChain(pChainContext);
		if (!args.getIgnoreError())
			throw SSLException("Failed to verify certificate chain");
		else
			return;
	}

	// perform revocation checking
	for (DWORD i = 0; i < pChainContext->cChain; i++)
	{
		std::vector<PCCERT_CONTEXT> certs;
		for (DWORD k = 0; k < pChainContext->rgpChain[i]->cElement; k++)
		{
			certs.push_back(pChainContext->rgpChain[i]->rgpElement[k]->pCertContext);
		}

		CERT_REVOCATION_STATUS revStat;
		revStat.cbSize = sizeof(CERT_REVOCATION_STATUS);

		BOOL ok = CertVerifyRevocation(
						X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
						CERT_CONTEXT_REVOCATION_TYPE,
						static_cast<DWORD>(certs.size()),
						(void**) &certs[0],
						CERT_VERIFY_REV_CHAIN_FLAG,
						NULL,
						&revStat);

		// Revocation check of the root certificate may fail due to missing CRL points, etc.
		// We ignore all errors checking the root certificate except CRYPT_E_REVOKED.
		if (!ok && revStat.dwIndex < certs.size() - 1 && revStat.dwError == CRYPT_E_REVOKED)
		{
			VerificationErrorArgs args(cert, revStat.dwIndex, revStat.dwReason, Utility::formatError(revStat.dwReason));
			SSLManager::instance().ServerVerificationError(this, args);
			if (!args.getIgnoreError())
			{
				CertFreeCertificateChain(pChainContext);
				throw SSLException("Failed to verify certificate chain");
			}
		}
	}

	if (pChainContext)
	{
		CertFreeCertificateChain(pChainContext);
	}
}


int SecureSocketImpl::clientShutdown(PCredHandle phCreds, CtxtHandle* phContext)
{
	if (phContext->dwLower == 0 && phContext->dwUpper == 0)
	{
		return 0;
	}

	AutoSecBufferDesc<1> tokBuffer(&_securityFunctions, false);

	DWORD tokenType = SCHANNEL_SHUTDOWN;
	tokBuffer.setSecBufferToken(0, &tokenType, sizeof(tokenType));
	DWORD status = _securityFunctions.ApplyControlToken(phContext, &tokBuffer);

	if (FAILED(status)) throw SSLException(Utility::formatError(status));

	DWORD sspiFlags = ISC_REQ_SEQUENCE_DETECT
                    | ISC_REQ_REPLAY_DETECT
	                | ISC_REQ_CONFIDENTIALITY
	                | ISC_RET_EXTENDED_ERROR
	                | ISC_REQ_ALLOCATE_MEMORY
	                | ISC_REQ_STREAM;

	AutoSecBufferDesc<1> outBuffer(&_securityFunctions, true);
	outBuffer.setSecBufferToken(0, 0, 0);

	DWORD sspiOutFlags;
	TimeStamp expiry;
	status = _securityFunctions.InitializeSecurityContextW(
				phCreds,
				phContext,
				NULL,
				sspiFlags,
				0,
				0,
				NULL,
				0,
				phContext,
				&outBuffer,
				&sspiOutFlags,
				&expiry);

	if (FAILED(status)) throw SSLException(Utility::formatError(status));

	if (outBuffer[0].pvBuffer && outBuffer[0].cbBuffer)
	{
		int sent = sendRawBytes(outBuffer[0].pvBuffer, outBuffer[0].cbBuffer);
		return sent;
	}

	return 0;
}


int SecureSocketImpl::serverShutdown(PCredHandle phCreds, CtxtHandle* phContext)
{
	if (phContext->dwLower == 0 && phContext->dwUpper == 0)
	{
		// handshake has never been done
		poco_assert_dbg (_needHandshake);
		return SEC_E_OK;
	}

	AutoSecBufferDesc<1> tokBuffer(&_securityFunctions, false);

	DWORD tokenType = SCHANNEL_SHUTDOWN;
	tokBuffer.setSecBufferToken(0, &tokenType, sizeof(tokenType));
	DWORD status = _securityFunctions.ApplyControlToken(phContext, &tokBuffer);

	if (FAILED(status)) return status;

	DWORD sspiFlags = ASC_REQ_SEQUENCE_DETECT
	                | ASC_REQ_REPLAY_DETECT
	                | ASC_REQ_CONFIDENTIALITY
	                | ASC_REQ_EXTENDED_ERROR
	                | ASC_REQ_ALLOCATE_MEMORY
	                | ASC_REQ_STREAM;

	AutoSecBufferDesc<1> outBuffer(&_securityFunctions, true);
	outBuffer.setSecBufferToken(0,0,0);

	DWORD sspiOutFlags;
	TimeStamp expiry;
	status = _securityFunctions.AcceptSecurityContext(
					phCreds,
					phContext,
					NULL,
					sspiFlags,
					0,
					NULL,
					&outBuffer,
					&sspiOutFlags,
					&expiry);

	if (FAILED(status)) return status;

	if (outBuffer[0].pvBuffer && outBuffer[0].cbBuffer)
	{
		int sent = sendRawBytes(outBuffer[0].pvBuffer, outBuffer[0].cbBuffer);
		if (sent <= 0)
		{
			status = WSAGetLastError();
		}
	}

	return status;
}


void SecureSocketImpl::stateIllegal()
{
	throw Poco::IllegalStateException("SSL state machine");
}


void SecureSocketImpl::stateConnected()
{
	_peerHostName = _pSocket->peerAddress().host().toString();
	setState(ST_CLIENT_HSK_START);
}


void SecureSocketImpl::performClientHandshakeStart()
{
	initClientContext();
	performInitialClientHandshake();
}


void SecureSocketImpl::stateMachine()
{
	StateMachine::instance().execute(this);
}


StateMachine& StateMachine::instance()
{
	static StateMachine sm;
	return sm;
}


StateMachine::StateMachine():
	_states(SecureSocketImpl::ST_MAX)
{														     
	_states[SecureSocketImpl::ST_INITIAL]                    = &SecureSocketImpl::stateIllegal;
	_states[SecureSocketImpl::ST_CONNECTING]                 = &SecureSocketImpl::stateConnected;
	_states[SecureSocketImpl::ST_CLIENT_HSK_START]           = &SecureSocketImpl::performClientHandshakeStart; 
	_states[SecureSocketImpl::ST_CLIENT_HSK_SEND_TOKEN]      = &SecureSocketImpl::performClientHandshakeSendToken; 
	_states[SecureSocketImpl::ST_CLIENT_HSK_LOOP_INIT]       = &SecureSocketImpl::performClientHandshakeLoopInit;
	_states[SecureSocketImpl::ST_CLIENT_HSK_LOOP_RECV]       = &SecureSocketImpl::performClientHandshakeLoopRecv;
	_states[SecureSocketImpl::ST_CLIENT_HSK_LOOP_PROCESS]    = &SecureSocketImpl::performClientHandshakeLoopProcess;
	_states[SecureSocketImpl::ST_CLIENT_HSK_LOOP_SEND]       = &SecureSocketImpl::performClientHandshakeLoopSend;
	_states[SecureSocketImpl::ST_CLIENT_HSK_LOOP_INCOMPLETE] = &SecureSocketImpl::performClientHandshakeLoopIncompleteMessage;
	_states[SecureSocketImpl::ST_CLIENT_HSK_LOOP_CONTINUE]   = &SecureSocketImpl::performClientHandshakeLoopContinue;
	_states[SecureSocketImpl::ST_CLIENT_HSK_LOOP_DONE]       = &SecureSocketImpl::performClientHandshakeLoopDone;
	_states[SecureSocketImpl::ST_CLIENT_HSK_SEND_FINAL]      = &SecureSocketImpl::performClientHandshakeSendFinal;
	_states[SecureSocketImpl::ST_CLIENT_HSK_SEND_ERROR]      = &SecureSocketImpl::performClientHandshakeSendError;
	_states[SecureSocketImpl::ST_CLIENT_VERIFY]              = &SecureSocketImpl::clientConnectVerify;
	_states[SecureSocketImpl::ST_DONE]                       = &SecureSocketImpl::stateIllegal;
	_states[SecureSocketImpl::ST_ERROR]                      = &SecureSocketImpl::performClientHandshakeError;
}


bool StateMachine::execute(SecureSocketImpl* pSock)
{
	try
	{
		poco_assert_dbg (pSock);
		auto curState = pSock->getState();
		StateImpl& stateImpl = _states[curState];
		(pSock->*(stateImpl))();
		return pSock->getState() != curState;
	}
	catch (...)
	{
		pSock->setState(SecureSocketImpl::ST_ERROR);
		throw;
	}
}


StateMachine::~StateMachine()
{
}


} } // namespace Poco::Net
