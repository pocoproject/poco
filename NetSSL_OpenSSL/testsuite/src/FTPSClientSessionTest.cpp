//
// FTPSClientSessionTest.cpp
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "FTPSClientSessionTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Net/FTPSClientSession.h"
#include "Poco/Net/DialogSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/NetException.h"
#include "Poco/Thread.h"
#include "Poco/ActiveMethod.h"
#include "Poco/StreamCopier.h"
#include "Poco/Net/Session.h"
#include "DialogServer.h"
#include <sstream>


using Poco::Net::FTPSClientSession;
using Poco::Net::DialogSocket;
using Poco::Net::SocketAddress;
using Poco::Net::FTPException;
using Poco::ActiveMethod;
using Poco::ActiveResult;
using Poco::StreamCopier;
using Poco::Thread;
using Poco::Net::Session;


namespace
{
	class ActiveDownloader
	{
	public:
		ActiveDownloader(FTPSClientSession& session):
			download(this, &ActiveDownloader::downloadImp),
			_session(session)
		{
		}

		ActiveMethod<std::string, std::string, ActiveDownloader> download;

	protected:
		std::string downloadImp(const std::string& path)
		{
			std::istream& istr = _session.beginDownload(path);
			std::ostringstream ostr;
			StreamCopier::copyStream(istr, ostr);
			_session.endDownload();
			return ostr.str();
		}

	private:
		FTPSClientSession& _session;
	};
};


FTPSClientSessionTest::FTPSClientSessionTest(const std::string& name): CppUnit::TestCase(name)
{
}


FTPSClientSessionTest::~FTPSClientSessionTest()
{
}


void FTPSClientSessionTest::login(DialogServer& server, FTPSClientSession& session)
{
	server.addResponse("331 Password required");
	server.addResponse("230 Welcome");
	server.addResponse("200 Type set to I");
	session.login("user", "password");
	std::string cmd = server.popCommand();
	assertTrue (cmd == "USER user");
	cmd = server.popCommand();
	assertTrue (cmd == "PASS password");
	cmd = server.popCommand();
	assertTrue (cmd == "TYPE I");

	assertTrue (session.getFileType() == FTPSClientSession::TYPE_BINARY);
}


void FTPSClientSessionTest::testLogin1()
{
	DialogServer server;
	server.addResponse("220 localhost FTP ready");
	FTPSClientSession session("127.0.0.1", server.port());
	assertTrue (session.isOpen());
	assertTrue (!session.isLoggedIn());
	login(server, session);
	assertTrue (session.isOpen());
	assertTrue (session.isLoggedIn());
	server.addResponse("221 Good Bye");
	session.logout();
	assertTrue (session.isOpen());
	assertTrue (!session.isLoggedIn());

	server.clearCommands();
	server.clearResponses();

	session.enableFTPS(true);
	login(server, session);
	assertTrue (session.isOpen());
	assertTrue (session.isLoggedIn());
	server.addResponse("221 Good Bye");
	session.close();
	assertTrue (!session.isOpen());
	assertTrue (!session.isLoggedIn());
}


void FTPSClientSessionTest::testLogin2()
{
	DialogServer server;
	server.addResponse("220 localhost FTP ready");

	server.addResponse("331 Password required");
	server.addResponse("230 Welcome");
	server.addResponse("200 Type set to I");
	Poco::UInt16 serverPort = server.port();
	FTPSClientSession session("127.0.0.1", serverPort, "user", "password");
	assertTrue (session.isOpen());
	assertTrue (session.isLoggedIn());
	server.addResponse("221 Good Bye");
	session.close();
	assertTrue (!session.isOpen());
	assertTrue (!session.isLoggedIn());

	server.clearCommands();
	server.clearResponses();
	server.addResponse("220 localhost FTP ready");

	server.addResponse("331 Password required");
	server.addResponse("230 Welcome");
	server.addResponse("200 Type set to I");
	session.enableFTPS(true);
	session.open("127.0.0.1", serverPort, "user", "password");
	assertTrue (session.isOpen());
	assertTrue (session.isLoggedIn());
	server.addResponse("221 Good Bye");
	session.close();
	assertTrue (!session.isOpen());
	assertTrue (!session.isLoggedIn());
}


void FTPSClientSessionTest::testLogin3()
{
	DialogServer server;
	server.addResponse("220 localhost FTP ready");
	server.addResponse("331 Password required");
	server.addResponse("230 Welcome");
	server.addResponse("200 Type set to I");
	FTPSClientSession session;
	assertTrue (!session.isOpen());
	assertTrue (!session.isLoggedIn());
	session.open("127.0.0.1", server.port(), "user", "password");
	server.addResponse("221 Good Bye");
	session.close();
	assertTrue (!session.isOpen());
	assertTrue (!session.isLoggedIn());
}



void FTPSClientSessionTest::testLoginFailed1()
{
	DialogServer server;
	server.addResponse("421 localhost FTP not ready");
	FTPSClientSession session("127.0.0.1", server.port());
	try
	{
		session.login("user", "password");
		fail("server not ready - must throw");
	}
	catch (FTPException&)
	{
	}
	server.addResponse("221 Good Bye");
	session.close();
}


void FTPSClientSessionTest::testLoginFailed2()
{
	DialogServer server;
	server.addResponse("220 localhost FTP ready");
	server.addResponse("331 Password required");
	server.addResponse("530 Login incorrect");
	FTPSClientSession session("127.0.0.1", server.port());
	try
	{
		session.login("user", "password");
		fail("login incorrect - must throw");
	}
	catch (FTPException&)
	{
	}
	server.addResponse("221 Good Bye");
	session.close();
}


void FTPSClientSessionTest::testCommands()
{
	DialogServer server;
	server.addResponse("220 localhost FTP ready");
	server.addResponse("331 Password required");
	server.addResponse("230 Welcome");
	server.addResponse("200 Type set to I");
	FTPSClientSession session("127.0.0.1", server.port());
	session.login("user", "password");
	std::string cmd = server.popCommand();
	assertTrue (cmd == "USER user");
	cmd = server.popCommand();
	assertTrue (cmd == "PASS password");
	cmd = server.popCommand();
	assertTrue (cmd == "TYPE I");

	// systemType
	server.clearCommands();
	server.addResponse("215 UNIX Type: L8 Version: dummyFTP 1.0");
	std::string type = session.systemType();
	cmd = server.popCommand();
	assertTrue (cmd == "SYST");
	assertTrue (type == "UNIX Type: L8 Version: dummyFTP 1.0");

	// getWorkingDirectory
	server.addResponse("257 \"/usr/test\" is current directory");
	std::string cwd = session.getWorkingDirectory();
	cmd = server.popCommand();
	assertTrue (cmd == "PWD");
	assertTrue (cwd == "/usr/test");

	// getWorkingDirectory (quotes in filename)
	server.addResponse("257 \"\"\"quote\"\"\" is current directory");
	cwd = session.getWorkingDirectory();
	cmd = server.popCommand();
	assertTrue (cmd == "PWD");
	assertTrue (cwd == "\"quote\"");

	// setWorkingDirectory
	server.addResponse("250 CWD OK");
	session.setWorkingDirectory("test");
	cmd = server.popCommand();
	assertTrue (cmd == "CWD test");

	server.addResponse("250 CDUP OK");
	session.cdup();
	cmd = server.popCommand();
	assertTrue (cmd == "CDUP");

	// rename
	server.addResponse("350 File exists, send destination name");
	server.addResponse("250 Rename OK");
	session.rename("old.txt", "new.txt");
	cmd = server.popCommand();
	assertTrue (cmd == "RNFR old.txt");
	cmd = server.popCommand();
	assertTrue (cmd == "RNTO new.txt");

	// rename (failing)
	server.addResponse("550 not found");
	try
	{
		session.rename("old.txt", "new.txt");
		fail("not found - must throw");
	}
	catch (FTPException&)
	{
	}
	server.clearCommands();

	// remove
	server.addResponse("250 delete ok");
	session.remove("test.txt");
	cmd = server.popCommand();
	assertTrue (cmd == "DELE test.txt");

	// remove (failing)
	server.addResponse("550 not found");
	try
	{
		session.remove("test.txt");
		fail("not found - must throw");
	}
	catch (FTPException&)
	{
	}
	server.clearCommands();

	// createDirectory
	server.addResponse("257 dir created");
	session.createDirectory("foo");
	cmd = server.popCommand();
	assertTrue (cmd == "MKD foo");

	// createDirectory (failing)
	server.addResponse("550 exists");
	try
	{
		session.createDirectory("foo");
		fail("not found - must throw");
	}
	catch (FTPException&)
	{
	}
	server.clearCommands();

	// removeDirectory
	server.addResponse("250 RMD OK");
	session.removeDirectory("foo");
	cmd = server.popCommand();
	assertTrue (cmd == "RMD foo");

	// removeDirectory (failing)
	server.addResponse("550 not found");
	try
	{
		session.removeDirectory("foo");
		fail("not found - must throw");
	}
	catch (FTPException&)
	{
	}
	server.clearCommands();

	server.addResponse("221 Good Bye");
	session.close();
}


void FTPSClientSessionTest::testDownloadPORT()
{
	DialogServer server;
	server.addResponse("220 localhost FTP ready");
	server.addResponse("331 Password required");
	server.addResponse("230 Welcome");
	server.addResponse("200 Type set to I");
	FTPSClientSession session("127.0.0.1", server.port());
	session.setPassive(false);
	session.login("user", "password");
	server.clearCommands();

	server.addResponse("500 EPRT not understood");
	server.addResponse("200 PORT OK");
	server.addResponse("150 Sending data\r\n226 Transfer complete");

	ActiveDownloader dl(session);
	ActiveResult<std::string> result = dl.download("test.txt");

	std::string cmd = server.popCommandWait();
	assertTrue (cmd.substr(0, 4) == "EPRT");

	cmd = server.popCommandWait();
	assertTrue (cmd.substr(0, 4) == "PORT");

	std::string dummy;
	int x, lo, hi;
	for (std::string::iterator it = cmd.begin(); it != cmd.end(); ++it)
	{
		if (*it == ',') *it = ' ';
	}
	std::istringstream istr(cmd);
	istr >> dummy >> x >> x >> x >> x >> hi >> lo;
	int port = hi*256 + lo;

	cmd = server.popCommandWait();
	assertTrue (cmd == "RETR test.txt");

	SocketAddress sa("127.0.0.1", (Poco::UInt16) port);
	DialogSocket dataSock;
	dataSock.connect(sa);

	std::string data("This is some data");
	dataSock.sendString(data);
	dataSock.close();

	result.wait();
	std::string received = result.data();
	assertTrue (received == data);

	server.addResponse("221 Good Bye");
	session.close();
}


void FTPSClientSessionTest::testDownloadEPRT()
{
	DialogServer server;
	server.addResponse("220 localhost FTP ready");
	server.addResponse("331 Password required");
	server.addResponse("230 Welcome");
	server.addResponse("200 Type set to I");
	FTPSClientSession session("127.0.0.1", server.port());
	session.setPassive(false);
	session.login("user", "password");
	server.clearCommands();

	server.addResponse("200 EPRT OK");
	server.addResponse("150 Sending data\r\n226 Transfer complete");

	ActiveDownloader dl(session);
	ActiveResult<std::string> result = dl.download("test.txt");

	std::string cmd = server.popCommandWait();
	assertTrue (cmd.substr(0, 4) == "EPRT");

	std::string dummy;
	char c;
	int d;
	int port;
	std::istringstream istr(cmd);
	istr >> dummy >> c >> d >> c >> d >> c >> d >> c >> d >> c >> d >> c >> port >> c;

	cmd = server.popCommandWait();
	assertTrue (cmd == "RETR test.txt");

	SocketAddress sa("127.0.0.1", (Poco::UInt16) port);
	DialogSocket dataSock;
	dataSock.connect(sa);

	std::string data("This is some data");
	dataSock.sendString(data);
	dataSock.close();

	result.wait();
	std::string received = result.data();
	assertTrue (received == data);

	server.addResponse("221 Good Bye");
	session.close();
}


void FTPSClientSessionTest::testDownloadPASV()
{
	DialogServer server;
	server.addResponse("220 localhost FTP ready");
	server.addResponse("331 Password required");
	server.addResponse("230 Welcome");
	server.addResponse("200 Type set to I");
	FTPSClientSession session("127.0.0.1", server.port());
	session.login("user", "password");
	server.clearCommands();

	server.addResponse("500 EPSV not understood");

	DialogServer dataServer(false);
	Poco::UInt16 dataServerPort = dataServer.port();
	dataServer.addResponse("This is some data");
	std::ostringstream pasv;
	pasv << "227 Entering Passive Mode (127,0,0,1," << (dataServerPort/256) << "," << (dataServerPort % 256) << ")";
	server.addResponse(pasv.str());
	server.addResponse("150 sending data\r\n226 Transfer complete");

	std::istream& istr = session.beginDownload("test.txt");
	std::ostringstream dataStr;
	StreamCopier::copyStream(istr, dataStr);
	session.endDownload();
	std::string s(dataStr.str());
	assertTrue (s == "This is some data\r\n");

	server.addResponse("221 Good Bye");
	session.close();
}


void FTPSClientSessionTest::testDownloadEPSV()
{
	DialogServer server;
	server.addResponse("220 localhost FTP ready");
	server.addResponse("331 Password required");
	server.addResponse("230 Welcome");
	server.addResponse("200 Type set to I");
	FTPSClientSession session("127.0.0.1", server.port());
	session.login("user", "password");
	server.clearCommands();

	DialogServer dataServer(false);
	dataServer.addResponse("This is some data");
	std::ostringstream epsv;
	epsv << "229 Entering Extended Passive Mode (|||" << dataServer.port() << "|)";
	server.addResponse(epsv.str());
	server.addResponse("150 sending data\r\n226 Transfer complete");

	std::istream& istr = session.beginDownload("test.txt");
	std::ostringstream dataStr;
	StreamCopier::copyStream(istr, dataStr);
	session.endDownload();
	std::string s(dataStr.str());
	assertTrue (s == "This is some data\r\n");

	std::string cmd = server.popCommand();
	assertTrue (cmd.substr(0, 4) == "EPSV");
	cmd = server.popCommand();
	assertTrue (cmd == "RETR test.txt");

	server.addResponse("221 Good Bye");
	session.close();
}


void FTPSClientSessionTest::testUpload()
{
	DialogServer server;
	server.addResponse("220 localhost FTP ready");
	server.addResponse("331 Password required");
	server.addResponse("230 Welcome");
	server.addResponse("200 Type set to I");
	FTPSClientSession session("127.0.0.1", server.port());
	session.login("user", "password");
	server.clearCommands();

	DialogServer dataServer;
	std::ostringstream epsv;
	epsv << "229 Entering Extended Passive Mode (|||" << dataServer.port() << "|)";
	server.addResponse(epsv.str());
	server.addResponse("150 send data\r\n226 Transfer complete");

	std::ostream& ostr = session.beginUpload("test.txt");
	ostr << "This is some data\r\n";
	session.endUpload();
	std::string s(dataServer.popCommandWait());
	assertTrue (s == "This is some data");

	std::string cmd = server.popCommand();
	assertTrue (cmd.substr(0, 4) == "EPSV");
	cmd = server.popCommand();
	assertTrue (cmd == "STOR test.txt");

	server.addResponse("221 Good Bye");
	session.close();
}


void FTPSClientSessionTest::testUploadSSL()
{
	DialogServer server(true, true);
	server.addResponse("220 localhost FTP ready");
	server.addResponse("331 Password required");
	server.addResponse("230 Welcome");
	server.addResponse("200 Type set to I");
	FTPSClientSession session("127.0.0.1", server.port());
	session.login("user", "password");
	server.clearCommands();

	DialogServer dataServer(true, true);
	Session::Ptr cSessionSSL = server.getSslSession();
	dataServer.setSslSession(cSessionSSL);

	std::ostringstream epsv;
	epsv << "229 Entering Extended Passive Mode (|||" << dataServer.port() << "|)";
	server.addResponse(epsv.str());
	server.addResponse("150 send data\r\n226 Transfer complete");

	std::ostream& ostr = session.beginUpload("test.txt");
	ostr << "This is some data\r\n";
	session.endUpload();
	//std::string s(dataServer.popCommandWait());
	//assertTrue (s == "This is some data");

	std::string cmd = server.popCommand();
	assertTrue (cmd.substr(0, 4) == "EPSV");
	cmd = server.popCommand();
	assertTrue (cmd == "STOR test.txt");

	server.addResponse("221 Good Bye");
	session.close();
}


void FTPSClientSessionTest::testList()
{
	DialogServer server;
	server.addResponse("220 localhost FTP ready");
	server.addResponse("331 Password required");
	server.addResponse("230 Welcome");
	server.addResponse("200 Type set to I");
	FTPSClientSession session("127.0.0.1", server.port());
	session.login("user", "password");
	server.clearCommands();

	DialogServer dataServer(false);
	dataServer.addResponse("file1\r\nfile2");
	std::ostringstream epsv;
	epsv << "229 Entering Extended Passive Mode (|||" << dataServer.port() << "|)";
	server.addResponse(epsv.str());
	server.addResponse("150 sending data\r\n226 Transfer complete");

	std::istream& istr = session.beginList();
	std::ostringstream dataStr;
	StreamCopier::copyStream(istr, dataStr);
	session.endList();
	std::string s(dataStr.str());
	assertTrue (s == "file1\r\nfile2\r\n");

	std::string cmd = server.popCommand();
	assertTrue (cmd.substr(0, 4) == "EPSV");
	cmd = server.popCommand();
	assertTrue (cmd == "NLST");

	server.addResponse("221 Good Bye");
	session.close();
}


void FTPSClientSessionTest::setUp()
{
}


void FTPSClientSessionTest::tearDown()
{
}


CppUnit::Test* FTPSClientSessionTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("FTPSClientSessionTest");

	CppUnit_addTest(pSuite, FTPSClientSessionTest, testLogin1);
	CppUnit_addTest(pSuite, FTPSClientSessionTest, testLogin2);
	CppUnit_addTest(pSuite, FTPSClientSessionTest, testLogin3);
	CppUnit_addTest(pSuite, FTPSClientSessionTest, testLoginFailed1);
	CppUnit_addTest(pSuite, FTPSClientSessionTest, testLoginFailed2);
	CppUnit_addTest(pSuite, FTPSClientSessionTest, testCommands);
	CppUnit_addTest(pSuite, FTPSClientSessionTest, testDownloadPORT);
	CppUnit_addTest(pSuite, FTPSClientSessionTest, testDownloadEPRT);
	CppUnit_addTest(pSuite, FTPSClientSessionTest, testDownloadPASV);
	CppUnit_addTest(pSuite, FTPSClientSessionTest, testDownloadEPSV);
	CppUnit_addTest(pSuite, FTPSClientSessionTest, testUpload);
	CppUnit_addTest(pSuite, FTPSClientSessionTest, testList);
	CppUnit_addTest(pSuite, FTPSClientSessionTest, testUploadSSL);

	return pSuite;
}
