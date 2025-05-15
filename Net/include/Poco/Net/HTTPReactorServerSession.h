#ifndef Net_HTTPReactorServerSession_INCLUDED
#define Net_HTTPReactorServerSession_INCLUDED
#include "Poco/Logger.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/HTTPSession.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/StreamSocket.h"
#include <cstring>
#include <string>
namespace Poco {
	namespace Net {
	
	
	class Net_API HTTPReactorServerSession: public HTTPSession
		/// This class handles the server side of a
		/// HTTP session. It is used internally by
		/// HTTPServer.
	{
	public:
		HTTPReactorServerSession(const StreamSocket& socket, const std::string& buf, HTTPServerParams::Ptr pParams):HTTPSession(), 
		_buf(buf),_realsocket(socket){
			_logger = &Logger::root();
			_pcur = const_cast<char*>(_buf.c_str());
			_idx = 0;
		}
			/// Creates the HTTPReactorServerSession.
	
		virtual ~HTTPReactorServerSession() {};
		/// Destroys the HTTPReactorServerSession.
		
		bool hasMoreRequests();
		/// Returns true if there are requests available.
		
		bool canKeepAlive() const;
		/// Returns true if the session can be kept alive.
		SocketAddress clientAddress() override{
			return _realsocket.peerAddress();
		}
			/// Returns the client's address.
	
		SocketAddress serverAddress() override{
			return _realsocket.address();
		}
			/// Returns the server's address.

		void detach() {
			_logger->information("detach socket " );
			_realsocket =detachSocket();
			_logger->information("detach socket " + _realsocket.peerAddress().toString());
		}
	private:
		int get() override{
			if(_idx<_buf.length()){

				return _buf[_idx++];
			}	else {

				return std::char_traits<char>::eof();
			}
		}
		
		int peek() override {
			if(_idx<_buf.length()){
	
				return _buf[_idx];
			}	else {
	
				return std::char_traits<char>::eof();
			}
		}



		int write(const char* buffer, std::streamsize length)override
		{
			try
			{
				_logger->trace("real socket " + _realsocket.peerAddress().toString()+ " write:" + std::string(buffer, length));
				return _realsocket.sendBytes(buffer, (int) length);
			}
			catch (Poco::Exception& exc)
			{
				setException(exc);
				throw;
			}
		}
	
	
	private:
		bool           _firstRequest;
		Poco::Timespan _keepAliveTimeout;
		int            _maxKeepAliveRequests;
		const std::string&  _buf;
		char* _pcur;
		char* _pend;
		int _idx;
		StreamSocket _realsocket;
		Logger* _logger;
	};

	inline bool HTTPReactorServerSession::canKeepAlive() const
	{
		return _maxKeepAliveRequests != 0;
	}
}
}

#endif // Net_HTTPReactorServerSession_INCLUDED