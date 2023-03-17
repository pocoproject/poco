//
// EscapeHTMLStream.cpp
//
// Library: Net
// Package: Mail
// Module:  EscapeHTMLStream
//
// Copyright (c) 2019, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/EscapeHTMLStream.h"


namespace Poco {
namespace Net {


EscapeHTMLStreamBuf::EscapeHTMLStreamBuf(std::ostream& ostr):
	_pOstr(&ostr)
{
}


EscapeHTMLStreamBuf::~EscapeHTMLStreamBuf()
{
}


int EscapeHTMLStreamBuf::readFromDevice()
{
	return std::char_traits<char>::eof();
}


int EscapeHTMLStreamBuf::writeToDevice(char c)
{
	switch (c)
	{
	case '<':
		*_pOstr << "&lt;";
		break;
	case '>':
		*_pOstr << "&gt;";
		break;
	case '"':
		*_pOstr << "&quot;";
		break;
	case '&':
		*_pOstr << "&amp;";
		break;
	default:
		_pOstr->put(c);
		break;
	}
	return charToInt(c);
}


EscapeHTMLIOS::EscapeHTMLIOS(std::ostream& ostr): _buf(ostr)
{
	poco_ios_init(&_buf);
}


EscapeHTMLIOS::~EscapeHTMLIOS()
{
}


EscapeHTMLStreamBuf* EscapeHTMLIOS::rdbuf()
{
	return &_buf;
}


EscapeHTMLOutputStream::EscapeHTMLOutputStream(std::ostream& ostr):
	EscapeHTMLIOS(ostr),
	std::ostream(&_buf)
{
}


EscapeHTMLOutputStream::~EscapeHTMLOutputStream()
{
}


} } // namespace Poco::Net
