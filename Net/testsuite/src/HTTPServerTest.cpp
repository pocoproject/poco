//
// HTTPServerTest.cpp
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "HTTPServerTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/AbstractHTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerRequestImpl.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerSession.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/NumberFormatter.h"
#include "Poco/StreamCopier.h"
#include "Poco/Path.h"
#include "Poco/FileStream.h"
#include "Poco/File.h"
#include <optional>
#include <sstream>
#include <utility>


using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerParams;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::AbstractHTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPServerResponse;
using Poco::Net::HTTPMessage;
using Poco::Net::ServerSocket;
using Poco::StreamCopier;
using Poco::Path;
using Poco::File;
using Poco::FileOutputStream;


namespace
{
	static constexpr int sendFileSize = 64000;

	class EchoBodyRequestHandler: public HTTPRequestHandler
	{
	public:
		void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
		{
			if (request.getChunkedTransferEncoding())
				response.setChunkedTransferEncoding(true);
			else if (request.getContentLength() != HTTPMessage::UNKNOWN_CONTENT_LENGTH)
				response.setContentLength(request.getContentLength());

			response.setContentType(request.getContentType());

			std::istream& istr = request.stream();
			std::ostream& ostr = response.send();
			StreamCopier::copyStream(istr, ostr);
		}
	};

	class EchoHeaderRequestHandler: public HTTPRequestHandler
	{
	public:
		void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
		{
			std::ostringstream osstr;
			request.write(osstr);
			int n = (int) osstr.str().length();
			response.setContentLength(n);
			std::ostream& ostr = response.send();
			if (request.getMethod() != HTTPRequest::HTTP_HEAD)
				request.write(ostr);
		}
	};

	class RedirectRequestHandler: public AbstractHTTPRequestHandler
	{
	public:
		void run()
		{
			response().redirect("http://www.appinf.com/");
		}
	};

	class LineBreakRedirectRequestHandler: public AbstractHTTPRequestHandler
	{
	public:
		void run()
		{
			response().redirect("/target\r\nSet-Cookie: injected=1");
		}
	};

	class LineBreakSendRequestHandler: public AbstractHTTPRequestHandler
	{
	public:
		void run()
		{
			response().set("X-Field", "a\r\nSet-Cookie: injected=1");
			response().send();
		}
	};

	class EchoFieldRequestHandler: public HTTPRequestHandler
	{
	public:
		void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
		{
			response.set("X-Echo", request.get("X-Echo", ""));
			response.setContentLength(0);
			response.send();
		}
	};

	class AuthRequestHandler: public HTTPRequestHandler
	{
	public:
		void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
		{
			response.requireAuthentication("/auth");
			response.send();
		}
	};

	class BufferRequestHandler: public HTTPRequestHandler
	{
	public:
		void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
		{
			std::string data("xxxxxxxxxx");
			response.sendBuffer(data.data(), data.length());
		}
	};
	
	class FileRequestHandler: public HTTPRequestHandler
	{
	public:
		void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
		{
			std::string payload(sendFileSize, 'x');
			Poco::Path testFilePath = Poco::Path::temp().append("test.http.server.sendfile.txt");
			const std::string fileName = testFilePath.toString();
			{
				File f(fileName);
				if (f.exists())
				{
					f.remove();
				}
			}
			FileOutputStream fout(fileName);
			fout << payload;
			fout.close();
			response.sendFile(fileName, "text/plain");
		}
	};

	class EmptyFileRequestHandler: public HTTPRequestHandler
	{
	public:
		void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
		{
			Poco::Path testFilePath = Poco::Path::temp().append("test.http.server.sendfile.empty.txt");
			const std::string fileName = testFilePath.toString();
			FileOutputStream fout(fileName);
			fout.close();
			response.sendFile(fileName, "text/plain");
		}
	};
	
	class TrailerRequestHandler: public HTTPRequestHandler
	{
	public:
		void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
		{
			std::string data("xxxxxxxxxx");

			response.setChunkedTransferEncoding(true);
			Poco::Net::MessageHeader& trailer = static_cast<Poco::Net::HTTPServerRequestImpl&>(request).session().responseTrailer();
			trailer.set("Trailer-1", "Value 1");
			trailer.set("Trailer-2", "Value 2");
			response.send() << data;
		}
	};

	class NonBlockingRequestHandler: public BufferRequestHandler
	{
	public:
		void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) override
		{
			static_cast<Poco::Net::HTTPServerRequestImpl&>(request).socket().setBlocking(false);
			BufferRequestHandler::handleRequest(request, response);
		}
	};

	class ToggleBlockingRequestHandler: public BufferRequestHandler
	{
	public:
		void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) override
		{
			Poco::Net::StreamSocket& socket = static_cast<Poco::Net::HTTPServerRequestImpl&>(request).socket();
			socket.setBlocking(false);
			std::string body;
			StreamCopier::copyToString(request.stream(), body);
			socket.setBlocking(true);
			BufferRequestHandler::handleRequest(request, response);
		}
	};

	class BodyStatusRequestHandler: public HTTPRequestHandler
	{
	public:
		void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
		{
			std::string body;
			StreamCopier::copyToString(request.stream(), body);
			const bool incomplete = static_cast<Poco::Net::HTTPServerRequestImpl&>(request).session().networkException() != nullptr;
			const std::string status(std::to_string(body.size()) + (incomplete ? " incomplete" : " complete"));
			response.sendBuffer(status.data(), status.length());
		}
	};

	class RequestHandlerFactory: public HTTPRequestHandlerFactory
	{
	public:
		HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request)
		{
			if (request.getURI() == "/echoBody")
				return new EchoBodyRequestHandler;
			else if (request.getURI() == "/echoHeader")
				return new EchoHeaderRequestHandler;
			else if (request.getURI() == "/redirect")
				return new RedirectRequestHandler;
			else if (request.getURI() == "/lineBreakRedirect")
				return new LineBreakRedirectRequestHandler;
			else if (request.getURI() == "/lineBreakSend")
				return new LineBreakSendRequestHandler;
			else if (request.getURI() == "/echoField")
				return new EchoFieldRequestHandler;
			else if (request.getURI() == "/auth")
				return new AuthRequestHandler;
			else if (request.getURI() == "/buffer")
				return new BufferRequestHandler;
			else if (request.getURI() == "/trailer")
				return new TrailerRequestHandler;
			else if (request.getURI() == "/file")
				return new FileRequestHandler;
			else if (request.getURI() == "/emptyFile")
				return new EmptyFileRequestHandler;
			else if (request.getURI() == "/nonBlocking")
				return new NonBlockingRequestHandler;
			else if (request.getURI() == "/toggleBlocking")
				return new ToggleBlockingRequestHandler;
			else if (request.getURI() == "/bodyStatus")
				return new BodyStatusRequestHandler;
			else
				return nullptr;
		}
	};
}


HTTPServerTest::HTTPServerTest(const std::string& name): CppUnit::TestCase(name)
{
}


HTTPServerTest::~HTTPServerTest()
{
}


void HTTPServerTest::testIdentityRequest()
{
	ServerSocket svs(0);
	HTTPServerParams* pParams = new HTTPServerParams;
	pParams->setKeepAlive(false);
	HTTPServer srv(new RequestHandlerFactory, svs, pParams);
	srv.start();

	HTTPClientSession cs("127.0.0.1", svs.address().port());
	std::string body(5000, 'x');
	HTTPRequest request("POST", "/echoBody");
	request.setContentLength((int) body.length());
	request.setContentType("text/plain");
	cs.sendRequest(request) << body;
	HTTPResponse response;
	std::string rbody;
	cs.receiveResponse(response) >> rbody;
	assertTrue (response.getContentLength() == body.size());
	assertTrue (response.getContentType() == "text/plain");
	assertTrue (rbody == body);
}


void HTTPServerTest::testPutIdentityRequest()
{
	ServerSocket svs(0);
	HTTPServerParams* pParams = new HTTPServerParams;
	pParams->setKeepAlive(false);
	HTTPServer srv(new RequestHandlerFactory, svs, pParams);
	srv.start();

	HTTPClientSession cs("127.0.0.1", svs.address().port());
	std::string body(5000, 'x');
	HTTPRequest request("PUT", "/echoBody");
	request.setContentLength((int) body.length());
	request.setContentType("text/plain");
	cs.sendRequest(request) << body;
	HTTPResponse response;
	std::string rbody;
	cs.receiveResponse(response) >> rbody;
	assertTrue (response.getContentLength() == body.size());
	assertTrue (response.getContentType() == "text/plain");
	assertTrue (rbody == body);
}


void HTTPServerTest::testChunkedRequest()
{
	ServerSocket svs(0);
	HTTPServerParams* pParams = new HTTPServerParams;
	pParams->setKeepAlive(false);
	HTTPServer srv(new RequestHandlerFactory, svs, pParams);
	srv.start();

	HTTPClientSession cs("127.0.0.1", svs.address().port());
	std::string body(5000, 'x');
	HTTPRequest request("POST", "/echoBody");
	request.setContentType("text/plain");
	request.setChunkedTransferEncoding(true);
	cs.sendRequest(request) << body;
	HTTPResponse response;
	std::string rbody;
	cs.receiveResponse(response) >> rbody;
	assertTrue (response.getContentLength() == HTTPMessage::UNKNOWN_CONTENT_LENGTH);
	assertTrue (response.getContentType() == "text/plain");
	assertTrue (response.getChunkedTransferEncoding());
	assertTrue (rbody == body);
}


void HTTPServerTest::testIdentityRequestKeepAlive()
{
	HTTPServer srv(new RequestHandlerFactory, 8008);
	srv.start();

	HTTPClientSession cs("127.0.0.1", srv.socket().address().port());
	cs.setKeepAlive(true);
	std::string body(5000, 'x');
	HTTPRequest request("POST", "/echoBody", HTTPMessage::HTTP_1_1);
	request.setContentLength((int) body.length());
	request.setContentType("text/plain");
	cs.sendRequest(request) << body;
	HTTPResponse response;
	std::string rbody;
	cs.receiveResponse(response) >> rbody;
	assertTrue (response.getContentLength() == body.size());
	assertTrue (response.getContentType() == "text/plain");
	assertTrue (response.getKeepAlive());
	assertTrue (rbody == body);

	body.assign(1000, 'y');
	request.setContentLength((int) body.length());
	request.setKeepAlive(false);
	cs.sendRequest(request) << body;
	cs.receiveResponse(response) >> rbody;
	assertTrue (response.getContentLength() == body.size());
	assertTrue (response.getContentType() == "text/plain");
	assertTrue (!response.getKeepAlive());
	assertTrue (rbody == body);}


void HTTPServerTest::testChunkedRequestKeepAlive()
{
	HTTPServer srv(new RequestHandlerFactory, 8009);
	srv.start();

	HTTPClientSession cs("127.0.0.1", srv.socket().address().port());
	cs.setKeepAlive(true);
	std::string body(5000, 'x');
	HTTPRequest request("POST", "/echoBody", HTTPMessage::HTTP_1_1);
	request.setContentType("text/plain");
	request.setChunkedTransferEncoding(true);
	cs.sendRequest(request) << body;
	HTTPResponse response;
	std::string rbody;
	cs.receiveResponse(response) >> rbody;
	assertTrue (response.getContentLength() == HTTPMessage::UNKNOWN_CONTENT_LENGTH);
	assertTrue (response.getContentType() == "text/plain");
	assertTrue (response.getChunkedTransferEncoding());
	assertTrue (rbody == body);

	body.assign(1000, 'y');
	request.setKeepAlive(false);
	cs.sendRequest(request) << body;
	cs.receiveResponse(response) >> rbody;
	assertTrue (response.getContentLength() == HTTPMessage::UNKNOWN_CONTENT_LENGTH);
	assertTrue (response.getContentType() == "text/plain");
	assertTrue (response.getChunkedTransferEncoding());
	assertTrue (!response.getKeepAlive());
	assertTrue (rbody == body);
}


void HTTPServerTest::testMaxKeepAlive()
{
	ServerSocket svs(0);
	HTTPServerParams* pParams = new HTTPServerParams;
	pParams->setKeepAlive(true);
	pParams->setMaxKeepAliveRequests(4);
	HTTPServer srv(new RequestHandlerFactory, svs, pParams);
	srv.start();

	HTTPClientSession cs("127.0.0.1", svs.address().port());
	cs.setKeepAlive(true);
	HTTPRequest request("POST", "/echoBody", HTTPMessage::HTTP_1_1);
	request.setContentType("text/plain");
	request.setChunkedTransferEncoding(true);
	std::string body(5000, 'x');
	for (int i = 0; i < 3; ++i)
	{
		cs.sendRequest(request) << body;
		HTTPResponse response;
		std::string rbody;
		cs.receiveResponse(response) >> rbody;
		assertTrue (response.getContentLength() == HTTPMessage::UNKNOWN_CONTENT_LENGTH);
		assertTrue (response.getContentType() == "text/plain");
		assertTrue (response.getChunkedTransferEncoding());
		assertTrue (response.getKeepAlive());
		assertTrue (rbody == body);
	}

	{
		cs.sendRequest(request) << body;
		HTTPResponse response;
		std::string rbody;
		cs.receiveResponse(response) >> rbody;
		assertTrue (response.getContentLength() == HTTPMessage::UNKNOWN_CONTENT_LENGTH);
		assertTrue (response.getContentType() == "text/plain");
		assertTrue (response.getChunkedTransferEncoding());
		assertTrue (!response.getKeepAlive());
		assertTrue (rbody == body);
	}

	{
		cs.setKeepAlive(false);
		cs.sendRequest(request) << body;
		HTTPResponse response;
		std::string rbody;
		cs.receiveResponse(response) >> rbody;
		assertTrue (response.getContentLength() == HTTPMessage::UNKNOWN_CONTENT_LENGTH);
		assertTrue (response.getContentType() == "text/plain");
		assertTrue (response.getChunkedTransferEncoding());
		assertTrue (!response.getKeepAlive());
		assertTrue (rbody == body);
	}
}


void HTTPServerTest::testKeepAliveTimeout()
{
	ServerSocket svs(0);
	HTTPServerParams* pParams = new HTTPServerParams;
	pParams->setKeepAlive(true);
	pParams->setMaxKeepAliveRequests(4);
	pParams->setKeepAliveTimeout(Poco::Timespan(3, 0));
	HTTPServer srv(new RequestHandlerFactory, svs, pParams);
	srv.start();

	HTTPClientSession cs("127.0.0.1", svs.address().port());
	cs.setKeepAlive(true);
	cs.setKeepAliveTimeout(Poco::Timespan(2, 0));
	HTTPRequest request("POST", "/echoBody", HTTPMessage::HTTP_1_1);
	request.setContentType("text/plain");
	request.setChunkedTransferEncoding(true);
	std::string body(5000, 'x');
	for (int i = 0; i < 3; ++i)
	{
		cs.sendRequest(request) << body;
		HTTPResponse response;
		std::string rbody;
		cs.receiveResponse(response) >> rbody;
		assertTrue (response.getContentLength() == HTTPMessage::UNKNOWN_CONTENT_LENGTH);
		assertTrue (response.getContentType() == "text/plain");
		assertTrue (response.getChunkedTransferEncoding());
		assertTrue (response.getKeepAlive());
		assertTrue (rbody == body);
	}

	Poco::Thread::sleep(4000);

	{
		cs.sendRequest(request) << body;
		HTTPResponse response;
		std::string rbody;
		cs.receiveResponse(response) >> rbody;
		assertTrue (response.getContentLength() == HTTPMessage::UNKNOWN_CONTENT_LENGTH);
		assertTrue (response.getContentType() == "text/plain");
		assertTrue (response.getChunkedTransferEncoding());
		assertTrue (response.getKeepAlive());
		assertTrue (rbody == body);
	}
}


void HTTPServerTest::test100Continue()
{
	ServerSocket svs(0);
	HTTPServerParams* pParams = new HTTPServerParams;
	pParams->setKeepAlive(false);
	HTTPServer srv(new RequestHandlerFactory, svs, pParams);
	srv.start();

	HTTPClientSession cs("127.0.0.1", svs.address().port());
	std::string body(5000, 'x');
	HTTPRequest request("POST", "/echoBody");
	request.setContentLength((int) body.length());
	request.setContentType("text/plain");
	request.set("Expect", "100-Continue");
	cs.sendRequest(request) << body;
	HTTPResponse response;
	std::string rbody;
	cs.receiveResponse(response) >> rbody;
	assertTrue (response.getContentLength() == body.size());
	assertTrue (response.getContentType() == "text/plain");
	assertTrue (rbody == body);
}


void HTTPServerTest::testRedirect()
{
	ServerSocket svs(0);
	HTTPServerParams* pParams = new HTTPServerParams;
	pParams->setKeepAlive(false);
	HTTPServer srv(new RequestHandlerFactory, svs, pParams);
	srv.start();

	HTTPClientSession cs("127.0.0.1", svs.address().port());
	HTTPRequest request("GET", "/redirect");
	cs.sendRequest(request);
	HTTPResponse response;
	std::string rbody;
	cs.receiveResponse(response) >> rbody;
	assertTrue (response.getStatus() == HTTPResponse::HTTP_FOUND);
	assertTrue (response.get("Location") == "http://www.appinf.com/");
	assertTrue (rbody.empty());
}


void HTTPServerTest::testAuth()
{
	ServerSocket svs(0);
	HTTPServerParams* pParams = new HTTPServerParams;
	pParams->setKeepAlive(false);
	HTTPServer srv(new RequestHandlerFactory, svs, pParams);
	srv.start();

	HTTPClientSession cs("127.0.0.1", svs.address().port());
	HTTPRequest request("GET", "/auth");
	cs.sendRequest(request);
	HTTPResponse response;
	std::string rbody;
	cs.receiveResponse(response) >> rbody;
	assertTrue (response.getStatus() == HTTPResponse::HTTP_UNAUTHORIZED);
	assertTrue (response.get("WWW-Authenticate") == "Basic realm=\"/auth\"");
	assertTrue (rbody.empty());
}


void HTTPServerTest::testNotImpl()
{
	ServerSocket svs(0);
	HTTPServerParams* pParams = new HTTPServerParams;
	pParams->setKeepAlive(false);
	HTTPServer srv(new RequestHandlerFactory, svs, pParams);
	srv.start();

	HTTPClientSession cs("127.0.0.1", svs.address().port());
	HTTPRequest request("GET", "/notImpl");
	cs.sendRequest(request);
	HTTPResponse response;
	std::string rbody;
	cs.receiveResponse(response) >> rbody;
	assertTrue (response.getStatus() == HTTPResponse::HTTP_NOT_IMPLEMENTED);
	assertTrue (rbody.empty());
}


void HTTPServerTest::testBuffer()
{
	ServerSocket svs(0);
	HTTPServerParams* pParams = new HTTPServerParams;
	pParams->setKeepAlive(false);
	HTTPServer srv(new RequestHandlerFactory, svs, pParams);
	srv.start();

	HTTPClientSession cs("127.0.0.1", svs.address().port());
	HTTPRequest request("GET", "/buffer");
	cs.sendRequest(request);
	HTTPResponse response;
	std::string rbody;
	cs.receiveResponse(response) >> rbody;
	assertTrue (response.getStatus() == HTTPResponse::HTTP_OK);
	assertTrue (rbody == "xxxxxxxxxx");
}

void HTTPServerTest::testFile()
{
	std::string payload(sendFileSize, 'x');
	
	ServerSocket svs(0);
	HTTPServerParams* pParams = new HTTPServerParams;
	pParams->setKeepAlive(false);
	HTTPServer srv(new RequestHandlerFactory, svs, pParams);
	srv.start();
	
	HTTPClientSession cs("127.0.0.1", svs.address().port());
	HTTPRequest request("GET", "/file");
	cs.sendRequest(request);
	HTTPResponse response;
	std::string rbody;
	cs.receiveResponse(response) >> rbody;
	assertTrue (response.getStatus() == HTTPResponse::HTTP_OK);
	assertTrue (rbody == payload);
}


void HTTPServerTest::testFileKeepAlive()
{
	std::string payload(sendFileSize, 'x');

	ServerSocket svs(0);
	HTTPServerParams* pParams = new HTTPServerParams;
	pParams->setKeepAlive(true);
	HTTPServer srv(new RequestHandlerFactory, svs, pParams);
	srv.start();

	HTTPClientSession cs("127.0.0.1", svs.address().port());
	cs.setKeepAlive(true);
	for (int i = 0; i < 2; ++i)
	{
		HTTPRequest request("GET", "/file", HTTPMessage::HTTP_1_1);
		cs.sendRequest(request);
		HTTPResponse response;
		std::string rbody;
		cs.receiveResponse(response) >> rbody;
		assertTrue (response.getStatus() == HTTPResponse::HTTP_OK);
		assertTrue (response.getContentLength() == sendFileSize);
		assertTrue (response.getKeepAlive());
		assertTrue (rbody == payload);
	}
}


void HTTPServerTest::testEmptyFileKeepAlive()
{
	std::string payload(sendFileSize, 'x');

	ServerSocket svs(0);
	HTTPServerParams* pParams = new HTTPServerParams;
	pParams->setKeepAlive(true);
	HTTPServer srv(new RequestHandlerFactory, svs, pParams);
	srv.start();

	HTTPClientSession cs("127.0.0.1", svs.address().port());
	cs.setKeepAlive(true);
	HTTPRequest request("GET", "/emptyFile", HTTPMessage::HTTP_1_1);
	cs.sendRequest(request);
	HTTPResponse response;
	std::string rbody;
	cs.receiveResponse(response) >> rbody;
	assertTrue (response.getStatus() == HTTPResponse::HTTP_OK);
	assertTrue (response.getContentLength() == 0);
	assertTrue (response.getKeepAlive());
	assertTrue (rbody.empty());
	const Poco::Net::SocketAddress clientAddress = cs.socket().address();

	// the connection still carries the next request
	HTTPRequest fileRequest("GET", "/file", HTTPMessage::HTTP_1_1);
	cs.sendRequest(fileRequest);
	HTTPResponse fileResponse;
	std::string fileBody;
	cs.receiveResponse(fileResponse) >> fileBody;
	assertTrue (fileResponse.getStatus() == HTTPResponse::HTTP_OK);
	assertTrue (fileBody == payload);
	assertTrue (cs.socket().address() == clientAddress);
}


void HTTPServerTest::testChunkedTrailer()
{
	ServerSocket svs(0);
	HTTPServerParams* pParams = new HTTPServerParams;
	pParams->setKeepAlive(false);
	HTTPServer srv(new RequestHandlerFactory, svs, pParams);
	srv.start();

	HTTPClientSession cs("127.0.0.1", svs.address().port());
	HTTPRequest request("GET", "/trailer");
	cs.sendRequest(request);
	HTTPResponse response;
	std::string rbody;
	cs.receiveResponse(response) >> rbody;
	assertTrue (response.getStatus() == HTTPResponse::HTTP_OK);
	assertTrue (rbody == "xxxxxxxxxx");
	assertTrue (!cs.responseTrailer().empty());
	assertTrue (cs.responseTrailer().get("Trailer-1") == "Value 1");
	assertTrue (cs.responseTrailer().get("Trailer-2") == "Value 2");
}


void HTTPServerTest::setUp()
{
}


void HTTPServerTest::tearDown()
{
}


namespace
{
	std::string sendRaw(Poco::UInt16 port, const std::string& request)
	{
		Poco::Net::StreamSocket ss(Poco::Net::SocketAddress("127.0.0.1", port));
		ss.sendBytes(request.data(), static_cast<int>(request.size()));
		std::string response;
		char buffer[512];
		try
		{
			int n = ss.receiveBytes(buffer, sizeof(buffer));
			while (n > 0)
			{
				response.append(buffer, n);
				if (response.find("\r\n\r\n") != std::string::npos) break;
				n = ss.receiveBytes(buffer, sizeof(buffer));
			}
		}
		catch (Poco::Exception&)
		{
		}
		return response;
	}

	std::optional<std::string> receiveUntilClosed(Poco::Net::StreamSocket& ss)
		/// Returns everything received until the server closes the connection, or
		/// nothing if the connection is still open when the receive timeout expires.
	{
		std::string response;
		char buffer[1024];
		try
		{
			int n = ss.receiveBytes(buffer, sizeof(buffer));
			while (n > 0)
			{
				response.append(buffer, n);
				n = ss.receiveBytes(buffer, sizeof(buffer));
			}
		}
		catch (Poco::Net::NetException&)
		{
			// A server that closes the connection with unread data resets or aborts it.
		}
		catch (Poco::TimeoutException&)
		{
			return std::nullopt;
		}
		return response;
	}

	std::optional<std::string> receiveUntil(Poco::Net::StreamSocket& ss, const std::string& marker)
		/// Returns everything received once it contains the marker, or nothing if the
		/// receive timeout expires or the server closes or resets the connection before that.
	{
		std::string response;
		char buffer[1024];
		try
		{
			int n = ss.receiveBytes(buffer, sizeof(buffer));
			while (n > 0)
			{
				response.append(buffer, n);
				if (response.find(marker) != std::string::npos) return response;
				n = ss.receiveBytes(buffer, sizeof(buffer));
			}
		}
		catch (Poco::Net::NetException&)
		{
		}
		catch (Poco::TimeoutException&)
		{
		}
		return std::nullopt;
	}

	std::optional<std::string> exchangeUntilClosed(HTTPServerParams* pParams, const std::string& request, bool shutdownSend)
		/// Starts a server with the given parameters, sends the request on a new connection,
		/// shuts down its sending side if shutdownSend is true and returns what receiveUntilClosed() returns.
		/// A send that fails because the server closed the connection is not an error;
		/// a send that times out returns nothing.
	{
		ServerSocket svs(Poco::Net::SocketAddress("127.0.0.1", 0));
		HTTPServer srv(new RequestHandlerFactory, svs, pParams);
		srv.start();

		Poco::Net::StreamSocket ss(Poco::Net::SocketAddress("127.0.0.1", svs.address().port()));
		ss.setReceiveTimeout(Poco::Timespan(10, 0));
		ss.setSendTimeout(Poco::Timespan(10, 0));
		try
		{
			ss.sendBytes(request.data(), static_cast<int>(request.size()));
			if (shutdownSend) ss.shutdownSend();
		}
		catch (Poco::TimeoutException&)
		{
			return std::nullopt;
		}
		catch (Poco::IOException&)
		{
		}
		return receiveUntilClosed(ss);
	}
}


void HTTPServerTest::testFramingConflicts()
{
	ServerSocket svs(0);
	HTTPServerParams* pParams = new HTTPServerParams;
	pParams->setKeepAlive(false);
	HTTPServer srv(new RequestHandlerFactory, svs, pParams);
	srv.start();
	const Poco::UInt16 port = svs.address().port();

	const std::string head("POST /echoBody HTTP/1.1\r\nHost: test\r\n");

	// Both framing headers present: the length is ambiguous.
	std::string response = sendRaw(port, head +
		"Transfer-Encoding: chunked\r\nContent-Length: 2\r\n\r\n");
	assertTrue (response.compare(0, 12, "HTTP/1.1 400") == 0);

	response = sendRaw(port, head + "Content-Length: 2\r\nContent-Length: 3\r\n\r\nhi");
	assertTrue (response.compare(0, 12, "HTTP/1.1 400") == 0);

	// A thousands separator must not turn 5 into 55.
	response = sendRaw(port, head + "Content-Length: 5,5\r\n\r\nhello");
	assertTrue (response.compare(0, 12, "HTTP/1.1 400") == 0);

	response = sendRaw(port, head + "Content-Length: 99999999999999999999\r\n\r\nhello");
	assertTrue (response.compare(0, 12, "HTTP/1.1 400") == 0);

	// A transfer coding other than chunked does not frame the body.
	response = sendRaw(port, head + "Transfer-Encoding: gzip, chunked\r\n\r\n");
	assertTrue (response.compare(0, 12, "HTTP/1.1 400") == 0);

	// The unambiguous case must still be served.
	response = sendRaw(port, head + "Content-Length: 2\r\n\r\nhi");
	assertTrue (response.compare(0, 12, "HTTP/1.1 200") == 0);

	srv.stop();
}


void HTTPServerTest::testChunkedFramingError()
{
	const std::optional<std::string> response = exchangeUntilClosed(new HTTPServerParams,
		"POST /echoBody HTTP/1.1\r\nHost: test\r\nTransfer-Encoding: chunked\r\n\r\n"
		"0\n\n"
		"GET /echoHeader HTTP/1.1\r\nHost: test\r\n\r\n", true);
	assertTrue (response.has_value());
	assertTrue (response->find("GET /echoHeader") == std::string::npos);
}


void HTTPServerTest::testUnreadRequestBody()
{
	const std::string body("GET /echoHeader HTTP/1.1\r\nHost: smuggled\r\n\r\n");
	const std::optional<std::string> response = exchangeUntilClosed(new HTTPServerParams,
		"POST /buffer HTTP/1.1\r\nHost: test\r\nContent-Length: " + std::to_string(body.size()) + "\r\n\r\n" + body +
		"GET /echoHeader HTTP/1.1\r\nHost: test\r\nConnection: close\r\n\r\n", false);
	assertTrue (response.has_value());
	assertTrue (response->find("smuggled") == std::string::npos);
	assertTrue (response->find("Host: test") != std::string::npos);
}


void HTTPServerTest::testUnreadChunkedRequestBody()
{
	const std::string body("GET /echoHeader HTTP/1.1\r\nHost: smuggled\r\n\r\n");
	const std::optional<std::string> response = exchangeUntilClosed(new HTTPServerParams,
		"POST /buffer HTTP/1.1\r\nHost: test\r\nTransfer-Encoding: chunked\r\n\r\n" +
		Poco::NumberFormatter::formatHex(body.size()) + "\r\n" + body + "\r\n0\r\n\r\n"
		"GET /echoHeader HTTP/1.1\r\nHost: test\r\nConnection: close\r\n\r\n", false);
	assertTrue (response.has_value());
	assertTrue (response->find("smuggled") == std::string::npos);
	assertTrue (response->find("Host: test") != std::string::npos);
}


// Checks that the server drains an unread request body only up to a bound.
void HTTPServerTest::testLargeUnreadRequestBody()
{
	const std::string body(10 * 1024 * 1024, 'x');
	const std::optional<std::string> response = exchangeUntilClosed(new HTTPServerParams,
		"POST /buffer HTTP/1.1\r\nHost: test\r\nContent-Length: " + std::to_string(body.size()) + "\r\n\r\n" + body, false);
	assertTrue (response.has_value());
}


void HTTPServerTest::testResponseBeforeUnreadRequestBody()
{
	ServerSocket svs(Poco::Net::SocketAddress("127.0.0.1", 0));
	HTTPServer srv(new RequestHandlerFactory, svs, new HTTPServerParams);
	srv.start();

	Poco::Net::StreamSocket ss(Poco::Net::SocketAddress("127.0.0.1", svs.address().port()));
	ss.setReceiveTimeout(Poco::Timespan(10, 0));
	const std::string head("POST /buffer HTTP/1.1\r\nHost: test\r\nContent-Length: 10\r\n\r\nxxxxx");
	ss.sendBytes(head.data(), static_cast<int>(head.size()));
	if (!receiveUntil(ss, "\r\n\r\nxxxxxxxxxx").has_value()) failmsg("server held the response back until the request body was complete");

	const std::string rest("xxxxxGET /echoHeader HTTP/1.1\r\nHost: test\r\nConnection: close\r\n\r\n");
	ss.sendBytes(rest.data(), static_cast<int>(rest.size()));
	const std::optional<std::string> next = receiveUntilClosed(ss);
	assertTrue (next.has_value());
	assertTrue (next->find("GET /echoHeader") != std::string::npos);

	srv.stop();
}


void HTTPServerTest::testNonBlockingHandlerClosesConnection()
{
	const std::string framedBodies[] = {
		"Transfer-Encoding: chunked\r\n\r\n40\r\n0123",
		"Content-Length: 64\r\n\r\n0123"
	};
	for (const std::string& body: framedBodies)
	{
		HTTPServerParams* pParams = new HTTPServerParams;
		pParams->setTimeout(Poco::Timespan(5, 0));
		const std::optional<std::string> response = exchangeUntilClosed(pParams,
			"POST /nonBlocking HTTP/1.1\r\nHost: test\r\n" + body, false);
		assertTrue (response.has_value());
		assertTrue (response->find("HTTP/1.1 200 OK") != std::string::npos);
	}
}


void HTTPServerTest::testHandlerNonBlockingReadThenBlocking()
{
	std::string data("GET /echoHeader HTTP/1.1\r\nHost: smuggled\r\n\r\n");
	data.resize(60, 'x');
	const std::pair<std::string, std::string> framedBodies[] = {
		{"Content-Length: 64\r\n\r\n0123", data},
		{"Transfer-Encoding: chunked\r\n\r\n40\r\n0123", data + "\r\n0\r\n\r\n"}
	};
	for (const auto& [head, rest]: framedBodies)
	{
		HTTPServerParams* pParams = new HTTPServerParams;
		pParams->setTimeout(Poco::Timespan(5, 0));
		ServerSocket svs(Poco::Net::SocketAddress("127.0.0.1", 0));
		HTTPServer srv(new RequestHandlerFactory, svs, pParams);
		srv.start();

		Poco::Net::StreamSocket ss(Poco::Net::SocketAddress("127.0.0.1", svs.address().port()));
		ss.setReceiveTimeout(Poco::Timespan(10, 0));
		const std::string request("POST /toggleBlocking HTTP/1.1\r\nHost: test\r\n" + head);
		ss.sendBytes(request.data(), static_cast<int>(request.size()));
		if (!receiveUntil(ss, "\r\n\r\nxxxxxxxxxx").has_value()) failmsg("server did not respond to the request");

		const std::string next(rest + "GET /echoHeader HTTP/1.1\r\nHost: test\r\nConnection: close\r\n\r\n");
		ss.sendBytes(next.data(), static_cast<int>(next.size()));
		const std::optional<std::string> response = receiveUntilClosed(ss);
		assertTrue (response.has_value());
		assertTrue (response->find("smuggled") == std::string::npos);
		assertTrue (response->find("Host: test") != std::string::npos);

		srv.stop();
	}
}


void HTTPServerTest::testIncompleteFixedLengthRequest()
{
	const std::string request("POST /bodyStatus HTTP/1.1\r\nHost: test\r\nContent-Length: 100\r\n\r\n0123456789");
	const std::optional<std::string> response = exchangeUntilClosed(new HTTPServerParams, request, true);
	assertTrue (response.has_value());
	assertTrue (response->find("\r\n\r\n10 incomplete") != std::string::npos);
}


void HTTPServerTest::testFieldNameWhitespace()
{
	const std::string body("GET /echoHeader HTTP/1.1\r\nHost: smuggled\r\n\r\n");
	const std::optional<std::string> response = exchangeUntilClosed(new HTTPServerParams,
		"POST /echoBody HTTP/1.1\r\nHost: test\r\nContent-Length : " + std::to_string(body.size()) + "\r\n\r\n" + body, false);
	assertTrue (response.has_value());
	assertTrue (response->compare(0, 12, "HTTP/1.1 400") == 0);
	assertTrue (response->find("smuggled") == std::string::npos);
}


void HTTPServerTest::testRedirectLineBreak()
{
	HTTPServerParams* pParams = new HTTPServerParams;
	pParams->setKeepAlive(false);
	const std::optional<std::string> response = exchangeUntilClosed(pParams,
		"GET /lineBreakRedirect HTTP/1.1\r\nHost: test\r\n\r\n", false);
	assertTrue (response.has_value());
	assertTrue (response->compare(0, 12, "HTTP/1.1 500") == 0);
	assertTrue (response->find("injected") == std::string::npos);
}


void HTTPServerTest::testSendLineBreak()
{
	HTTPServerParams* pParams = new HTTPServerParams;
	pParams->setKeepAlive(false);
	const std::optional<std::string> response = exchangeUntilClosed(pParams,
		"GET /lineBreakSend HTTP/1.1\r\nHost: test\r\n\r\n", false);
	assertTrue (response.has_value());
	assertTrue (response->compare(0, 12, "HTTP/1.1 500") == 0);
	assertTrue (response->find("injected") == std::string::npos);
}


void HTTPServerTest::testEchoDecodedLineBreak()
{
	HTTPServerParams* pParams = new HTTPServerParams;
	pParams->setKeepAlive(false);
	const std::optional<std::string> response = exchangeUntilClosed(pParams,
		"GET /echoField HTTP/1.1\r\nHost: test\r\nX-Echo: =?UTF-8?Q?x=0D=0ASet-Cookie:_injected=1?=\r\n\r\n", false);
	assertTrue (response.has_value());
	assertTrue (response->compare(0, 12, "HTTP/1.1 200") == 0);
	assertTrue (response->find("\r\nX-Echo: =?UTF-8?Q?x=0D=0ASet-Cookie:_injected=1?=\r\n") != std::string::npos);
	assertTrue (response->find("\r\nSet-Cookie") == std::string::npos);
}


CppUnit::Test* HTTPServerTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("HTTPServerTest");

	CppUnit_addTest(pSuite, HTTPServerTest, testIdentityRequest);
	CppUnit_addTest(pSuite, HTTPServerTest, testFramingConflicts);
	CppUnit_addTest(pSuite, HTTPServerTest, testPutIdentityRequest);
	CppUnit_addTest(pSuite, HTTPServerTest, testChunkedRequest);
	CppUnit_addTest(pSuite, HTTPServerTest, testIdentityRequestKeepAlive);
	CppUnit_addTest(pSuite, HTTPServerTest, testChunkedRequestKeepAlive);
	CppUnit_addTest(pSuite, HTTPServerTest, testMaxKeepAlive);
	CppUnit_addTest(pSuite, HTTPServerTest, testKeepAliveTimeout);
	CppUnit_addTest(pSuite, HTTPServerTest, test100Continue);
	CppUnit_addTest(pSuite, HTTPServerTest, testRedirect);
	CppUnit_addTest(pSuite, HTTPServerTest, testAuth);
	CppUnit_addTest(pSuite, HTTPServerTest, testNotImpl);
	CppUnit_addTest(pSuite, HTTPServerTest, testBuffer);
	CppUnit_addTest(pSuite, HTTPServerTest, testFile);
	CppUnit_addTest(pSuite, HTTPServerTest, testFileKeepAlive);
	CppUnit_addTest(pSuite, HTTPServerTest, testEmptyFileKeepAlive);
	CppUnit_addTest(pSuite, HTTPServerTest, testChunkedTrailer);
	CppUnit_addTest(pSuite, HTTPServerTest, testChunkedFramingError);
	CppUnit_addTest(pSuite, HTTPServerTest, testUnreadRequestBody);
	CppUnit_addTest(pSuite, HTTPServerTest, testUnreadChunkedRequestBody);
	CppUnit_addTest(pSuite, HTTPServerTest, testLargeUnreadRequestBody);
	CppUnit_addTest(pSuite, HTTPServerTest, testResponseBeforeUnreadRequestBody);
	CppUnit_addTest(pSuite, HTTPServerTest, testNonBlockingHandlerClosesConnection);
	CppUnit_addTest(pSuite, HTTPServerTest, testHandlerNonBlockingReadThenBlocking);
	CppUnit_addTest(pSuite, HTTPServerTest, testIncompleteFixedLengthRequest);
	CppUnit_addTest(pSuite, HTTPServerTest, testFieldNameWhitespace);
	CppUnit_addTest(pSuite, HTTPServerTest, testRedirectLineBreak);
	CppUnit_addTest(pSuite, HTTPServerTest, testSendLineBreak);
	CppUnit_addTest(pSuite, HTTPServerTest, testEchoDecodedLineBreak);

	return pSuite;
}
