//
// SecureSocketImpl.h
//
// Library: NetSSL_Win
// Package: SSLSockets
// Module:  SecureSocketImpl
//
// Definition of the SecureSocketImpl class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef NetSSL_SecureSocketImpl_INCLUDED
#define NetSSL_SecureSocketImpl_INCLUDED


// Temporary debugging aid, to be removed
// #define ENABLE_PRINT_STATE


#include "Poco/Net/SocketImpl.h"
#include "Poco/Net/NetSSL.h"
#include "Poco/Net/Context.h"
#include "Poco/Net/AutoSecBufferDesc.h"
#include "Poco/Net/X509Certificate.h"
#include "Poco/Buffer.h"
#include <winsock2.h>
#include <windows.h>
#include <wincrypt.h>
#include <schannel.h>
#ifndef SECURITY_WIN32
#define SECURITY_WIN32
#endif
#include <security.h>
#include <sspi.h>



#ifdef ENABLE_PRINT_STATE
#define PRINT_STATE(m) printState(m)
#else
#define PRINT_STATE(m)
#endif


namespace Poco {
namespace Net {


class NetSSL_Win_API SecureSocketImpl
	/// The SocketImpl for SecureStreamSocket.
{
public:
	enum Mode
	{
		MODE_CLIENT,
		MODE_SERVER
	};

	SecureSocketImpl(Poco::AutoPtr<SocketImpl> pSocketImpl, Context::Ptr pContext);
		/// Creates the SecureSocketImpl.

	virtual ~SecureSocketImpl();
		/// Destroys the SecureSocketImpl.

	SocketImpl* acceptConnection(SocketAddress& clientAddr);
		/// Get the next completed connection from the
		/// socket's completed connection queue.
		///
		/// If the queue is empty, waits until a connection
		/// request completes.
		///
		/// Returns a new TCP socket for the connection
		/// with the client.
		///
		/// The client socket's address is returned in clientAddr.

	void connect(const SocketAddress& address, bool performHandshake);
		/// Initializes the socket and establishes a connection to
		/// the TCP server at the given address.
		///
		/// Can also be used for UDP sockets. In this case, no
		/// connection is established. Instead, incoming and outgoing
		/// packets are restricted to the specified address.

	void connect(const SocketAddress& address, const Poco::Timespan& timeout, bool performHandshake);
		/// Initializes the socket, sets the socket timeout and
		/// establishes a connection to the TCP server at the given address.

	void connectNB(const SocketAddress& address);
		/// Initializes the socket and establishes a connection to
		/// the TCP server at the given address. Prior to opening the
		/// connection the socket is set to nonblocking mode.

	void bind(const SocketAddress& address, bool reuseAddress = false);
		/// Bind a local address to the socket.
		///
		/// This is usually only done when establishing a server
		/// socket. TCP clients should not bind a socket to a
		/// specific address.
		///
		/// If reuseAddress is true, sets the SO_REUSEADDR
		/// socket option.

	void bind(const SocketAddress& address, bool reuseAddress, bool reusePort);
		/// Bind a local address to the socket.
		///
		/// This is usually only done when establishing a server
		/// socket. SSL clients should not bind a socket to a
		/// specific address.
		///
		/// If reuseAddress is true, sets the SO_REUSEADDR
		/// socket option.
		///
		/// If reusePort is true, sets the SO_REUSEPORT
		/// socket option.

	void bind6(const SocketAddress& address, bool reuseAddress = false, bool ipV6Only = false);
		/// Bind a local IPv6 address to the socket.
		///
		/// This is usually only done when establishing a server
		/// socket. TCP clients should not bind a socket to a
		/// specific address.
		///
		/// If reuseAddress is true, sets the SO_REUSEADDR
		/// socket option.
		///
		/// The given address must be an IPv6 address. The
		/// IPPROTO_IPV6/IPV6_V6ONLY option is set on the socket
		/// according to the ipV6Only parameter.
		///
		/// If the library has not been built with IPv6 support,
		/// a Poco::NotImplementedException will be thrown.

	void bind6(const SocketAddress& address, bool reuseAddress, bool reusePort, bool ipV6Only);
		/// Bind a local IPv6 address to the socket.
		///
		/// This is usually only done when establishing a server
		/// socket. TCP clients should not bind a socket to a
		/// specific address.
		///
		/// If reuseAddress is true, sets the SO_REUSEADDR
		/// socket option.
		///
		/// If reusePort is true, sets the SO_REUSEPORT
		/// socket option.
		///
		/// The given address must be an IPv6 address. The
		/// IPPROTO_IPV6/IPV6_V6ONLY option is set on the socket
		/// according to the ipV6Only parameter.
		///
		/// If the library has not been built with IPv6 support,
		/// a Poco::NotImplementedException will be thrown.

	void listen(int backlog = 64);
		/// Puts the socket into listening state.
		///
		/// The socket becomes a passive socket that
		/// can accept incoming connection requests.
		///
		/// The backlog argument specifies the maximum
		/// number of connections that can be queued
		/// for this socket.

	int shutdown();
		/// Shuts down the connection by attempting
		/// an orderly SSL shutdown, then actually
		/// shutting down the TCP connection in the
		/// send direction.

	void close();
		/// Close the socket.

	void abort();
		/// Aborts the connection by closing the
		/// underlying TCP connection. No orderly SSL shutdown
		/// is performed.

	int sendBytes(const void* buffer, int length, int flags = 0);
		/// Sends the contents of the given buffer through
		/// the socket. Any specified flags are ignored.
		///
		/// Returns the number of bytes sent, which may be
		/// less than the number of bytes specified.

	int receiveBytes(void* buffer, int length, int flags = 0);
		/// Receives data from the socket and stores it
		/// in buffer. Up to length bytes are received.
		///
		/// Returns the number of bytes received.

	void setPeerHostName(const std::string& hostName);
		/// Sets the peer host name for certificate validation purposes.

	const std::string& getPeerHostName() const;
		/// Returns the peer host name.

	void verifyPeerCertificate();
		/// Performs post-connect (or post-accept) peer certificate validation,
		/// using the peer host name set with setPeerHostName(), or the peer's
		/// IP address string if no peer host name has been set.

	void verifyPeerCertificate(const std::string& hostName);
		/// Performs post-connect (or post-accept) peer certificate validation
		/// using the given peer host name.

	Context::Ptr context() const;
		/// Returns the Context.

	PCCERT_CONTEXT peerCertificate() const;
		/// Returns the peer certificate.

	poco_socket_t sockfd();
		/// Returns the underlying socket descriptor.

	int available() const;
		/// Returns the number of bytes available in the buffer.

	SocketImpl* socket();
		/// Returns the underlying SocketImpl.
		
	const SocketImpl* socket() const;
		/// Returns the underlying SocketImpl.

protected:
	enum
	{
		IO_BUFFER_SIZE    = 32768,
		TIMEOUT_MILLISECS = 200
	};

	enum State
	{
		ST_INITIAL = 0,
		ST_CONNECTING,
		ST_CLIENT_HSK_START,
		ST_CLIENT_HSK_SEND_TOKEN,
		ST_CLIENT_HSK_LOOP_INIT,
		ST_CLIENT_HSK_LOOP_RECV,
		ST_CLIENT_HSK_LOOP_PROCESS,
		ST_CLIENT_HSK_LOOP_SEND,
		ST_CLIENT_HSK_LOOP_DONE,
		ST_CLIENT_HSK_SEND_FINAL,
		ST_CLIENT_HSK_SEND_ERROR,
		ST_CLIENT_VERIFY,
		ST_ACCEPTING,
		ST_SERVER_HSK_START,
		ST_SERVER_HSK_LOOP_INIT,
		ST_SERVER_HSK_LOOP_RECV,
		ST_SERVER_HSK_LOOP_PROCESS,
		ST_SERVER_HSK_LOOP_SEND,
		ST_SERVER_HSK_LOOP_DONE,
		ST_SERVER_VERIFY,
		ST_DONE,
		ST_ERROR,
		ST_MAX
	};

	enum TLSShutdown
	{
		TLS_SHUTDOWN_SENT = 1,
		TLS_SHUTDOWN_RECEIVED = 2
	};

	int sendRawBytes(const void* buffer, int length, int flags = 0);
	int receiveRawBytes(void* buffer, int length, int flags = 0);
	void clientVerifyCertificate(const std::string& hostName);
	void verifyCertificateChainClient(PCCERT_CONTEXT pServerCert);
	void serverVerifyCertificate();
	int serverShutdown(PCredHandle phCreds, CtxtHandle* phContext);
	int clientShutdown(PCredHandle phCreds, CtxtHandle* phContext);
	PCCERT_CONTEXT loadCertificate(bool mustFindCertificate);
	void initCommon();
	void cleanup();

	void stateIllegal();
	void stateError();

	void stateClientConnected();
	void stateClientHandshakeStart();
	void stateClientHandshakeSendToken();
	void stateClientHandshakeLoopInit();
	void stateClientHandshakeLoopRecv();
	void stateClientHandshakeLoopProcess();
	void stateClientHandshakeLoopSend();
	void stateClientHandshakeLoopDone();
	void stateClientHandshakeSendFinal();
	void stateClientHandshakeSendError();
	void stateClientVerify();

	void stateServerAccepted();
	void stateServerHandshakeStart();
	void stateServerHandshakeLoopInit();
	void stateServerHandshakeLoopRecv();
	void stateServerHandshakeLoopProcess();
	void stateServerHandshakeLoopSend();
	void stateServerHandshakeLoopDone();
	void stateServerHandshakeVerify();

	void sendOutSecBufferAndAdvanceState(State state);
	void drainExtraBuffer();
	static int getRecordLength(const BYTE* pBuffer, int length);
	static bool bufferHasCompleteRecords(const BYTE* pBuffer, int length);

	void initClientCredentials();
	void initServerCredentials();
	SECURITY_STATUS doHandshake();
	int completeHandshake();

	SECURITY_STATUS decodeMessage(BYTE* pBuffer, DWORD bufSize, AutoSecBufferDesc<4>& msg, SecBuffer*& pData, SecBuffer*& pExtra);
	SECURITY_STATUS decodeBufferFull(BYTE* pBuffer, DWORD bufSize, char* pOutBuffer, int outLength, int& bytesDecoded);

	void acceptSSL();
	void connectSSL(bool completeHandshake);
	static int lastError();
	bool stateMachine();
	State getState() const;
	void setState(State st);
	static int stateToReturnValue(State state);
	static bool isLocalHost(const std::string& hostName);

#ifdef ENABLE_PRINT_STATE
	void printState(const std::string& msg);
#endif

private:
	SecureSocketImpl(const SecureSocketImpl&);
	SecureSocketImpl& operator = (const SecureSocketImpl&);

	Poco::AutoPtr<SocketImpl> _pSocket;
	Context::Ptr   _pContext;
	Mode           _mode;
	int            _shutdownFlags;
	std::string    _peerHostName;
	bool           _useMachineStore;
	bool           _clientAuthRequired;

	SecurityFunctionTableW& _securityFunctions;

	PCCERT_CONTEXT _pOwnCertificate;
	PCCERT_CONTEXT _pPeerCertificate;

	CredHandle _hCreds;
	CtxtHandle _hContext;
	DWORD      _contextFlags;

	Poco::Buffer<BYTE> _overflowBuffer;
	Poco::Buffer<BYTE> _sendBuffer;
	DWORD _sendBufferOffset;
	DWORD _sendBufferPending;
	Poco::Buffer<BYTE> _recvBuffer;
	DWORD _recvBufferOffset;
	DWORD _ioBufferSize;

	SecPkgContext_StreamSizes _streamSizes;
	AutoSecBufferDesc<1> _outSecBuffer;
	AutoSecBufferDesc<2> _inSecBuffer;
	SecBuffer _extraSecBuffer;
	SECURITY_STATUS _securityStatus;
	State _state;
	bool _needData;
	bool _needHandshake;
	bool _initServerContext = false;

	friend class SecureStreamSocketImpl;
	friend class StateMachine;
};


//
// inlines
//
inline SocketImpl* SecureSocketImpl::socket()
{
	return _pSocket.get();
}


inline const SocketImpl* SecureSocketImpl::socket() const
{
	return _pSocket.get();
}


inline poco_socket_t SecureSocketImpl::sockfd()
{
	return _pSocket->sockfd();
}


inline Context::Ptr SecureSocketImpl::context() const
{
	return _pContext;
}


inline SecureSocketImpl::State SecureSocketImpl::getState() const
{
	return _state;
}


inline void SecureSocketImpl::setState(SecureSocketImpl::State st)
{
	_state = st;
	PRINT_STATE("setState: ");
}


inline const std::string& SecureSocketImpl::getPeerHostName() const
{
	return _peerHostName;
}


inline PCCERT_CONTEXT SecureSocketImpl::peerCertificate() const
{
	return _pPeerCertificate;
}


inline int SecureSocketImpl::lastError()
{
	return SocketImpl::lastError();
}


} } // namespace Poco::Net


#endif // NetSSL_SecureSocketImpl_INCLUDED
