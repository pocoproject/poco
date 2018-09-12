//
// SocketReactorTest.cpp
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "SocketReactorTest.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"
#include "Poco/Net/SocketReactor.h"
#include "Poco/Net/SocketNotification.h"
#include "Poco/Net/SocketConnector.h"
#include "Poco/Net/SocketAcceptor.h"
#include "Poco/Net/ParallelSocketAcceptor.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Observer.h"
#include "Poco/Exception.h"
#include "Poco/Thread.h"
#include <sstream>


using Poco::Net::SocketReactor;
using Poco::Net::SocketConnector;
using Poco::Net::SocketAcceptor;
using Poco::Net::ParallelSocketAcceptor;
using Poco::Net::StreamSocket;
using Poco::Net::ServerSocket;
using Poco::Net::SocketAddress;
using Poco::Net::SocketNotification;
using Poco::Net::ReadableNotification;
using Poco::Net::WritableNotification;
using Poco::Net::TimeoutNotification;
using Poco::Net::ShutdownNotification;
using Poco::Observer;
using Poco::IllegalStateException;
using Poco::Thread;


namespace
{
	class EchoServiceHandler
	{
	public:
		EchoServiceHandler(const StreamSocket& socket, SocketReactor& reactor):
			_socket(socket),
			_reactor(reactor)
		{
			_reactor.addEventHandler(_socket, Observer<EchoServiceHandler, ReadableNotification>(*this, &EchoServiceHandler::onReadable));
		}

		~EchoServiceHandler()
		{
		}

		void onReadable(ReadableNotification* pNf)
		{
			pNf->release();
			char buffer[8];
			int n = _socket.receiveBytes(buffer, sizeof(buffer));
			if (n > 0)
			{
				_socket.sendBytes(buffer, n);
			}
			else
			{
				_reactor.removeEventHandler(_socket, Observer<EchoServiceHandler, ReadableNotification>(*this, &EchoServiceHandler::onReadable));
				delete this;
			}
		}

	private:
		StreamSocket   _socket;
		SocketReactor& _reactor;
	};

	class ClientServiceHandler
	{
	public:
		ClientServiceHandler(const StreamSocket& socket, SocketReactor& reactor):
			_socket(socket),
			_reactor(reactor),
			_or(*this, &ClientServiceHandler::onReadable),
			_ow(*this, &ClientServiceHandler::onWritable),
			_ot(*this, &ClientServiceHandler::onTimeout)
		{
			_timeout = false;
			_readableError = false;
			_writableError = false;
			_timeoutError = false;
			checkReadableObserverCount(0);
			_reactor.addEventHandler(_socket, _or);
			checkReadableObserverCount(1);
			checkWritableObserverCount(0);
			_reactor.addEventHandler(_socket, _ow);
			checkWritableObserverCount(1);
			checkTimeoutObserverCount(0);
			_reactor.addEventHandler(_socket, _ot);
			checkTimeoutObserverCount(1);
		}

		~ClientServiceHandler()
		{
		}

		void onReadable(ReadableNotification* pNf)
		{
			pNf->release();
			char buffer[32];
			int n = _socket.receiveBytes(buffer, sizeof(buffer));
			if (n > 0)
			{
				_str.write(buffer, n);
				_data += _str.str();
				_str.str("");
			}
			else
			{
				checkReadableObserverCount(1);
				_reactor.removeEventHandler(_socket, Observer<ClientServiceHandler, ReadableNotification>(*this, &ClientServiceHandler::onReadable));
				checkReadableObserverCount(0);
				if (_once || _data.size() == 8192)
				{
					_reactor.stop();
					delete this;
				}
			}
		}

		void onWritable(WritableNotification* pNf)
		{
			pNf->release();
			checkWritableObserverCount(1);
			_reactor.removeEventHandler(_socket, Observer<ClientServiceHandler, WritableNotification>(*this, &ClientServiceHandler::onWritable));
			checkWritableObserverCount(0);
			std::string data(1024, 'x');
			_socket.sendBytes(data.data(), (int) data.length());
			_socket.shutdownSend();
		}

		void onTimeout(TimeoutNotification* pNf)
		{
			pNf->release();
			_timeout = true;
			if (_closeOnTimeout)
			{
				_reactor.stop();
				delete this;
			}
		}

		static std::string data()
		{
			return _data;
		}
		
		static void resetData()
		{
			_data.clear();
		}
		
		static bool timeout()
		{
			return _timeout;
		}

		static bool getCloseOnTimeout()
		{
			return _closeOnTimeout;
		}

		static void setCloseOnTimeout(bool flag)
		{
			_closeOnTimeout = flag;
		}

		static bool readableError()
		{
			return _readableError;
		}

		static bool writableError()
		{
			return _writableError;
		}

		static bool timeoutError()
		{
			return _timeoutError;
		}

		static void setOnce(bool once = true)
		{
			_once = once;
		}

	private:
		void checkReadableObserverCount(std::size_t oro)
		{
			if (((oro == 0) && _reactor.hasEventHandler(_socket, _or)) ||
				((oro > 0) && !_reactor.hasEventHandler(_socket, _or)))
			{
				_readableError = true;
			}
		}

		void checkWritableObserverCount(std::size_t ow)
		{
			if (((ow == 0) && _reactor.hasEventHandler(_socket, _ow)) ||
				((ow > 0) && !_reactor.hasEventHandler(_socket, _ow)))
			{
				_writableError = true;
			}
		}

		void checkTimeoutObserverCount(std::size_t ot)
		{
			if (((ot == 0) && _reactor.hasEventHandler(_socket, _ot)) ||
				((ot > 0) && !_reactor.hasEventHandler(_socket, _ot)))
			{
				_timeoutError = true;
			}
		}

		StreamSocket                                         _socket;
		SocketReactor&                                       _reactor;
		Observer<ClientServiceHandler, ReadableNotification> _or;
		Observer<ClientServiceHandler, WritableNotification> _ow;
		Observer<ClientServiceHandler, TimeoutNotification>  _ot;
		std::stringstream                                    _str;
		static std::string                                   _data;
		static bool                                          _readableError;
		static bool                                          _writableError;
		static bool                                          _timeoutError;
		static bool                                          _timeout;
		static bool                                          _closeOnTimeout;
		static bool                                          _once;
	};


	std::string ClientServiceHandler::_data;
	bool ClientServiceHandler::_readableError = false;
	bool ClientServiceHandler::_writableError = false;
	bool ClientServiceHandler::_timeoutError = false;
	bool ClientServiceHandler::_timeout = false;
	bool ClientServiceHandler::_closeOnTimeout = false;
	bool ClientServiceHandler::_once = false;


	class FailConnector: public SocketConnector<ClientServiceHandler>
	{
	public:
		FailConnector(SocketAddress& address, SocketReactor& reactor):
			SocketConnector<ClientServiceHandler>(address, reactor),
			_failed(false),
			_shutdown(false)
		{
			reactor.addEventHandler(socket(), Observer<FailConnector, TimeoutNotification>(*this, &FailConnector::onTimeout));
			reactor.addEventHandler(socket(), Observer<FailConnector, ShutdownNotification>(*this, &FailConnector::onShutdown));
		}

		void onShutdown(ShutdownNotification* pNf)
		{
			pNf->release();
			_shutdown = true;
		}

		void onTimeout(TimeoutNotification* pNf)
		{
			pNf->release();
			_failed = true;
			reactor()->stop();
		}

		void onError(int error)
		{
			_failed = true;
			reactor()->stop();
		}

		bool failed() const
		{
			return _failed;
		}

		bool shutdown() const
		{
			return _shutdown;
		}

	private:
		bool _failed;
		bool _shutdown;
	};

	class DataServiceHandler
	{
	public:
		typedef std::vector<std::string> Data;

		DataServiceHandler(StreamSocket& socket, SocketReactor& reactor):
				_socket(socket),
				_reactor(reactor),
				_pos(0)
		{
			_data.resize(1);
			_reactor.addEventHandler(_socket, Observer<DataServiceHandler, ReadableNotification>(*this, &DataServiceHandler::onReadable));
			_reactor.addEventHandler(_socket, Observer<DataServiceHandler, ShutdownNotification>(*this, &DataServiceHandler::onShutdown));
		}

		~DataServiceHandler()
		{
			_reactor.removeEventHandler(_socket, Observer<DataServiceHandler, ReadableNotification>(*this, &DataServiceHandler::onReadable));
			_reactor.removeEventHandler(_socket, Observer<DataServiceHandler, ShutdownNotification>(*this, &DataServiceHandler::onShutdown));
		}

		void onReadable(ReadableNotification* pNf)
		{
			pNf->release();
			char buffer[64];
			int n = _socket.receiveBytes(&buffer[0], sizeof(buffer));
			if (n > 0)
			{
				_data[_pos].append(buffer, n);
				std::size_t pos;
				pos = _data[_pos].find('\n');
				if(pos != std::string::npos)
				{
					if (pos == _data[_pos].size() - 1)
					{
						_data[_pos].erase(pos, 1);
						_data.push_back(std::string());
					}
					else
					{
						_data.push_back(_data[_pos].substr(pos + 1));
						_data[_pos].erase(pos);
					}
					++_pos;
				}
			}
			else return;
		}

		void onShutdown(ShutdownNotification* pNf)
		{
			pNf->release();
			delete this;
		}

		static Data _data;

	private:
		StreamSocket   _socket;
		SocketReactor& _reactor;
		int            _pos;
	};

	DataServiceHandler::Data DataServiceHandler::_data;
}


SocketReactorTest::SocketReactorTest(const std::string& name): CppUnit::TestCase(name)
{
}


SocketReactorTest::~SocketReactorTest()
{
}


void SocketReactorTest::testSocketReactor()
{
	SocketAddress ssa;
	ServerSocket ss(ssa);
	SocketReactor reactor;
	SocketAcceptor<EchoServiceHandler> acceptor(ss, reactor);
	SocketAddress sa("127.0.0.1", ss.address().port());
	SocketConnector<ClientServiceHandler> connector(sa, reactor);
	ClientServiceHandler::setOnce(true);
	ClientServiceHandler::resetData();
	reactor.run();
	std::string data(ClientServiceHandler::data());
	assertTrue (data.size() == 1024);
	assertTrue (!ClientServiceHandler::readableError());
	assertTrue (!ClientServiceHandler::writableError());
	assertTrue (!ClientServiceHandler::timeoutError());
}


void SocketReactorTest::testSetSocketReactor()
{
	SocketAddress ssa;
	ServerSocket ss(ssa);
	SocketReactor reactor;
	SocketAcceptor<EchoServiceHandler> acceptor(ss);
	acceptor.setReactor(reactor);
	SocketAddress sa("127.0.0.1", ss.address().port());
	SocketConnector<ClientServiceHandler> connector(sa, reactor);
	ClientServiceHandler::setOnce(true);
	ClientServiceHandler::resetData();
	reactor.run();
	std::string data(ClientServiceHandler::data());
	assertTrue (data.size() == 1024);
	assertTrue (!ClientServiceHandler::readableError());
	assertTrue (!ClientServiceHandler::writableError());
	assertTrue (!ClientServiceHandler::timeoutError());
}


void SocketReactorTest::testParallelSocketReactor()
{
	SocketAddress ssa;
	ServerSocket ss(ssa);
	SocketReactor reactor;
	ParallelSocketAcceptor<EchoServiceHandler, SocketReactor> acceptor(ss, reactor);
	SocketAddress sa("127.0.0.1", ss.address().port());
	SocketConnector<ClientServiceHandler> connector1(sa, reactor);
	SocketConnector<ClientServiceHandler> connector2(sa, reactor);
	SocketConnector<ClientServiceHandler> connector3(sa, reactor);
	SocketConnector<ClientServiceHandler> connector4(sa, reactor);
	SocketConnector<ClientServiceHandler> connector5(sa, reactor);
	SocketConnector<ClientServiceHandler> connector6(sa, reactor);
	SocketConnector<ClientServiceHandler> connector7(sa, reactor);
	SocketConnector<ClientServiceHandler> connector8(sa, reactor);
	ClientServiceHandler::setOnce(false);
	ClientServiceHandler::resetData();
	reactor.run();
	std::string data(ClientServiceHandler::data());
	assertTrue (data.size() == 8192);
	assertTrue (!ClientServiceHandler::readableError());
	assertTrue (!ClientServiceHandler::writableError());
	assertTrue (!ClientServiceHandler::timeoutError());
}


void SocketReactorTest::testSocketConnectorFail()
{
	SocketReactor reactor;
	reactor.setTimeout(Poco::Timespan(3, 0));
	SocketAddress sa("192.168.168.192", 12345);
	FailConnector connector(sa, reactor);
	assertTrue (!connector.failed());
	assertTrue (!connector.shutdown());
	reactor.run();
	assertTrue (connector.failed());
	assertTrue (connector.shutdown());
}


void SocketReactorTest::testSocketConnectorTimeout()
{
	ClientServiceHandler::setCloseOnTimeout(true);

	SocketAddress ssa;
	ServerSocket ss(ssa);
	SocketReactor reactor;
	SocketAddress sa("127.0.0.1", ss.address().port());
	SocketConnector<ClientServiceHandler> connector(sa, reactor);
	reactor.run();
	assertTrue (ClientServiceHandler::timeout());
}


void SocketReactorTest::testDataCollection()
{
	SocketAddress ssa;
	ServerSocket ss(ssa);
	SocketReactor reactor;
	SocketAcceptor<DataServiceHandler> acceptor(ss, reactor);
	Thread thread;
	thread.start(reactor);

	SocketAddress sa("127.0.0.1", ss.address().port());
	StreamSocket sock(sa);

	std::string data0("{"
					  "  \"src\":\"127.0.0.1\","
					  "  \"id\":\"test0\","
					  "  \"ts\":\"1524864651000001\","
					  "  \"data\":123"
					  "}\n");
	sock.sendBytes(data0.data(), static_cast<int>(data0.size()));

	std::string data1("{"
					  "  \"src\":\"127.0.0.1\","
					  "  \"id\":\"test1\","
					  "  \"ts\":\"1524864651123456\","
					  "  \"data\":"
					  "  ["
					  "   {"
					  "     \"tag1\":"
					  "     ["
					  "      {\"val1\":123},"
					  "      {\"val2\":\"abc\"}"
					  "     ]"
					  "   }"
					  "  ]"
					  "}\n");
	sock.sendBytes(data1.data(), static_cast<int>(data1.size()));

	std::string data2 = "{"
						"  \"src\":\"127.0.0.1\","
						"  \"id\":\"test2\","
						"  \"ts\":\"1524864652654321\","
						"  \"data\":"
						"  ["
						"    {"
						"     \"tag1\":"
						"     ["
						"      {"
						"       \"val1\":123,"
						"       \"val2\":\"abc\","
						"       \"val3\":42.123"
						"      },"
						"      {"
						"       \"val1\":987,"
						"       \"val2\":\"xyz\","
						"       \"val3\":24.321"
						"      }"
						"     ],"
						"     \"tag2\":"
						"     ["
						"      {"
						"       \"val1\":42.123,"
						"       \"val2\":123,"
						"       \"val3\":\"abc\""
						"      },"
						"      {"
						"       \"val1\":24.321,"
						"       \"val2\":987,"
						"       \"val3\":\"xyz\""
						"      }"
						"    ]"
						"   }"
						" ]"
						"}\n";
	sock.sendBytes(data2.data(), static_cast<int>(data2.size()));
	Thread::sleep(500);
	reactor.stop();
	thread.join();

	assertTrue (DataServiceHandler::_data.size() == 4);
	data0.erase(data0.size() - 1);
	assertTrue (DataServiceHandler::_data[0] == data0);
	data1.erase(data1.size() - 1);
	assertTrue (DataServiceHandler::_data[1] == data1);
	data2.erase(data2.size() - 1);
	assertTrue (DataServiceHandler::_data[2] == data2);
	assertTrue (DataServiceHandler::_data[3].empty());
}


void SocketReactorTest::setUp()
{
	ClientServiceHandler::setCloseOnTimeout(false);
}


void SocketReactorTest::tearDown()
{
}


CppUnit::Test* SocketReactorTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SocketReactorTest");

	CppUnit_addTest(pSuite, SocketReactorTest, testSocketReactor);
	CppUnit_addTest(pSuite, SocketReactorTest, testSetSocketReactor);
	CppUnit_addTest(pSuite, SocketReactorTest, testParallelSocketReactor);
	CppUnit_addTest(pSuite, SocketReactorTest, testSocketConnectorFail);
	CppUnit_addTest(pSuite, SocketReactorTest, testSocketConnectorTimeout);
	CppUnit_addTest(pSuite, SocketReactorTest, testDataCollection);

	return pSuite;
}
