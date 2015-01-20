//
// HTTPClient.cpp
//
// $Id: //poco/1.4/Net/src/HTTPClient.cpp#15 $
//
// Library: Net
// Package: HTTPClient
// Module:  HTTPClient
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/HTTPClient.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPResponseEventArgs.h"
#include "Poco/Net/HTTPHeaderStream.h"
#include "Poco/Net/HTTPStream.h"
#include "Poco/StreamCopier.h"
#include "Poco/Thread.h"
#include <sstream>


namespace Poco {
namespace Net {


HTTPClient::HTTPClient(const StreamSocket& socket):
	_session(socket),
	_activity(this, &HTTPClient::runActivity)
{
}


HTTPClient::HTTPClient(const SocketAddress& address):
	_session(address),
	_activity(this, &HTTPClient::runActivity)
{
}


HTTPClient::HTTPClient(const std::string& host, Poco::UInt16 port):
	_session(host, port),
	_activity(this, &HTTPClient::runActivity)
{
}


HTTPClient::HTTPClient(const std::string& host, Poco::UInt16 port, const HTTPClientSession::ProxyConfig& proxyConfig) :
	_session(host, port, proxyConfig),
	_activity(this, &HTTPClient::runActivity)
{
}


HTTPClient::~HTTPClient()
{
	clearQueue();
}


void HTTPClient::runActivity()
{
    while (!_activity.isStopped())
    {
		RequestQueue   requestQueue;
		RequestBodyMap requestBodyMap;
		{
			Mutex::ScopedLock l(_mutex);
			if (_requestQueue.size())
			{
				requestQueue.assign(_requestQueue.begin(), _requestQueue.end());
				_requestQueue.clear();
			}

			if (_requestBodyMap.size())
			{
				requestBodyMap.insert(_requestBodyMap.begin(), _requestBodyMap.end());
				_requestBodyMap.clear();
			}
		}

		for (RequestQueue::iterator it = requestQueue.begin(),
			end = requestQueue.end(); it != end; ++it)
		{
			HTTPRequest& req = **it;

			try
			{
				std::ostream& os = _session.sendRequest(req);
				if (_requestBodyMap.find(*it) != _requestBodyMap.end())
				{
					os << _requestBodyMap[*it];
				}

				HTTPResponse response;
				std::ostringstream ostr;
				std::istream& rs = _session.receiveResponse(response);
				StreamCopier::copyStream(rs, ostr);
				HTTPResponseEventArgs args(req.getURI(), ostr.str());
				httpResponse.notify(this, args);
			}
			catch (Poco::Exception& ex)
			{
				HTTPResponseEventArgs args(req.getURI(), "", ex.displayText());
				httpError.notify(this, args);
			}
		}

		Thread::trySleep(100);
    }
}


void HTTPClient::wakeUp()
{
	if (_pThread) _pThread->wakeUp();
}


void HTTPClient::clearQueue()
{
	_requestBodyMap.clear();

	for (RequestQueue::iterator it = _requestQueue.begin(),
		end = _requestQueue.end(); it != end; ++it)
	{
		delete *it;
	}
}


void HTTPClient::reset()
{
	Mutex::ScopedLock l(_mutex);
	_session.reset();
	clearQueue();
}


void HTTPClient::sendRequest(const std::string& method,
	const std::string& uri,
	const std::string& body,
	const std::string& version)
{
	HTTPRequest* pReq = new HTTPRequest(method, uri, version);
	try
	{
		Mutex::ScopedLock l(_mutex);
		if (body.size())
		{
			pReq->setContentLength(static_cast<int>(body.length()));
			_requestBodyMap.insert(RequestBodyMap::value_type(pReq, body));
		}
		_requestQueue.push_back(pReq);
	}
	catch (...)
	{
		{
			Mutex::ScopedLock l(_mutex);
			RequestBodyMap::iterator it = _requestBodyMap.find(pReq);
			if (it != _requestBodyMap.end())
			{
				_requestBodyMap.erase(it);
			}
		}
		delete pReq;
		throw;
	}

	wakeUp();
}


void HTTPClient::setHost(const std::string& host)
{
	Mutex::ScopedLock l(_mutex);
	reset();
	_session.setHost(host);
	_session.reconnect();
}


void HTTPClient::setPort(Poco::UInt16 port)
{
	Mutex::ScopedLock l(_mutex);
	reset();
	_session.setPort(port);
	_session.reconnect();
}


void HTTPClient::setProxy(const std::string& host, Poco::UInt16 port)
{
	Mutex::ScopedLock l(_mutex);
	reset();
	_session.setProxy(host, port);
	_session.reconnect();
}


void HTTPClient::setProxyHost(const std::string& host)
{
	Mutex::ScopedLock l(_mutex);
	reset();
	_session.setProxyHost(host);
	_session.reconnect();
}


void HTTPClient::setProxyPort(Poco::UInt16 port)
{
	Mutex::ScopedLock l(_mutex);
	reset();
	_session.setProxyPort(port);
	_session.reconnect();
}


void HTTPClient::setProxyCredentials(const std::string& username, const std::string& password)
{
	Mutex::ScopedLock l(_mutex);
	reset();
	_session.setProxyCredentials(username, password);
	_session.reconnect();
}


void HTTPClient::setProxyUsername(const std::string& username)
{
	Mutex::ScopedLock l(_mutex);
	reset();
	_session.setProxyUsername(username);
	_session.reconnect();
}


void HTTPClient::setProxyPassword(const std::string& password)
{
	Mutex::ScopedLock l(_mutex);
	reset();
	_session.setProxyPassword(password);
	_session.reconnect();
}


void HTTPClient::setProxyConfig(const HTTPClientSession::ProxyConfig& config)
{
	Mutex::ScopedLock l(_mutex);
	reset();
	_session.setProxyConfig(config);
	_session.reconnect();
}


void HTTPClient::setKeepAliveTimeout(const Poco::Timespan& timeout)
{
	Mutex::ScopedLock l(_mutex);
	reset();
	_session.setKeepAliveTimeout(timeout);
	_session.reconnect();
}


} } // namespace Poco::Net
