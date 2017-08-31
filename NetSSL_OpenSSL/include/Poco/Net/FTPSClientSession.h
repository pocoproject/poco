#pragma once
#include "Poco/Net/FTPClientSession.h"

namespace Poco {
namespace Net {

class FTPSClientSession :
	public Poco::Net::FTPClientSession
{
public:
	FTPSClientSession();
	/// Creates an FTPSClientSession.
	///
	/// Passive mode will be used for data transfers.

	explicit FTPSClientSession(const StreamSocket& socket);
	/// Creates an FTPSClientSession using the given
	/// connected socket for the control connection.
	///
	/// Passive mode will be used for data transfers.

	FTPSClientSession(const std::string& host,
		Poco::UInt16 port = FTP_PORT,
		const std::string& username = "",
		const std::string& password = "");
	/// Creates an FTPSClientSession using a socket connected
	/// to the given host and port. If username is supplied,
	/// login is attempted.
	///
	/// Passive mode will be used for data transfers.

	virtual ~FTPSClientSession();	

	void tryFTPSmode(bool bTryFTPS);
		/// avoid or require TLS mode

	bool isSecure() const;
		/// Returns true if the session is FTPS.

protected:
	virtual StreamSocket establishDataConnection(const std::string& command, const std::string& arg);
		/// Create secure data connection

	virtual void receiveServerReadyReply();
		/// Function that read server welcome message after connetion and set and make secure socket

private:
	bool _bTryFTPS = true;

	void beforeCreateDataSocket();
		///Send commands to check if we can encrypt data socket

	void afterCreateControlSocket();
		///Send commands to make SSL negotiating of control channel

	bool _bSecureDataConnection = false;
};

inline bool FTPSClientSession::isSecure() const
{
	if (_pControlSocket != nullptr)
		return _pControlSocket->secure();
	return false;
}

}} // namespace Poco::Net
