#ifndef Net_HTTPReactorServerSession_INCLUDED
#define Net_HTTPReactorServerSession_INCLUDED
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPSession.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/StreamSocket.h"
#include <cstring>
#include <string>
namespace Poco {
namespace Net {


class Net_API HTTPReactorServerSession : public HTTPSession
/// This class handles the server side of a
/// HTTP session. It is used internally by
/// HTTPServer.
{
public:
	HTTPReactorServerSession(const StreamSocket& socket, std::string& buf, HTTPServerParams::Ptr pParams);
	/// Creates the HTTPReactorServerSession.

	virtual ~HTTPReactorServerSession();
	/// Destroys the HTTPReactorServerSession.

	SocketAddress clientAddress() override
	{
		return _realsocket.peerAddress();
	}
	/// Returns the client's address.

	SocketAddress serverAddress() override
	{
		return _realsocket.address();
	}
	/// Returns the server's address.

	bool checkRequestComplete();

	void popCompletedRequest();

private:
	int get() override;

	int peek() override;

	int write(const char* buffer, std::streamsize length) override;

private:
	bool           _firstRequest;
	Poco::Timespan _keepAliveTimeout;
	int            _maxKeepAliveRequests;
	std::string&   _buf;
	char*          _pcur;
	char*          _pend;
	int            _idx;
	int            _complete;
	StreamSocket   _realsocket;
};

}} // namespace Poco::Net

#endif // Net_HTTPReactorServerSession_INCLUDED

