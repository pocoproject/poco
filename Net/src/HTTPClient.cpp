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
#include "Poco/URI.h"
#include "Poco/Thread.h"
#include "Poco/ScopedLock.h"
#include "Poco/NumberFormatter.h"
#include <sstream>


namespace Poco {
namespace Net {


HTTPClient::HTTPClient(const URI& uri,
	InstantiatorPtr pInstantiator,
	bool redirect):
		_pInstantiator(pInstantiator),
		_pSecureInstantiator(0),
		_pSession(0),
		_redirect(redirect),
		_redirectHTTPS(false),
		_activity(this, &HTTPClient::runActivity),
		_pThread(0),
		_requestsInProcess(0)
{
	_factory.registerProtocol(uri.getScheme(), _pInstantiator);
	_pSession = _factory.createClientSession(uri);
	poco_check_ptr(_pSession);
	_activity.start();
}


HTTPClient::HTTPClient(const SocketAddress& address,
	const std::string& scheme,
	InstantiatorPtr pInstantiator,
	bool redirect):
		_pInstantiator(pInstantiator),
		_pSecureInstantiator(0),
		_pSession(0),
		_redirect(redirect),
		_redirectHTTPS(false),
		_activity(this, &HTTPClient::runActivity),
		_pThread(0),
		_requestsInProcess(0)
{
	_factory.registerProtocol(scheme, _pInstantiator);
	std::string uriStr = scheme;
	URI uri(uriStr.append("://").append(address.toString()));
	_pSession = _factory.createClientSession(uri);
	poco_check_ptr(_pSession);
	_activity.start();
}


HTTPClient::HTTPClient(const std::string& host,
	Poco::UInt16 port,
	const std::string& scheme,
	InstantiatorPtr pInstantiator,
	bool redirect):
		_pInstantiator(pInstantiator),
		_pSecureInstantiator(0),
		_pSession(0),
		_redirect(redirect),
		_redirectHTTPS(false),
		_activity(this, &HTTPClient::runActivity),
		_pThread(0),
		_requestsInProcess(0)
{
	_factory.registerProtocol(scheme, _pInstantiator);
	std::string uriStr = scheme;
	URI uri(uriStr.append("://").append(host).append(1, ':').append(NumberFormatter::format(port)));
	_pSession = _factory.createClientSession(uri);
	poco_check_ptr(_pSession);
	_activity.start();
}


HTTPClient::HTTPClient(const std::string& host,
	Poco::UInt16 port,
	const HTTPClientSession::ProxyConfig& proxyConfig,
	const std::string& scheme,
	InstantiatorPtr pInstantiator,
	bool redirect) :
		_pInstantiator(pInstantiator),
		_pSecureInstantiator(0),
		_pSession(0),
		_redirect(redirect),
		_redirectHTTPS(false),
		_activity(this, &HTTPClient::runActivity),
		_pThread(0),
		_requestsInProcess(0)
{
	_factory.registerProtocol(scheme, _pInstantiator);
	std::string uriStr = scheme;
	URI uri(uriStr.append("://").append(host).append(1, ':').append(NumberFormatter::format(port)));
	_factory.setProxy(proxyConfig.host, proxyConfig.port);
	if (!proxyConfig.username.empty())
		_factory.setProxyCredentials(proxyConfig.username, proxyConfig.password);
	_pSession = _factory.createClientSession(uri);
	poco_check_ptr(_pSession);
	_activity.start();
}


HTTPClient::~HTTPClient()
{
	_activity.stop();
	wakeUp();
	_activity.wait();
	clearQueue();
	if (_pInstantiator)
	{
		if (_factory.isRegistered("http"))
			_factory.unregisterProtocol("http");

		if (_factory.isRegistered("https"))
			_factory.unregisterProtocol("https");

		delete _pSession;
	}
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
				std::ostringstream ostr;
				std::istream* pRS = 0;
				do
				{
					HTTPResponse::HTTPStatus status = response.getStatus();
					if (status == HTTPResponse::HTTP_MOVED_PERMANENTLY ||
						status == HTTPResponse::HTTP_FOUND ||
						status == HTTPResponse::HTTP_TEMPORARY_REDIRECT ||
						status == HTTPResponse::HTTP_SEE_OTHER)
					{
						URI newURI(response.get("Location"));
						std::string scheme = newURI.getScheme();
						if (((scheme == "http") && _pSession->secure()) ||
							((scheme == "https") && !_pSession->secure()))
						{
							if (!_factory.isRegistered(scheme))
							{
								if (scheme == "http")
								{
									if (!_factory.isRegistered(scheme))
										_factory.registerProtocol(scheme, _pInstantiator);
								}
								else if (scheme == "https")
								{
									if (_pSecureInstantiator)
									{
										if (!_factory.isRegistered(scheme))
											_factory.registerProtocol(scheme, _pSecureInstantiator);
									}
									else
										throw NullPointerException("Unable to redirect to https, "
											"instantiator not available.");
								}
							}

							delete _pSession;
							_pSession = _factory.createClientSession(newURI);
							poco_check_ptr(_pSession);
						}
						redirect(req, newURI.toString(), status);
					}

					std::ostream& os = _pSession->sendRequest(req);
					if (requestBodyMap.find(*it) != requestBodyMap.end())
					{
						os << requestBodyMap[*it];
					}

					std::istream& rs = _pSession->receiveResponse(response);
					pRS = &rs;
				}
				while (_redirect && response.getStatus() == HTTPResponse::HTTP_FOUND);

				l.unlock();
				StreamCopier::copyStream(*pRS, ostr);
				HTTPEventArgs args(req.getURI(), response, ostr.str());
				if (response.getStatus() >= HTTPResponse::HTTP_BAD_REQUEST)
					httpError.notify(this, args);
				else
					httpResponse.notify(this, args);
			}
			catch (Poco::Exception& ex)
			{
				HTTPEventArgs args(req.getURI(), response, "", &ex);
				httpException.notify(this, args);
			}

			--_requestsInProcess;
		}

		Thread::trySleep(100);
    }
}


void HTTPClient::redirect(HTTPRequest& req,
	const std::string& newURI, HTTPResponse::HTTPStatus status)
{
	req.setURI(newURI);
	if (status == HTTPResponse::HTTP_SEE_OTHER &&
		req.getMethod() != HTTPRequest::HTTP_GET)
	{
		req.setMethod(HTTPRequest::HTTP_GET);
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
	_pSession->reset();
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
	_pSession->setHost(host);
	_pSession->reconnect();
}


void HTTPClient::setPort(Poco::UInt16 port)
{
	Mutex::ScopedLock l(_mutex);
	reset();
	_pSession->setPort(port);
	_pSession->reconnect();
}


void HTTPClient::setProxy(const std::string& host, Poco::UInt16 port)
{
	Mutex::ScopedLock l(_mutex);
	reset();
	_pSession->setProxy(host, port);
	_pSession->reconnect();
}


void HTTPClient::setProxyHost(const std::string& host)
{
	Mutex::ScopedLock l(_mutex);
	reset();
	_pSession->setProxyHost(host);
	_pSession->reconnect();
}


void HTTPClient::setProxyPort(Poco::UInt16 port)
{
	Mutex::ScopedLock l(_mutex);
	reset();
	_pSession->setProxyPort(port);
	_pSession->reconnect();
}


void HTTPClient::setProxyCredentials(const std::string& username, const std::string& password)
{
	_pSession->setProxyCredentials(username, password);
}


void HTTPClient::setProxyUsername(const std::string& username)
{
	Mutex::ScopedLock l(_mutex);
	_pSession->setProxyUsername(username);
}


void HTTPClient::setProxyPassword(const std::string& password)
{
	Mutex::ScopedLock l(_mutex);
	_pSession->setProxyPassword(password);
}


void HTTPClient::setProxyConfig(const HTTPClientSession::ProxyConfig& config)
{
	Mutex::ScopedLock l(_mutex);
	_pSession->setProxyConfig(config);
}


void HTTPClient::setKeepAlive(bool keepAlive)
{
	Mutex::ScopedLock l(_mutex);
	_pSession->setKeepAlive(keepAlive);
}


void HTTPClient::setTimeout(const Poco::Timespan& timeout)
{
	Mutex::ScopedLock l(_mutex);
	_pSession->setTimeout(timeout);
}


void HTTPClient::setKeepAliveTimeout(const Poco::Timespan& timeout)
{
	Mutex::ScopedLock l(_mutex);
	_pSession->setKeepAliveTimeout(timeout);
}


} } // namespace Poco::Net
