//
// FTPSStreamFactory.cpp
//
// Library: NetSSL_OpenSSL
// Package: FTPS
// Module:  FTPSStreamFactory
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/FTPSStreamFactory.h"
#include "Poco/Net/FTPSClientSession.h"
#include "Poco/Net/NetException.h"
#include "Poco/URI.h"
#include "Poco/URIStreamOpener.h"
#include "Poco/UnbufferedStreamBuf.h"
#include "Poco/Path.h"


using Poco::URIStreamFactory;
using Poco::URI;
using Poco::URIStreamOpener;
using Poco::UnbufferedStreamBuf;
using Poco::Path;


namespace Poco {
namespace Net {


class FTPSStreamBuf: public UnbufferedStreamBuf
{
public:
	FTPSStreamBuf(std::istream& istr):
		_istr(istr)
	{
		// make sure exceptions from underlying string propagate
		_istr.exceptions(std::ios::badbit);
	}

	~FTPSStreamBuf()
	{
	}

private:
	int readFromDevice()
	{
		return _istr.get();
	}

	std::istream& _istr;
};


class FTPSIOS: public virtual std::ios
{
public:
	FTPSIOS(std::istream& istr):
		_buf(istr)
	{
		poco_ios_init(&_buf);
	}

	~FTPSIOS()
	{
	}

	FTPSStreamBuf* rdbuf()
	{
		return &_buf;
	}

protected:
	FTPSStreamBuf _buf;
};


class FTPSStream: public FTPSIOS, public std::istream
{
public:
	FTPSStream(std::istream& istr, FTPSClientSession* pSession):
		FTPSIOS(istr),
		std::istream(&_buf),
		_pSession(pSession)
	{
	}

	~FTPSStream()
	{
		delete _pSession;
	}

private:
	FTPSClientSession* _pSession;
};


FTPSStreamFactory::FTPSStreamFactory()
{
}


FTPSStreamFactory::~FTPSStreamFactory()
{
}


std::istream* FTPSStreamFactory::open(const URI& uri)
{
	poco_assert (uri.getScheme() == "ftps");

	Poco::UInt16 port = uri.getPort();
	if (port == 0) port = FTPClientSession::FTP_PORT;
	FTPSClientSession* pSession = new FTPSClientSession(uri.getHost(), port);
	try
	{
		std::string username;
		std::string password;
		getUserInfo(uri, username, password);

		std::string path;
		char        type;
		getPathAndType(uri, path, type);

		pSession->login(username, password);
		if (type == 'a')
			pSession->setFileType(FTPClientSession::TYPE_TEXT);

		Path p(path, Path::PATH_UNIX);
		p.makeFile();
		for (int i = 0; i < p.depth(); ++i)
			pSession->setWorkingDirectory(p[i]);
		std::string file(p.getFileName());
		std::istream& istr = (type == 'd' ? pSession->beginList(file) : pSession->beginDownload(file));
		return new FTPSStream(istr, pSession);
	}
	catch (...)
	{
		delete pSession;
		throw;
	}
}


void FTPSStreamFactory::registerFactory()
{
	URIStreamOpener::defaultOpener().registerStreamFactory("ftps", new FTPSStreamFactory);
}


void FTPSStreamFactory::unregisterFactory()
{
	URIStreamOpener::defaultOpener().unregisterStreamFactory("ftps");
}


} } // namespace Poco::Net
