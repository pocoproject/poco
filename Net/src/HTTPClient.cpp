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
#include "Poco/Net/HTTPEventArgs.h"
#include "Poco/Net/HTTPHeaderStream.h"
#include "Poco/Net/HTTPStream.h"
#include "Poco/StreamCopier.h"
#include "Poco/Thread.h"
#include "Poco/ScopedLock.h"
#include <sstream>


namespace Poco {
namespace Net {


HTTPClient::HTTPClient(const StreamSocket& socket):
	_session(socket),
	_activity(this, &HTTPClient::runActivity),
	_pThread(0),
	_requestsInProcess(0)
{
	_activity.start();
}


HTTPClient::HTTPClient(const SocketAddress& address):
	_session(address),
	_activity(this, &HTTPClient::runActivity),
	_pThread(0),
	_requestsInProcess(0)
{
	_activity.start();
}


HTTPClient::HTTPClient(const std::string& host, Poco::UInt16 port):
	_session(host, port),
	_activity(this, &HTTPClient::runActivity),
	_pThread(0),
	_requestsInProcess(0)
{
	_activity.start();
}


HTTPClient::HTTPClient(const std::string& host, Poco::UInt16 port, const HTTPClientSession::ProxyConfig& proxyConfig) :
	_session(host, port, proxyConfig),
	_activity(this, &HTTPClient::runActivity),
	_pThread(0),
	_requestsInProcess(0)
{
	_activity.start();
}


HTTPClient::~HTTPClient()
{
	_activity.stop();
	wakeUp();
	_activity.wait();
	clearQueue();
}


void HTTPClient::runActivity()
{
    while (!_activity.isStopped())
    {
		RequestQueue   requestQueue;
		RequestBodyMap requestBodyMap;
		{
			Mutex::ScopedLock l(_exchangeMutex);
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

		_requestsInProcess = static_cast<int>(requestQueue.size());

		for (RequestQueue::iterator it = requestQueue.begin(),
			end = requestQueue.end(); it != end; ++it)
		{
			HTTPRequest& req = **it;
			HTTPResponse response;

			try
			{
				ScopedLockWithUnlock<Mutex> l(_mutex);
				std::ostream& os = _session.sendRequest(req);
				if (requestBodyMap.find(*it) != requestBodyMap.end())
				{
					os << requestBodyMap[*it];
				}

				std::ostringstream ostr;
				std::istream& rs = _session.receiveResponse(response);
				l.unlock();
				StreamCopier::copyStream(rs, ostr);
				HTTPEventArgs args(req.getURI(), response, ostr.str());
				if (response.getStatus() >= HTTPResponse::HTTP_BAD_REQUEST)
					httpError.notify(this, args);
				else
					httpResponse.notify(this, args);
			}
			catch (Poco::Exception& ex)
			{
				HTTPEventArgs args(req.getURI(), response, "", ex.displayText());
				httpException.notify(this, args);
			}

			--_requestsInProcess;
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
	Mutex::ScopedLock l(_exchangeMutex);
	_requestBodyMap.clear();
	_requestQueue.clear();
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
	SharedPtr<HTTPRequest> request = new HTTPRequest(method, uri, version);
	addRequest(request, body);
}


void HTTPClient::addRequest(SharedPtr<HTTPRequest> pReq,
	const std::string& body)
{
	try
	{
		Mutex::ScopedLock l(_exchangeMutex);
		if (body.size())
		{
			if (pReq->getContentLength() == HTTPMessage::UNKNOWN_CONTENT_LENGTH &&
				!pReq->getChunkedTransferEncoding() &&
				(body.find("\r\n\r\n") != std::string::npos ||
				body.find("\r\n0\r\n") != std::string::npos))
			{
				pReq->setContentLength(static_cast<int>(body.length()));
			}
			_requestBodyMap.insert(RequestBodyMap::value_type(pReq, body));
		}
		_requestQueue.push_back(pReq);
	}
	catch (...)
	{
		{
			Mutex::ScopedLock l(_exchangeMutex);
			RequestBodyMap::iterator it = _requestBodyMap.find(pReq);
			if (it != _requestBodyMap.end())
			{
				_requestBodyMap.erase(it);
			}
		}
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
	_session.setProxyCredentials(username, password);
}


void HTTPClient::setProxyUsername(const std::string& username)
{
	Mutex::ScopedLock l(_mutex);
	_session.setProxyUsername(username);
}


void HTTPClient::setProxyPassword(const std::string& password)
{
	Mutex::ScopedLock l(_mutex);
	_session.setProxyPassword(password);
}


void HTTPClient::setProxyConfig(const HTTPClientSession::ProxyConfig& config)
{
	Mutex::ScopedLock l(_mutex);
	_session.setProxyConfig(config);
}


void HTTPClient::setKeepAlive(bool keepAlive)
{
	Mutex::ScopedLock l(_mutex);
	_session.setKeepAlive(keepAlive);
}


void HTTPClient::setTimeout(const Poco::Timespan& timeout)
{
	Mutex::ScopedLock l(_mutex);
	_session.setTimeout(timeout);
}


void HTTPClient::setKeepAliveTimeout(const Poco::Timespan& timeout)
{
	Mutex::ScopedLock l(_mutex);
	_session.setKeepAliveTimeout(timeout);
}


} } // namespace Poco::Net
