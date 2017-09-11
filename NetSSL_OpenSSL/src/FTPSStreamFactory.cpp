//
// FTPSStreamFactory.cpp
//
// Library: Net
// Package: FTP
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


class FTPStreamBuf: public UnbufferedStreamBuf
{
public:
	FTPStreamBuf(std::istream& istr):
		_istr(istr)
	{
		// make sure exceptions from underlying string propagate
		_istr.exceptions(std::ios::badbit);
	}
	
	~FTPStreamBuf()
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
	
	FTPStreamBuf* rdbuf()
	{
		return &_buf;
	}

protected:
	FTPStreamBuf _buf;
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


FTPPasswordProvider::FTPPasswordProvider()
{
}


FTPPasswordProvider::~FTPPasswordProvider()
{
}


std::string          FTPSStreamFactory::_anonymousPassword("poco@localhost");
FTPPasswordProvider* FTPSStreamFactory::_pPasswordProvider(0);


FTPSStreamFactory::FTPSStreamFactory()
{
}


FTPSStreamFactory::~FTPSStreamFactory()
{
}


std::istream* FTPSStreamFactory::open(const URI& uri)
{
	poco_assert (uri.getScheme() == "ftps");

	FTPSClientSession* pSession = new FTPSClientSession(uri.getHost(), uri.getPort());
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


void FTPSStreamFactory::setAnonymousPassword(const std::string& password)
{
	_anonymousPassword = password;
}

	
const std::string& FTPSStreamFactory::getAnonymousPassword()
{
	return _anonymousPassword;
}

	
void FTPSStreamFactory::setPasswordProvider(FTPPasswordProvider* pProvider)
{
	_pPasswordProvider = pProvider;
}

	
FTPPasswordProvider* FTPSStreamFactory::getPasswordProvider()
{
	return _pPasswordProvider;
}


void FTPSStreamFactory::splitUserInfo(const std::string& userInfo, std::string& username, std::string& password)
{
	std::string::size_type pos = userInfo.find(':');
	if (pos != std::string::npos)
	{
		username.assign(userInfo, 0, pos++);
		password.assign(userInfo, pos, userInfo.size() - pos);
	}
	else username = userInfo;
}


void FTPSStreamFactory::getUserInfo(const URI& uri, std::string& username, std::string& password)
{
	splitUserInfo(uri.getUserInfo(), username, password);
	if (username.empty())
	{
		username = "anonymous";
		password = _anonymousPassword;
	}
	else if (password.empty())
	{
		if (_pPasswordProvider)
			password = _pPasswordProvider->password(username, uri.getHost());
		else
			throw FTPException(std::string("Password required for ") + username + "@" + uri.getHost());
	}
}


void FTPSStreamFactory::getPathAndType(const Poco::URI& uri, std::string& path, char& type)
{
	path = uri.getPath();
	type = 'i';
	std::string::size_type pos = path.rfind(';');
	if (pos != std::string::npos)
	{
		if (path.length() == pos + 7 && path.compare(pos + 1, 5, "type=") == 0)
		{
			type = path[pos + 6];
			path.resize(pos);
		}
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
