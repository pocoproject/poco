//
// SecureSocketImpl.cpp
//
// $Id$
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
#include "Poco/Net/NetException.h"
#include "Poco/Net/DNS.h"
#include "Poco/NumberFormatter.h"
#include "Poco/NumberParser.h"
#include "Poco/String.h"
#include "Poco/Format.h"
#include "Poco/UnicodeConverter.h"
#include <wininet.h>


using Poco::IOException;
using Poco::TimeoutException;
using Poco::InvalidArgumentException;
using Poco::NumberFormatter;
using Poco::Timespan;


namespace Poco {
namespace Net {


class StateMachine
{
public:
	typedef bool (StateMachine::*ConditionMethod)(SOCKET sockfd);
	typedef void (SecureSocketImpl::*StateImpl)(void);

	StateMachine();
	~StateMachine();

	static StateMachine& instance();

	// Conditions
	bool readable(SOCKET sockfd);
	bool writable(SOCKET sockfd);
	bool readOrWritable(SOCKET sockfd);
	bool none(SOCKET sockfd);
	void select(fd_set* fdRead, fd_set* fdWrite, SOCKET sockfd);

	void execute(SecureSocketImpl* pSock);

private:
	StateMachine(const StateMachine&);
	StateMachine& operator=(const StateMachine&);

	typedef std::pair<ConditionMethod, StateImpl> ConditionState;
	std::vector<ConditionState> _states;
};


SecureSocketImpl::SecureSocketImpl(Poco::AutoPtr<SocketImpl> pSocketImpl, Context::Ptr pContext):
	_pSocket(pSocketImpl),
	_pContext(pContext),
	_mode(pContext->isForServerUse() ? MODE_SERVER : MODE_CLIENT),
	_clientAuthRequired(pContext->verificationMode() >= Context::VERIFY_STRICT),
	_pServerCertificate(0),
	_pPeerCertificate(0),
	_hCreds(),
	_hContext(),
	_clientFlags(0),
	_serverFlags(0),
	_securityFunctions(SSLManager::instance().securityFunctions()),
	_pReceiveBuffer(0),
	_receiveBufferSize(0),
	_pIOBuffer(0),
	_pSendBuffer(),
	_ioBufferOffset(0),
	_ioBufferSize(0),
	_streamSizes(),
	_outSecBuffer(&_securityFunctions, true),
	_inSecBuffer(&_securityFunctions, false),
	_extraSecBuffer(),
	_doReadFirst(true),
	_bytesRead(0),
	_securityStatus(SEC_E_INCOMPLETE_MESSAGE),
	_state(ST_INITIAL),
	_needHandshake(false)
{
	_hCreds.dwLower = 0;
	_hCreds.dwUpper = 0;

	_hContext.dwLower = 0;
	_hContext.dwUpper = 0;
	_streamSizes.cbBlockSize = _streamSizes.cbHeader = _streamSizes.cbMaximumMessage = _streamSizes.cbTrailer = 0;

	initCommon();
}


SecureSocketImpl::~SecureSocketImpl()
{
	cleanup();
}


void SecureSocketImpl::initCommon()
{
	DWORD commonFlags = ISC_REQ_SEQUENCE_DETECT
	             | ISC_REQ_REPLAY_DETECT
	             | ISC_REQ_CONFIDENTIALITY
	             | ISC_RET_EXTENDED_ERROR
	             | ISC_REQ_ALLOCATE_MEMORY
	             | ISC_REQ_STREAM;

	if (_pContext->verificationMode() == Context::VERIFY_NONE)
	{
		commonFlags |= ISC_REQ_MANUAL_CRED_VALIDATION;
	}
	else if (_pContext->verificationMode() == Context::VERIFY_RELAXED)
	{
		commonFlags |= ISC_REQ_INTEGRITY;
	}
	else if (_pContext->verificationMode() == Context::VERIFY_STRICT)
	{
		commonFlags |= ISC_REQ_INTEGRITY;
	}

	_serverFlags = commonFlags;
	_clientFlags = commonFlags;

	if (_pContext->verificationMode() == Context::VERIFY_RELAXED)
		_clientFlags |= ISC_REQ_MANUAL_CRED_VALIDATION;
}


void SecureSocketImpl::cleanup()
{
	_hostName.clear();

	_hCreds.dwLower = 0;
	_hCreds.dwUpper = 0;

	if (_hContext.dwLower != 0 && _hContext.dwUpper != 0)
	{
		_securityFunctions.DeleteSecurityContext(&_hContext);
		_hContext.dwLower = 0;
		_hContext.dwUpper = 0;
	}

	if (_pServerCertificate)
	{
		CertFreeCertificateContext(_pServerCertificate);
		_pServerCertificate = 0;
	}

	if (_pPeerCertificate)
	{
		CertFreeCertificateContext(_pPeerCertificate);
		_pPeerCertificate = 0;
	}

	// must release buffers before unloading library
	_outSecBuffer.release();
	_inSecBuffer.release();

	delete [] _pReceiveBuffer;
	_pReceiveBuffer = 0;

	delete [] _pIOBuffer;
	_pIOBuffer = 0;
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
	_state = ST_ERROR;
	_pSocket->connect(address);
	connectSSL(performHandshake);
	_state = ST_DONE;
}


void SecureSocketImpl::connect(const SocketAddress& address, const Poco::Timespan& timeout, bool performHandshake)
{
	_state = ST_ERROR;
	_pSocket->connect(address, timeout);
	connectSSL(performHandshake);
	_state = ST_DONE;
}


void SecureSocketImpl::connectNB(const SocketAddress& address)
{
	try
	{
		_state = ST_CONNECTING;
		_pSocket->connectNB(address);
	}
	catch (...)
	{
		_state = ST_ERROR;
	}
}


void SecureSocketImpl::bind(const SocketAddress& address, bool reuseAddress)
{
	_pSocket->bind(address, reuseAddress);
}


void SecureSocketImpl::listen(int backlog)
{
	_mode = MODE_SERVER;

	if ((_hCreds.dwLower == 0) && (_hCreds.dwUpper == 0))
	{
		initServerContext();
	}
	_pSocket->listen(backlog);
}


void SecureSocketImpl::shutdown()
{
	if (_mode == MODE_SERVER)
		serverDisconnect(&_hCreds, &_hContext);
	else
		clientDisconnect(&_hCreds, &_hContext);

	_pSocket->shutdown();
}


void SecureSocketImpl::close()
{
	if (_mode == MODE_SERVER)
		serverDisconnect(&_hCreds, &_hContext);
	else
		clientDisconnect(&_hCreds, &_hContext);
	
	_pSocket->close();
	cleanup();
}


void SecureSocketImpl::abort()
{
	_pSocket->shutdown();
	cleanup();
}


void SecureSocketImpl::acceptSSL()
{
	_state = ST_DONE;
	initServerContext();
	_needHandshake = true;
}


int SecureSocketImpl::sendBytes(const void* buffer, int length, int flags)
{
	if (_needHandshake)
	{
		completeHandshake();
		_needHandshake = false;
	}

	if (_state == ST_ERROR)
		return 0;

	if (_state != ST_DONE)
	{
		bool establish = _pSocket->getBlocking();
		if (establish)
		{
			while (_state != ST_DONE)
				stateMachine();
		}
		else
		{
			stateMachine();
			return -1;
		}
	}

	int rc = 0;
	int dataToSend = length;
	int dataSent = 0;
	const char* pBuffer = reinterpret_cast<const char*>(buffer);
	if (!_pSendBuffer || (_pSendBuffer->size() != _ioBufferSize))
		_pSendBuffer = new Poco::Buffer<BYTE>(_ioBufferSize);

	while (dataToSend > 0)
	{
		AutoSecBufferDesc<4> msg(&_securityFunctions, false);
		int dataSize = dataToSend;
		if (dataSize > _streamSizes.cbMaximumMessage)
			dataSize = _streamSizes.cbMaximumMessage;
		SecBuffer* pDataBuffer  = 0;
		SecBuffer* pExtraBuffer = 0;

		CopyMemory(_pSendBuffer->begin() + _streamSizes.cbHeader, pBuffer + dataSent, dataSize);
		
		msg.setSecBufferStreamHeader(0, _pSendBuffer->begin(), _streamSizes.cbHeader);
		msg.setSecBufferData(1, _pSendBuffer->begin() + _streamSizes.cbHeader, dataSize);
		msg.setSecBufferStreamTrailer(2, _pSendBuffer->begin() + _streamSizes.cbHeader + dataSize, _streamSizes.cbTrailer);
		msg.setSecBufferEmpty(3);

		SECURITY_STATUS securityStatus = _securityFunctions.EncryptMessage(&_hContext, 0, &msg, 0);

		if (FAILED(securityStatus) && securityStatus != SEC_E_CONTEXT_EXPIRED)
			throw SSLException("Failed to encrypt message", Utility::formatError(securityStatus));

		int outBufferLen = msg[0].cbBuffer+msg[1].cbBuffer + msg[2].cbBuffer;

		int rcTmp = sendRawBytes(_pSendBuffer->begin(), outBufferLen, flags);
		if (_pSocket->getBlocking() && rcTmp == -1)
		{
			if (dataSent == 0)
				return -1;
			return dataSent;
		}
		if (rcTmp != outBufferLen)
			throw SSLException("Failed to send encrypted message");

		dataToSend -= dataSize;
		dataSent += dataSize;
		rc += rcTmp;
	}
	return dataSent;
}


int SecureSocketImpl::sendRawBytes(const void* buffer, int length, int flags)
{
	return _pSocket->sendBytes(buffer, length, flags);
}


int SecureSocketImpl::receiveBytes(void* buffer, int length, int flags)
{
	if (_needHandshake)
	{
		completeHandshake();
		_needHandshake = false;
	}

	if (_state == ST_ERROR)
		return 0;

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
			return -1;
		}
	}

	int rc(0);
	if (_receiveBufferSize) // any remaining data from previous calls?
	{
		if (static_cast<DWORD>(length) < _receiveBufferSize)
		{
			rc = length;
			CopyMemory(buffer, _pReceiveBuffer, rc);
			MoveMemory(_pReceiveBuffer, _pReceiveBuffer + rc, _receiveBufferSize-rc);
			_receiveBufferSize -= rc;
		}
		else
		{
			rc = _receiveBufferSize;
			CopyMemory(buffer, _pReceiveBuffer, rc);
			delete [] _pReceiveBuffer;
			_pReceiveBuffer = 0;
			_receiveBufferSize = 0;
		}
	}
	else
	{
		poco_assert (!_pReceiveBuffer);

		if (!_pIOBuffer) 
			_pIOBuffer = new BYTE[IO_BUFFER_SIZE];
		bool mustReceive = _ioBufferOffset == 0;
		bool cont = true;
		do
		{
			if (mustReceive)
			{
				int numBytes = receiveRawBytes(_pIOBuffer + _ioBufferOffset, _ioBufferSize - _ioBufferOffset);
					
				if (numBytes == -1)
					return -1;
				else if (numBytes == 0) 
					break;
				else
					_ioBufferOffset += numBytes;
			}
			else mustReceive = true;

			int bytesDecoded = 0;
			_extraSecBuffer = SecBuffer();
			SECURITY_STATUS securityStatus = decodeBufferFull(_pIOBuffer, _ioBufferOffset, reinterpret_cast<char*>(buffer), length, bytesDecoded);
			if (_extraSecBuffer.cbBuffer > 0)
			{
				MoveMemory(_pIOBuffer, _extraSecBuffer.pvBuffer, _extraSecBuffer.cbBuffer);
				_ioBufferOffset = _extraSecBuffer.cbBuffer;
			}
			else
			{
				_ioBufferOffset = 0;
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

				return rc;
			}

			if (securityStatus == SEC_E_INCOMPLETE_MESSAGE)
			{
				if (!_pSocket->getBlocking())
					return -1;
				continue;
			}

			if (securityStatus == SEC_I_CONTEXT_EXPIRED)
			{
				SetLastError(securityStatus);
				break;
			}

			if (securityStatus != SEC_E_OK && securityStatus != SEC_I_RENEGOTIATE && securityStatus != SEC_I_CONTEXT_EXPIRED)
			{
				SetLastError(securityStatus);
				break;
			}

			if (securityStatus == SEC_I_RENEGOTIATE)
			{
				_doReadFirst = false;
				_state = ST_CLIENTHANDSHAKECONDREAD;
				if (!_pSocket->getBlocking())
					return -1;

				securityStatus = performClientHandshakeLoop();

				if (securityStatus != SEC_E_OK)
					break;

				if (_extraSecBuffer.pvBuffer)
				{
					MoveMemory(_pIOBuffer, _extraSecBuffer.pvBuffer, _extraSecBuffer.cbBuffer);
					_ioBufferOffset = _extraSecBuffer.cbBuffer;
				}

				delete [] _extraSecBuffer.pvBuffer;
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

	if (securityStatus == SEC_E_OK || securityStatus == SEC_I_RENEGOTIATE)
	{
		for (int i = 1; i < 4; ++i)
		{
			if (pDataBuffer == 0 && msg[i].BufferType == SECBUFFER_DATA)
				pDataBuffer = &msg[i];

			if (pExtraBuffer == NULL && msg[i].BufferType == SECBUFFER_EXTRA)
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
	poco_assert (_pReceiveBuffer == 0);

	_extraSecBuffer.pvBuffer = 0;
	_extraSecBuffer.cbBuffer = 0;

	SecBuffer* pDataBuffer = 0;
	SecBuffer* pExtraBuffer = 0;
	int bytes = 0;
	bytesDecoded = 0;

#ifdef _DEBUG
	Poco::Buffer<BYTE> bufCpy(bufSize);
	memcpy(bufCpy.begin(), pBuffer, bufSize);
#endif

	Poco::SharedPtr<Poco::Buffer<BYTE> > ptrOverflow;
	int overflowIdx = 0;
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
				memcpy(pOutBuffer, pDataBuffer->pvBuffer, bytes);
				pOutBuffer += bytes;
			}
			else
			{
				// not enough room in pOutBuffer, write overflow data
				// simply reserve bufSize bytes (is large enough even in worst case scenario, no need to re-increase)
				if (!ptrOverflow)
					ptrOverflow = new Poco::Buffer<BYTE>(bufSize);
				if (outLength > 0)
				{
					// make pOutBuffer full 
					memcpy(pOutBuffer, pDataBuffer->pvBuffer, outLength);
					// no longer valid to write to pOutBuffer
					pOutBuffer = 0;
					// copy the rest to ptrOverflow
					memcpy(ptrOverflow->begin(), reinterpret_cast<char*>(pDataBuffer->pvBuffer) + outLength, bytes - outLength);
					overflowIdx = bytes - outLength;
					outLength = 0;
				}
				else
				{
					// append to ptrOverflow
					poco_assert_dbg (overflowIdx + bytes <= ptrOverflow->size());
					memcpy(ptrOverflow->begin() + overflowIdx, pDataBuffer->pvBuffer, bytes);
					overflowIdx += bytes;
				}
			}
		}
		if (pExtraBuffer && pExtraBuffer->cbBuffer > 0)
		{
			// we have potentially more data to decode
			// decode as much as possible
			pBuffer = reinterpret_cast<BYTE*>(pExtraBuffer->pvBuffer);
			bufSize = pExtraBuffer->cbBuffer;

			poco_assert_dbg (memcmp(pBuffer, bufCpy.end() - bufSize, bufSize) == 0);
		}
		else
		{
			// everything decoded
			if (securityStatus != SEC_E_OK && securityStatus != SEC_E_INCOMPLETE_MESSAGE && securityStatus != SEC_I_RENEGOTIATE && securityStatus != SEC_I_CONTEXT_EXPIRED)
			{
				throw SSLException("Failed to decode data", Utility::formatError(securityStatus));
			}
			else if (securityStatus == SEC_E_OK)
			{
				pBuffer = 0;
				bufSize = 0;
			}
		}

		if (securityStatus == SEC_I_RENEGOTIATE) 
		{
			_doReadFirst = false;
			securityStatus = performClientHandshakeLoop();
			if (securityStatus != SEC_E_OK)
				break;
		}
	}
	while (securityStatus == SEC_E_OK && pBuffer);
	
	if (overflowIdx > 0)
	{
		_pReceiveBuffer = new BYTE[overflowIdx];
		memcpy(_pReceiveBuffer, ptrOverflow->begin(), overflowIdx);
		_receiveBufferSize = overflowIdx;
	}
	if (bufSize > 0)
	{
		_extraSecBuffer.cbBuffer = bufSize;
		_extraSecBuffer.pvBuffer = pBuffer;
	}

	if (pBuffer == 0) securityStatus = SEC_E_OK;
	return securityStatus;
}


int SecureSocketImpl::receiveRawBytes(void* buffer, int length, int flags)
{
	return _pSocket->receiveBytes(buffer, length, flags);
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
		// TODO: avoid use of PCCERT_CONTEXT
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
	poco_assert_dbg(_hostName.empty());

	_hostName = _pSocket->address().host().toString();
	
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
	poco_assert_dbg(!_hostName.empty());

	try
	{
		SECURITY_STATUS securityStatus = _securityFunctions.QueryContextAttributesW(&_hContext, SECPKG_ATTR_REMOTE_CERT_CONTEXT, (PVOID) &_pPeerCertificate);
		if (securityStatus != SEC_E_OK) 
			throw SSLException("Failed to obtain peer certificate", Utility::formatError(securityStatus));

		clientVerifyCertificate(_pPeerCertificate, _hostName, 0);

		securityStatus = _securityFunctions.QueryContextAttributesW(&_hContext, SECPKG_ATTR_STREAM_SIZES, &_streamSizes);
		if (securityStatus != SEC_E_OK)
			throw SSLException("Failed to query stream sizes", Utility::formatError(securityStatus));

		_ioBufferSize = _streamSizes.cbHeader + _streamSizes.cbMaximumMessage + _streamSizes.cbTrailer;
		_state = ST_DONE;
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
	_pServerCertificate = loadCertificate(false);
	_hCreds = _pContext->credentials();
}


void SecureSocketImpl::performClientHandshake()
{
	performInitialClientHandshake();
	performClientHandshakeLoop();
	clientConnectVerify();
}


void SecureSocketImpl::performInitialClientHandshake()
{
	// get initial security token
	_outSecBuffer = AutoSecBufferDesc<1>(&_securityFunctions, true);
	_outSecBuffer.setSecBufferToken(0, 0, 0);

	TimeStamp ts;
	DWORD contextAttributes(0);
	std::wstring whostName;
	Poco::UnicodeConverter::convert(_hostName, whostName);
	_securityStatus = _securityFunctions.InitializeSecurityContextW(
						&_hCreds,
						0,
						const_cast<SEC_WCHAR*>(whostName.c_str()),
						_clientFlags,
						0,
						0,
						0,
						0,
						&_hContext, // init hContext
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
	
	// incomplete credentials: more calls to InitializeSecurityContext needed
	// send the token
	sendInitialTokenOutBuffer();

	if (_securityStatus == SEC_E_OK)
	{
		// The security context was successfully initialized. 
		// There is no need for another InitializeSecurityContext (Schannel) call. 
		_state = ST_DONE;
		return;
	}

	//SEC_I_CONTINUE_NEEDED was returned:
	// Wait for a return token. The returned token is then passed in 
	// another call to InitializeSecurityContext (Schannel). The output token can be empty.

	_extraSecBuffer = SecBuffer();
	_doReadFirst = true;
	_state = ST_CLIENTHANDSHAKECONDREAD;
	_securityStatus = SEC_E_INCOMPLETE_MESSAGE;
}


void SecureSocketImpl::sendInitialTokenOutBuffer()
{
	// send the token
	if (_outSecBuffer[0].cbBuffer != 0 && _outSecBuffer[0].pvBuffer != 0)
	{
		int numBytes = sendRawBytes(_outSecBuffer[0].pvBuffer, _outSecBuffer[0].cbBuffer);

		if (numBytes != _outSecBuffer[0].cbBuffer)
			throw SSLException("Failed to send token to the server");
	}
}


SECURITY_STATUS SecureSocketImpl::performClientHandshakeLoop()
{
	_bytesRead = 0;
	_ioBufferOffset = 0;
	_securityStatus = SEC_E_INCOMPLETE_MESSAGE;

	while (_securityStatus == SEC_I_CONTINUE_NEEDED || _securityStatus == SEC_E_INCOMPLETE_MESSAGE || _securityStatus == SEC_I_INCOMPLETE_CREDENTIALS)
	{
		performClientHandshakeLoopCondRead();
		
		if (_securityStatus == SEC_E_OK)
			performClientHandshakeLoopOK();
		else if (_securityStatus == SEC_I_CONTINUE_NEEDED)
			performClientHandshakeLoopContinueNeeded();
		else if (FAILED(_securityStatus))
		{
			if (_outFlags & ISC_RET_EXTENDED_ERROR)
				performClientHandshakeLoopExtError();
			else
				performClientHandshakeLoopError();
		}
		else
			performClientHandshakeLoopIncompleteMessage();
	}

	if (FAILED(_securityStatus))
	{
		performClientHandshakeLoopError();
	}

	return _securityStatus;
}


void SecureSocketImpl::performClientHandshakeLoopExtError()
{
	poco_assert_dbg (FAILED(_securityStatus));

	performClientHandshakeSendOutBuffer();
	performClientHandshakeLoopError();
}


void SecureSocketImpl::performClientHandshakeLoopError()
{
	poco_assert_dbg (FAILED(_securityStatus));
	cleanup();
	_state = ST_ERROR;
	throw SSLException("Error during handshake", Utility::formatError(_securityStatus));
}


void SecureSocketImpl::performClientHandshakeSendOutBuffer()
{
	if (_outSecBuffer[0].cbBuffer != 0 && _outSecBuffer[0].pvBuffer != NULL) 
	{
		int numBytes = sendRawBytes(static_cast<const void*>(_outSecBuffer[0].pvBuffer), _outSecBuffer[0].cbBuffer);

		if (numBytes != _outSecBuffer[0].cbBuffer)
		{
			throw SSLException("Socket error during handshake");
		}
		_outSecBuffer.release(0);
	}
}


void SecureSocketImpl::performClientHandshakeExtraBuffer()
{
	if (_inSecBuffer[1].BufferType == SECBUFFER_EXTRA)
	{
		MoveMemory(_pIOBuffer, _pIOBuffer + (_ioBufferOffset - _inSecBuffer[1].cbBuffer), _inSecBuffer[1].cbBuffer);
		_ioBufferOffset = _inSecBuffer[1].cbBuffer;
	}
	else _ioBufferOffset = 0;
}


void SecureSocketImpl::performClientHandshakeLoopOK()
{
	poco_assert_dbg(_securityStatus == SEC_E_OK);

	performClientHandshakeSendOutBuffer();
	performClientHandshakeExtraBuffer();
	_state = ST_VERIFY;
}


void SecureSocketImpl::performClientHandshakeLoopInit()
{
	_inSecBuffer = AutoSecBufferDesc<2> (&_securityFunctions, false);
	_outSecBuffer = AutoSecBufferDesc<1>(&_securityFunctions, true);
}


void SecureSocketImpl::performClientHandshakeLoopRead()
{
	poco_assert_dbg (_doReadFirst);
	poco_assert (IO_BUFFER_SIZE > _ioBufferOffset);

	if (!_pIOBuffer)
		_pIOBuffer = new BYTE[IO_BUFFER_SIZE];

	_bytesRead = receiveRawBytes(_pIOBuffer + _ioBufferOffset, IO_BUFFER_SIZE - _ioBufferOffset);
	if (_bytesRead <= 0)
		throw SSLException("Error during handshake: failed to read data");

	_ioBufferOffset += _bytesRead;
}


void SecureSocketImpl::performClientHandshakeLoopCondRead()
{
	poco_assert_dbg (_securityStatus == SEC_E_INCOMPLETE_MESSAGE || SEC_I_CONTINUE_NEEDED);
	
	performClientHandshakeLoopInit();
	if (_doReadFirst)
	{
		performClientHandshakeLoopRead();
	}
	else _doReadFirst = true;
		
	_inSecBuffer.setSecBufferToken(0, _pIOBuffer, _ioBufferOffset);
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
								_clientFlags,
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
		_state = ST_CLIENTHANDSHAKEOK;
	}
	else if (_securityStatus == SEC_I_CONTINUE_NEEDED)
	{
		_state = ST_CLIENTHANDSHAKECONTINUE;
	}
	else if (FAILED(_securityStatus))
	{
		if (_outFlags & ISC_RET_EXTENDED_ERROR)
			_state = ST_CLIENTHANDSHAKEEXTERROR;
		else
			_state = ST_ERROR;
	}
	else
	{
		_state = ST_CLIENTHANDSHAKEINCOMPLETE;
	}
}


void SecureSocketImpl::performClientHandshakeLoopContinueNeeded()
{
	performClientHandshakeSendOutBuffer();
	performClientHandshakeExtraBuffer();
	_state = ST_CLIENTHANDSHAKECONDREAD;
}


void SecureSocketImpl::performClientHandshakeLoopIncompleteMessage()
{
	_doReadFirst = true;
	_state = ST_CLIENTHANDSHAKECONDREAD;
}


void SecureSocketImpl::initServerContext()
{
	_pServerCertificate = loadCertificate(true);
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
			serverVerifyCertificate(_pPeerCertificate, 0);
		}
	}

	securityStatus = _securityFunctions.QueryContextAttributesW(&_hContext,SECPKG_ATTR_STREAM_SIZES, &_streamSizes);
	if (securityStatus != SEC_E_OK)
	{
		throw SSLException("Cannot query stream sizes", Utility::formatError(securityStatus));
	}

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
				if (!_pIOBuffer)
				{
					_pIOBuffer = new BYTE[IO_BUFFER_SIZE];
				}
				n = receiveRawBytes(_pIOBuffer + _ioBufferOffset, IO_BUFFER_SIZE - _ioBufferOffset);

				if (n <= 0)
					throw SSLException("Failed to receive data in handshake");
				else
					_ioBufferOffset += n;
			} 
			else doRead = true;
		}

		AutoSecBufferDesc<2> inBuffer(&_securityFunctions, false);
		AutoSecBufferDesc<1> outBuffer(&_securityFunctions, true);
		inBuffer.setSecBufferToken(0, _pIOBuffer, _ioBufferOffset);
		inBuffer.setSecBufferEmpty(1);
		outBuffer.setSecBufferToken(0, 0, 0);

		securityStatus = _securityFunctions.AcceptSecurityContext(
						phCred,
						initContext ? NULL : phContext,
						&inBuffer,
						_serverFlags,
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
				memcpy(_pIOBuffer, _pIOBuffer + (_ioBufferOffset - inBuffer[1].cbBuffer), inBuffer[1].cbBuffer);
				_ioBufferOffset = inBuffer[1].cbBuffer;
			} 
			else 
			{
				_ioBufferOffset = 0;
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
				memcpy(_pIOBuffer, _pIOBuffer + (_ioBufferOffset - inBuffer[1].cbBuffer), inBuffer[1].cbBuffer);
				_ioBufferOffset = inBuffer[1].cbBuffer;
			}
			else 
			{
				_ioBufferOffset = 0;
			}
		}
	}

	return false;
}


void SecureSocketImpl::clientVerifyCertificate(PCCERT_CONTEXT pServerCert, const std::string& serverName, DWORD dwCertFlags)
{
	if (_pContext->verificationMode() == Context::VERIFY_NONE)
		return;
	
	if (pServerCert == NULL)
		throw SSLException("No Server certificate");

	if (serverName.empty())
		throw SSLException("Server name not set");

	int iRc = CertVerifyTimeValidity(NULL,pServerCert->pCertInfo);
	if (iRc != 0) 
	{
		X509Certificate cert(pServerCert, true);
		VerificationErrorArgs args(cert, 0, SEC_E_CERT_EXPIRED, "The certificate is expired");
		SSLManager::instance().ClientVerificationError(this, args);
		if (!args.getIgnoreError())
			throw InvalidCertificateException("Expired certificate");
	}

	CERT_CHAIN_PARA chainPara;
	PCCERT_CHAIN_CONTEXT pChainContext = NULL;
	ZeroMemory(&chainPara, sizeof(chainPara));
	chainPara.cbSize = sizeof(chainPara);

	if (!CertGetCertificateChain(
							NULL,
							pServerCert,
							NULL,
							NULL,
							&chainPara,
							0,
							NULL,
							&pChainContext))
	{
		CertFreeCertificateChain(pChainContext);
		throw SSLException("Failed to get certificate chain", GetLastError());
	}

	verifyCertificateChainClient(pServerCert, pChainContext);	
}


void SecureSocketImpl::verifyCertificateChainClient(PCCERT_CONTEXT pServerCert, PCCERT_CHAIN_CONTEXT pChainContext)
{
	X509Certificate cert(pServerCert, true);

	HTTPSPolicyCallbackData polHttps; 
	memset(&polHttps, 0, sizeof(HTTPSPolicyCallbackData));
	polHttps.cbStruct = sizeof(HTTPSPolicyCallbackData);
	polHttps.dwAuthType = AUTHTYPE_SERVER;
	polHttps.fdwChecks = SECURITY_FLAG_IGNORE_UNKNOWN_CA; // we do our own CA verification!
	polHttps.pwszServerName = 0;// not supported on Win98, ME! but ignored on client side anyway

	CERT_CHAIN_POLICY_PARA polPara;
	memset(&polPara, 0, sizeof(polPara));
	polPara.cbSize = sizeof(polPara);
	polPara.pvExtraPolicyPara = &polHttps;

	CERT_CHAIN_POLICY_STATUS polStatus;
	memset(&polStatus, 0, sizeof(polStatus));
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
			throw SSLException("Failed to verify certificate chain");
		}
		else return;
	}

	if (polStatus.dwError)
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
	Poco::Buffer<PCERT_CONTEXT> certs(pChainContext->cChain);
	for (DWORD i = 0; i < pChainContext->cChain; i++)
	{
		certs[i] = (PCERT_CONTEXT)(pChainContext->rgpChain[i]->rgpElement[0]->pCertContext);
		// each issuer of the pCert must be a member of the trustedCerts store
		PCCERT_CONTEXT pResult = CertFindCertificateInStore(trustedCerts, certs[i]->dwCertEncodingType, 0, CERT_FIND_ISSUER_OF, certs[i], 0);

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
	}

#if !defined(_WIN32_WCE)
	// check if cert is revoked
	if (_pContext->options() & Context::OPT_PERFORM_REVOCATION_CHECK)
	{
		CERT_REVOCATION_STATUS revStat;
		revStat.cbSize = sizeof(CERT_REVOCATION_STATUS);

		PCERT_CONTEXT* pCerts = certs.begin();
		BOOL rc = CertVerifyRevocation(
				X509_ASN_ENCODING,
				CERT_CONTEXT_REVOCATION_TYPE,
				pChainContext->cChain,
				(void **)pCerts,
				CERT_VERIFY_REV_CHAIN_FLAG,
				NULL,
				&revStat);

		if (!rc)
		{
			VerificationErrorArgs args(cert, revStat.dwIndex, revStat.dwReason, Utility::formatError(revStat.dwError));
			SSLManager::instance().ClientVerificationError(this, args);
			if (!args.getIgnoreError())
			{
				CertFreeCertificateChain(pChainContext);
				throw SSLException("Failed to verify revoked certificate chain");
			}
		}
	}
#endif
	CertFreeCertificateChain(pChainContext);
}


void SecureSocketImpl::serverVerifyCertificate(PCCERT_CONTEXT pPeerCert, DWORD dwCertFlags)
{
	if (_pContext->verificationMode() < Context::VERIFY_STRICT)
		return;	

	DWORD status = SEC_E_OK;

	X509Certificate cert(pPeerCert, true);
	// we are now in Strict mode
	if (pPeerCert == 0) throw SSLException("No client certificate");
	
	int iRc = CertVerifyTimeValidity(NULL, pPeerCert->pCertInfo);
	if (iRc != 0)
	{
		VerificationErrorArgs args(cert, 0, SEC_E_CERT_EXPIRED, "The certificate is expired");
		SSLManager::instance().ServerVerificationError(this, args);
		
		if (!args.getIgnoreError())
			throw SSLException("Expired certificate");
		else
			return;
	}
	
	PCCERT_CHAIN_CONTEXT pChainContext = NULL;
	CERT_CHAIN_PARA chainPara;
	ZeroMemory(&chainPara, sizeof(chainPara));
	chainPara.cbSize = sizeof(chainPara);

	if (!CertGetCertificateChain(
							NULL,
							pPeerCert,
							NULL,
							NULL,
							&chainPara,
							CERT_CHAIN_REVOCATION_CHECK_CHAIN,
							NULL,
							&pChainContext)) 
	{
		VerificationErrorArgs args(cert, 0, GetLastError(), "The certificate chain is expired");
		SSLManager::instance().ServerVerificationError(this, args);
		if (pChainContext) CertFreeCertificateChain(pChainContext);
		if (!args.getIgnoreError())
		{
			throw SSLException("The certificate chain is expired");
		}
		else return;
	}

	HTTPSPolicyCallbackData polHttps;
	ZeroMemory(&polHttps, sizeof(HTTPSPolicyCallbackData));
	polHttps.cbStruct       = sizeof(HTTPSPolicyCallbackData);
	polHttps.dwAuthType     = AUTHTYPE_CLIENT;
	polHttps.fdwChecks      = dwCertFlags;
	polHttps.pwszServerName = NULL;

	CERT_CHAIN_POLICY_PARA policyPara;
	memset(&policyPara, 0, sizeof(policyPara));
	policyPara.cbSize = sizeof(policyPara);
	policyPara.pvExtraPolicyPara = &polHttps;

	CERT_CHAIN_POLICY_STATUS policyStatus;
	memset(&policyStatus, 0, sizeof(policyStatus));
	policyStatus.cbSize = sizeof(policyStatus);

	if (!CertVerifyCertificateChainPolicy(CERT_CHAIN_POLICY_SSL, pChainContext, &policyPara, &policyStatus)) 
	{
		VerificationErrorArgs args(cert, 0, GetLastError(), "Failed to verify certificate chain");
		SSLManager::instance().ServerVerificationError(this, args);
		if (pChainContext) CertFreeCertificateChain(pChainContext);
		if (!args.getIgnoreError()) 
			throw SSLException("Failed to verify certificate chain");
		else
			return;
	}

	if (policyStatus.dwError) 
	{
		VerificationErrorArgs args(cert, policyStatus.lElementIndex, status, Utility::formatError(policyStatus.dwError));
		SSLManager::instance().ServerVerificationError(this, args);
		if (pChainContext) CertFreeCertificateChain(pChainContext);
		if (!args.getIgnoreError())
			throw SSLException("Failed to verify certificate chain");
		else 
			return;
	}

#if !defined(_WIN32_WCE)
	PCERT_CONTEXT *pCerts = new PCERT_CONTEXT[pChainContext->cChain];
	for (DWORD i = 0; i < pChainContext->cChain; i++) 
	{
		pCerts[i] = (PCERT_CONTEXT)(pChainContext->rgpChain[i]->rgpElement[0]->pCertContext);
	}
	
	CERT_REVOCATION_STATUS revStat;
	revStat.cbSize = sizeof(CERT_REVOCATION_STATUS);

	BOOL bRc = CertVerifyRevocation(
					X509_ASN_ENCODING,
					CERT_CONTEXT_REVOCATION_TYPE,
					pChainContext->cChain,
					(void **)pCerts,
					CERT_VERIFY_REV_CHAIN_FLAG,
					NULL,
					&revStat);
	if (!bRc) 
	{
		VerificationErrorArgs args(cert, revStat.dwIndex, revStat.dwReason, Utility::formatError(revStat.dwReason));
		SSLManager::instance().ServerVerificationError(this, args);
		if (!args.getIgnoreError())
		{
			CertFreeCertificateChain(pChainContext);
			delete [] pCerts;
			throw SSLException("Failed to verify certificate chain");
		}
	}

	delete [] pCerts;
#endif
	if (pChainContext) 
	{
		CertFreeCertificateChain(pChainContext);
	}
}


LONG SecureSocketImpl::clientDisconnect(PCredHandle phCreds, CtxtHandle* phContext)
{
	if (phContext->dwLower == 0 && phContext->dwUpper == 0)
	{
		// handshake has never been done
		poco_assert_dbg (_needHandshake);
		return SEC_E_OK;
	}

	AutoSecBufferDesc<1> tokBuffer(&_securityFunctions, false);

	DWORD dwType = SCHANNEL_SHUTDOWN;
	tokBuffer.setSecBufferToken(0, &dwType, sizeof(dwType));
	DWORD status = _securityFunctions.ApplyControlToken(phContext, &tokBuffer);

	if (FAILED(status)) return status;

	DWORD dwSSPIFlags = ISC_REQ_SEQUENCE_DETECT
                      | ISC_REQ_REPLAY_DETECT
	                  | ISC_REQ_CONFIDENTIALITY
	                  | ISC_RET_EXTENDED_ERROR
	                  | ISC_REQ_ALLOCATE_MEMORY
	                  | ISC_REQ_STREAM;

	AutoSecBufferDesc<1> outBuffer(&_securityFunctions, true);
	outBuffer.setSecBufferToken(0, 0, 0);

	DWORD dwSSPIOutFlags;
	TimeStamp tsExpiry;
	status = _securityFunctions.InitializeSecurityContextW(
				phCreds,
				phContext,
				NULL,
				dwSSPIFlags,
				0,
				0,
				NULL,
				0,
				phContext,
				&outBuffer,
				&dwSSPIOutFlags,
				&tsExpiry);

	return status;
}


LONG SecureSocketImpl::serverDisconnect(PCredHandle phCreds, CtxtHandle* phContext)
{
	if (phContext->dwLower == 0 && phContext->dwUpper == 0)
	{
		// handshake has never been done
		poco_assert_dbg (_needHandshake);
		return SEC_E_OK;
	}

	AutoSecBufferDesc<1> tokBuffer(&_securityFunctions, false);
	
	DWORD dwType = SCHANNEL_SHUTDOWN;
	tokBuffer.setSecBufferToken(0, &dwType, sizeof(dwType));
	DWORD status = _securityFunctions.ApplyControlToken(phContext, &tokBuffer);

	if (FAILED(status)) return status;

	DWORD dwSSPIFlags = ASC_REQ_SEQUENCE_DETECT
	                  | ASC_REQ_REPLAY_DETECT 
	                  | ASC_REQ_CONFIDENTIALITY 
	                  | ASC_REQ_EXTENDED_ERROR 
	                  | ASC_REQ_ALLOCATE_MEMORY
	                  | ASC_REQ_STREAM;

	AutoSecBufferDesc<1> outBuffer(&_securityFunctions, true);
	outBuffer.setSecBufferToken(0,0,0);

	DWORD dwSSPIOutFlags;
	TimeStamp tsExpiry;
	status = _securityFunctions.AcceptSecurityContext(
					phCreds,
					phContext,
					NULL,
					dwSSPIFlags,
					0,
					NULL,
					&outBuffer,
					&dwSSPIOutFlags,
					&tsExpiry);

	if (FAILED(status)) return status;

	PBYTE pbMessage = (BYTE *)(outBuffer[0].pvBuffer);
	DWORD cbMessage = outBuffer[0].cbBuffer;

	if (pbMessage != NULL && cbMessage != 0)
	{
		DWORD cbData = sendRawBytes(pbMessage, cbMessage);
		if (cbData == SOCKET_ERROR || cbData == 0)
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
	_hostName = _pSocket->address().host().toString();
	initClientContext();
	performInitialClientHandshake();
}


void SecureSocketImpl::stateMachine()
{
	StateMachine::instance().execute(this);
}


namespace
{
	static Poco::SingletonHolder<StateMachine> stateMachineSingleton;
}


StateMachine& StateMachine::instance()
{
	return *stateMachineSingleton.get();
}


bool StateMachine::readable(SOCKET sockfd)
{
	fd_set fdRead;
	FD_ZERO(&fdRead);
	FD_SET(sockfd, &fdRead);
	select(&fdRead, 0, sockfd);
	return (FD_ISSET(sockfd, &fdRead) != 0);
}


bool StateMachine::writable(SOCKET sockfd)
{
	fd_set fdWrite;
	FD_ZERO(&fdWrite);
	FD_SET(sockfd, &fdWrite);
	select(0, &fdWrite, sockfd);
	return (FD_ISSET(sockfd, &fdWrite) != 0);
}


bool StateMachine::readOrWritable(SOCKET sockfd)
{
	fd_set fdRead, fdWrite;
	FD_ZERO(&fdRead);
	FD_SET(sockfd, &fdRead);
	fdWrite = fdRead;
	select(&fdRead, &fdWrite, sockfd);
	return (FD_ISSET(sockfd, &fdRead) != 0 || FD_ISSET(sockfd, &fdWrite) != 0);
}


bool StateMachine::none(SOCKET sockfd)
{
	return true;
}


void StateMachine::select(fd_set* fdRead, fd_set* fdWrite, SOCKET sockfd)
{
	Poco::Timespan remainingTime(((Poco::Timestamp::TimeDiff)SecureSocketImpl::TIMEOUT_MILLISECS)*1000);
	int rc(0);
	do
	{
		struct timeval tv;
		tv.tv_sec  = (long) remainingTime.totalSeconds();
		tv.tv_usec = (long) remainingTime.useconds();
		Poco::Timestamp start;
		rc = ::select(int(sockfd) + 1, fdRead, fdWrite, 0, &tv);
		if (rc < 0 && SecureSocketImpl::lastError() == POCO_EINTR)
		{
			Poco::Timestamp end;
			Poco::Timespan waited = end - start;
			if (waited < remainingTime)
				remainingTime -= waited;
			else
				remainingTime = 0;
		}
	}
	while (rc < 0 && SecureSocketImpl::lastError() == POCO_EINTR);
}


StateMachine::StateMachine():
	_states()
{
	//ST_INITIAL: 0, -> this one is illegal, you must call connectNB before
	_states.push_back(std::make_pair(&StateMachine::none, &SecureSocketImpl::stateIllegal));
	//ST_CONNECTING: connectNB was called, check if the socket is already available for writing
	_states.push_back(std::make_pair(&StateMachine::writable, &SecureSocketImpl::stateConnected));
	//ST_ESTABLISHTUNNELRECEIVED: we got the response, now start the handshake
	_states.push_back(std::make_pair(&StateMachine::writable, &SecureSocketImpl::performInitialClientHandshake));
	//ST_CLIENTHANDSHAKECONDREAD: condread
	_states.push_back(std::make_pair(&StateMachine::readable, &SecureSocketImpl::performClientHandshakeLoopCondRead));
	//ST_CLIENTHANDSHAKEINCOMPLETE,
	_states.push_back(std::make_pair(&StateMachine::none, &SecureSocketImpl::performClientHandshakeLoopIncompleteMessage));
	//ST_CLIENTHANDSHAKEOK,
	_states.push_back(std::make_pair(&StateMachine::writable, &SecureSocketImpl::performClientHandshakeLoopOK));
	//ST_CLIENTHANDSHAKEEXTERROR,
	_states.push_back(std::make_pair(&StateMachine::writable, &SecureSocketImpl::performClientHandshakeLoopExtError));
	//ST_CLIENTHANDSHAKECONTINUE,
	_states.push_back(std::make_pair(&StateMachine::writable, &SecureSocketImpl::performClientHandshakeLoopContinueNeeded));
	//ST_VERIFY,
	_states.push_back(std::make_pair(&StateMachine::none, &SecureSocketImpl::clientConnectVerify));
	//ST_DONE,
	_states.push_back(std::make_pair(&StateMachine::none, &SecureSocketImpl::stateIllegal));
	//ST_ERROR
	_states.push_back(std::make_pair(&StateMachine::none, &SecureSocketImpl::performClientHandshakeLoopError));
}


void StateMachine::execute(SecureSocketImpl* pSock)
{
	try
	{
		poco_assert_dbg (pSock);
		ConditionState& state = _states[pSock->getState()];
		ConditionMethod& meth = state.first;
		if ((this->*state.first)(pSock->sockfd()))
		{
			(pSock->*(state.second))();
			(pSock->getState() == SecureSocketImpl::ST_DONE);
		}
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
