//
// SecureSocketImpl.cpp
//
// Library: NetSSL_OpenSSL
// Package: SSLSockets
// Module:  SecureSocketImpl
//
// Copyright (c) 2006-2010, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/SecureSocketImpl.h"
#include "Poco/Net/SSLException.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Net/Context.h"
#include "Poco/Net/X509Certificate.h"
#include "Poco/Net/Utility.h"
#include "Poco/Net/SecureStreamSocket.h"
#include "Poco/Net/SecureStreamSocketImpl.h"
#include "Poco/Net/StreamSocketImpl.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/NumberFormatter.h"
#include "Poco/Format.h"
#include <openssl/x509v3.h>
#include <openssl/err.h>


using Poco::IOException;
using Poco::TimeoutException;
using Poco::InvalidArgumentException;
using Poco::NumberFormatter;
using Poco::Timespan;


namespace Poco {
namespace Net {


SecureSocketImpl::SecureSocketImpl(Poco::AutoPtr<SocketImpl> pSocketImpl, Context::Ptr pContext):
	_pSSL(nullptr),
	_pSocket(pSocketImpl),
	_pContext(pContext),
	_needHandshake(false)
{
	poco_check_ptr (_pSocket);
	poco_check_ptr (_pContext);
}


SecureSocketImpl::~SecureSocketImpl()
{
	try
	{
		close();
		reset();
	}
	catch (...)
	{
		poco_unexpected();
	}
}


SocketImpl* SecureSocketImpl::acceptConnection(SocketAddress& clientAddr)
{
	poco_assert (!_pSSL);

	StreamSocket ss = _pSocket->acceptConnection(clientAddr);
	Poco::AutoPtr<SecureStreamSocketImpl> pSecureStreamSocketImpl = new SecureStreamSocketImpl(static_cast<StreamSocketImpl*>(ss.impl()), _pContext);
	pSecureStreamSocketImpl->acceptSSL();
	pSecureStreamSocketImpl->duplicate();
	return pSecureStreamSocketImpl;
}


void SecureSocketImpl::acceptSSL()
{
	poco_assert (!_pSSL);

	LockT l(_mutex);

	BIO* pBIO = ::BIO_new(BIO_s_socket());
	if (!pBIO) throw SSLException("Cannot create BIO object");
	BIO_set_fd(pBIO, static_cast<int>(_pSocket->sockfd()), BIO_NOCLOSE);

	_pSSL = ::SSL_new(_pContext->sslContext());
	if (!_pSSL)
	{
		::BIO_free(pBIO);
		throw SSLException("Cannot create SSL object");
	}

#if OPENSSL_VERSION_NUMBER >= 0x1010100fL
	/* TLS 1.3 server sends session tickets after a handhake as part of
	* the SSL_accept(). If a client finishes all its job before server
	* sends the tickets, SSL_accept() fails with EPIPE errno. Since we
	* are not interested in a session resumption, we can not to send the
	* tickets. */
	if (1 != SSL_set_num_tickets(_pSSL, 0))
	{
		::BIO_free(pBIO);
		throw SSLException("Cannot create SSL object");
	}
	//Otherwise we can perform two-way shutdown. Client must call SSL_read() before the final SSL_shutdown().
#endif

	::SSL_set_bio(_pSSL, pBIO, pBIO);
	::SSL_set_accept_state(_pSSL);
	::SSL_set_ex_data(_pSSL, SSLManager::instance().socketIndex(), this);
	_needHandshake = true;
}


void SecureSocketImpl::connect(const SocketAddress& address, bool performHandshake)
{
	if (_pSSL) reset();

	poco_assert (!_pSSL);

	_pSocket->connect(address);
	connectSSL(performHandshake);
}


void SecureSocketImpl::connect(const SocketAddress& address, const Poco::Timespan& timeout, bool performHandshake)
{
	if (_pSSL) reset();

	poco_assert (!_pSSL);

	_pSocket->connect(address, timeout);
	Poco::Timespan receiveTimeout = _pSocket->getReceiveTimeout();
	Poco::Timespan sendTimeout = _pSocket->getSendTimeout();
	_pSocket->setReceiveTimeout(timeout);
	_pSocket->setSendTimeout(timeout);
	connectSSL(performHandshake);
	_pSocket->setReceiveTimeout(receiveTimeout);
	_pSocket->setSendTimeout(sendTimeout);
}


void SecureSocketImpl::connectNB(const SocketAddress& address)
{
	if (_pSSL) reset();

	poco_assert (!_pSSL);

	_pSocket->connectNB(address);
	connectSSL(false);
}


void SecureSocketImpl::connectSSL(bool performHandshake)
{
	poco_assert (!_pSSL);
	poco_assert (_pSocket->initialized());

	LockT l(_mutex);

	::BIO* pBIO = ::BIO_new(BIO_s_socket());
	if (!pBIO) throw SSLException("Cannot create SSL BIO object");
	BIO_set_fd(pBIO, static_cast<int>(_pSocket->sockfd()), BIO_NOCLOSE);

	_pSSL = ::SSL_new(_pContext->sslContext());
	if (!_pSSL)
	{
		::BIO_free(pBIO);
		throw SSLException("Cannot create SSL object");
	}
	::SSL_set_bio(_pSSL, pBIO, pBIO);
	::SSL_set_ex_data(_pSSL, SSLManager::instance().socketIndex(), this);

	if (!_peerHostName.empty())
	{
		SSL_set_tlsext_host_name(_pSSL, _peerHostName.c_str());
	}

#if OPENSSL_VERSION_NUMBER >= 0x10001000L
	if(_pContext->ocspStaplingResponseVerificationEnabled())
	{
		SSL_set_tlsext_status_type(_pSSL, TLSEXT_STATUSTYPE_ocsp);
	}
#endif

	if (_pSession && _pSession->isResumable())
	{
		::SSL_set_session(_pSSL, _pSession->sslSession());
	}

	try
	{
		if (performHandshake && _pSocket->getBlocking())
		{
			int ret = ::SSL_connect(_pSSL);
			handleError(ret);
			verifyPeerCertificate();
		}
		else
		{
			::SSL_set_connect_state(_pSSL);
			_needHandshake = true;
		}
	}
	catch (...)
	{
		::SSL_free(_pSSL);
		_pSSL = nullptr;
		throw;
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
	poco_check_ptr (_pSocket);

	_pSocket->listen(backlog);
}


void SecureSocketImpl::shutdown()
{
	if (_pSSL)
	{
		UnLockT l(_mutex);

		// Don't shut down the socket more than once.
		int shutdownState = ::SSL_get_shutdown(_pSSL);
		bool shutdownSent = (shutdownState & SSL_SENT_SHUTDOWN) == SSL_SENT_SHUTDOWN;
		if (!shutdownSent)
		{
			// A proper clean shutdown would require us to
			// retry the shutdown if we get a zero return
			// value, until SSL_shutdown() returns 1.
			// However, this will lead to problems with
			// most web browsers, so we just set the shutdown
			// flag by calling SSL_shutdown() once and be
			// done with it.
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
			int rc = 0;
			if (!_bidirectShutdown)
				rc = ::SSL_shutdown(_pSSL);
			else
			{
				Poco::Timespan recvTimeout = _pSocket->getReceiveTimeout();
				Poco::Timespan pollTimeout(0, 100000);
				Poco::Timestamp tsNow;
				do
				{
					rc = ::SSL_shutdown(_pSSL);
					if (rc == 1) break;
					if (rc < 0)
					{
						int err = ::SSL_get_error(_pSSL, rc);
						if (err == SSL_ERROR_WANT_READ)
							_pSocket->poll(pollTimeout, Poco::Net::Socket::SELECT_READ);
						else if (err == SSL_ERROR_WANT_WRITE)
							_pSocket->poll(pollTimeout, Poco::Net::Socket::SELECT_WRITE);
						else
						{
							int socketError = SocketImpl::lastError();
							long lastError = ::ERR_get_error();
							if ((err == SSL_ERROR_SSL) && (socketError == 0) && (lastError == 0x0A000123))
								rc = 0;
							break;
						}
					}
					else _pSocket->poll(pollTimeout, Poco::Net::Socket::SELECT_READ);
				} while (!tsNow.isElapsed(recvTimeout.totalMicroseconds()));
			}
#else
			int rc = ::SSL_shutdown(_pSSL);
#endif
			if (rc < 0) handleError(rc);

			l.unlock();

			if (_pSocket->getBlocking())
			{
				_pSocket->shutdown();
			}
		}
	}
}


void SecureSocketImpl::close()
{
	try
	{
		shutdown();
	}
	catch (...)
	{
	}
	_pSocket->close();
}


void SecureSocketImpl::setBlocking(bool flag)
{
	poco_check_ptr (_pSocket);

	_pSocket->setBlocking(flag);
}


bool SecureSocketImpl::getBlocking() const
{
	poco_check_ptr (_pSocket);

	return _pSocket->getBlocking();
}


int SecureSocketImpl::sendBytes(const void* buffer, int length, int flags)
{
	poco_assert (_pSocket->initialized());
	poco_check_ptr (_pSSL);

	int rc;

	LockT l(_mutex);

	if (_needHandshake)
	{
		rc = completeHandshake();
		if (rc == 1)
			verifyPeerCertificate();
		else if (rc == 0)
			throw SSLConnectionUnexpectedlyClosedException();
		else
			return rc;
	}
	const auto sendTimeout = _pSocket->getSendTimeout();
	Poco::Timestamp tsStart;
	while (true)
	{
		rc = ::SSL_write(_pSSL, buffer, length);
		if (!mustRetry(rc))
			break;

		if (tsStart.isElapsed(sendTimeout.totalMicroseconds()))
			throw Poco::TimeoutException();
	};
	if (rc <= 0)
	{
		rc = handleError(rc);
		if (rc == 0) throw SSLConnectionUnexpectedlyClosedException();
	}
	return rc;
}


int SecureSocketImpl::receiveBytes(void* buffer, int length, int flags)
{
	poco_assert (_pSocket->initialized());
	poco_check_ptr (_pSSL);

	int rc;

	LockT l(_mutex);

	if (_needHandshake)
	{
		rc = completeHandshake();
		if (rc == 1)
			verifyPeerCertificate();
		else
			return rc;
	}

	const auto recvTimeout = _pSocket->getReceiveTimeout();
	Poco::Timestamp tsStart;
	while (true)
	{
		rc = ::SSL_read(_pSSL, buffer, length);
		if (!mustRetry(rc))
			break;

		if (tsStart.isElapsed(recvTimeout.totalMicroseconds()))
			throw Poco::TimeoutException();
	};
	_bidirectShutdown = false;
	if (rc <= 0)
	{
		return handleError(rc);
	}
	return rc;
}


int SecureSocketImpl::available() const
{
	poco_check_ptr (_pSSL);

	LockT l(_mutex);

	return ::SSL_pending(_pSSL);
}


int SecureSocketImpl::completeHandshake()
{
	poco_assert (_pSocket->initialized());
	poco_check_ptr (_pSSL);

	int rc;
	const auto recvTimeout = _pSocket->getReceiveTimeout();
	Poco::Timestamp tsStart;
	while (true)
	{
		rc = ::SSL_do_handshake(_pSSL);
		if (!mustRetry(rc))
			break;

		if (tsStart.isElapsed(recvTimeout.totalMicroseconds()))
			throw Poco::TimeoutException();
	};
	if (rc <= 0)
	{
		return handleError(rc);
	}
	_needHandshake = false;
	return rc;
}


void SecureSocketImpl::verifyPeerCertificate()
{
	if (_peerHostName.empty())
		verifyPeerCertificate(_pSocket->peerAddress().host().toString());
	else
		verifyPeerCertificate(_peerHostName);
}


void SecureSocketImpl::verifyPeerCertificate(const std::string& hostName)
{
	long certErr = verifyPeerCertificateImpl(hostName);
	if (certErr != X509_V_OK)
	{
		std::string msg = Utility::convertCertificateError(certErr);
		throw CertificateValidationException("Unacceptable certificate from " + hostName, msg);
	}
}


long SecureSocketImpl::verifyPeerCertificateImpl(const std::string& hostName)
{
	Context::VerificationMode mode = _pContext->verificationMode();
	if (mode == Context::VERIFY_NONE || !_pContext->extendedCertificateVerificationEnabled() ||
	   (mode != Context::VERIFY_STRICT && isLocalHost(hostName)))
	{
		return X509_V_OK;
	}

	::X509* pCert = ::SSL_get_peer_certificate(_pSSL);
	if (pCert)
	{
		X509Certificate cert(pCert);
		return cert.verify(hostName) ? X509_V_OK : X509_V_ERR_APPLICATION_VERIFICATION;
	}
	else return X509_V_OK;
}


bool SecureSocketImpl::isLocalHost(const std::string& hostName)
{
	try
	{
		SocketAddress addr(hostName, 0);
		return addr.host().isLoopback();
	}
	catch (const Poco::Exception&)
	{
		return false;
	}
}


X509* SecureSocketImpl::peerCertificate() const
{
	LockT l(_mutex);

	X509* pCert = nullptr;

	if (_pSSL)
		return ::SSL_get_peer_certificate(_pSSL);
	else
		return nullptr;
}


bool SecureSocketImpl::mustRetry(int rc)
{
	if (rc <= 0)
	{
		static const Poco::Timespan pollTimeout(0, 100000);

		int sslError = ::SSL_get_error(_pSSL, rc);
		int socketError = _pSocket->lastError();
		switch (sslError)
		{
		case SSL_ERROR_WANT_READ:
			if (_pSocket->getBlocking())
			{
				_pSocket->poll(pollTimeout, Poco::Net::Socket::SELECT_READ);
				return true;
			}
			break;
		case SSL_ERROR_WANT_WRITE:
			if (_pSocket->getBlocking())
			{
				_pSocket->poll(pollTimeout, Poco::Net::Socket::SELECT_WRITE);
				return true;
			}
			break;
		case SSL_ERROR_SYSCALL:
			return socketError == POCO_EAGAIN || socketError == POCO_EINTR;
		default:
			return socketError == POCO_EINTR;
		}
	}
	return false;
}


int SecureSocketImpl::handleError(int rc)
{
	if (rc > 0) return rc;

	int sslError = ::SSL_get_error(_pSSL, rc);
	int socketError = SocketImpl::lastError();

	switch (sslError)
	{
	case SSL_ERROR_ZERO_RETURN:
		return 0;
	case SSL_ERROR_WANT_READ:
		return SecureStreamSocket::ERR_SSL_WANT_READ;
	case SSL_ERROR_WANT_WRITE:
		return SecureStreamSocket::ERR_SSL_WANT_WRITE;
	case SSL_ERROR_WANT_CONNECT:
	case SSL_ERROR_WANT_ACCEPT:
	case SSL_ERROR_WANT_X509_LOOKUP:
		// these should not occur
		poco_bugcheck();
		return rc;
	// SSL_GET_ERROR(3ossl):
	// On an unexpected EOF, versions before OpenSSL 3.0 returned
	// SSL_ERROR_SYSCALL, nothing was added to the error stack, and
	// errno was 0.  Since OpenSSL 3.0 the returned error is
	// SSL_ERROR_SSL with a meaningful error on the error stack.
	// However, we still need to check for socket errors in both
	// cases with OpenSSL 3.0 or later.
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
	case SSL_ERROR_SSL:
		// fallthrough to handle socket errors first
#endif
	case SSL_ERROR_SYSCALL:
		if (socketError)
		{
			SocketImpl::error(socketError);
		}
		// fallthrough
	default:
		{
		long lastError = ::ERR_get_error();
			std::string msg;
			if (lastError)
			{
				char buffer[256];
				::ERR_error_string_n(lastError, buffer, sizeof(buffer));
				msg = buffer;
			}
			// SSL_GET_ERROR(3ossl):
			// On an unexpected EOF, versions before OpenSSL 3.0 returned
			// SSL_ERROR_SYSCALL, nothing was added to the error stack, and
			// errno was 0.  Since OpenSSL 3.0 the returned error is
			// SSL_ERROR_SSL with a meaningful error on the error stack.
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
			if (sslError == SSL_ERROR_SSL)
#else
			if (lastError == 0)
#endif
			{
				if (rc == 0)
				{
					// Most web browsers do this, don't report an error
					if (_pContext->isForServerUse())
						return 0;
					else
						throw SSLConnectionUnexpectedlyClosedException(msg);
				}
				else if (rc == -1)
				{
					throw SSLConnectionUnexpectedlyClosedException(msg);
				}
				else
				{
					SecureStreamSocketImpl::error(Poco::format("The BIO reported an error: %d", rc));
				}
			}
			else if (lastError)
			{
				throw SSLException(msg);
			}
		}
 		break;
	}
	return rc;
}


void SecureSocketImpl::setPeerHostName(const std::string& peerHostName)
{
	_peerHostName = peerHostName;
}


void SecureSocketImpl::reset()
{
	if (_pSSL)
	{
		LockT l(_mutex);

		::SSL_set_ex_data(_pSSL, SSLManager::instance().socketIndex(), nullptr);
		::SSL_free(_pSSL);
		_pSSL = nullptr;
	}
}


void SecureSocketImpl::abort()
{
	_pSocket->shutdown();
}


Session::Ptr SecureSocketImpl::currentSession()
{
	return _pSession;
}


void SecureSocketImpl::useSession(Session::Ptr pSession)
{
	_pSession = pSession;
}


bool SecureSocketImpl::sessionWasReused()
{
	if (_pSSL)
	{
		LockT l(_mutex);
		return ::SSL_session_reused(_pSSL) != 0;
	}

	return false;
}


int SecureSocketImpl::onSessionCreated(SSL* pSSL, SSL_SESSION* pSession)
{
	void* pEx = ::SSL_get_ex_data(pSSL, SSLManager::instance().socketIndex());
	if (pEx)
	{
		SecureSocketImpl* pThis = reinterpret_cast<SecureSocketImpl*>(pEx);
		pThis->_pSession = new Session(pSession);
		return 1;
	}
	else return 0;
}


} } // namespace Poco::Net
