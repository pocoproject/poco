//
// ApacheStream.h
//
// $Id: //poco/Main/ApacheConnector/src/ApacheStream.cpp#9 $
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "ApacheStream.h"
#include "ApacheConnector.h"
#include "Poco/Exception.h"


using Poco::BufferedStreamBuf;


//
// ApacheStreamBuf
//


ApacheStreamBuf::ApacheStreamBuf(ApacheRequestRec* pApacheRequest, bool haveData): 
	BufferedStreamBuf(STREAM_BUFFER_SIZE, std::ios::in | std::ios::out),
	_pApacheRequest(pApacheRequest),
	_haveData(haveData)
{
}


ApacheStreamBuf::~ApacheStreamBuf()
{
}


int ApacheStreamBuf::readFromDevice(char* buffer, std::streamsize len)
{
	if (_haveData)
		return _pApacheRequest->readRequest(buffer, static_cast<int>(len));
	else
		return 0;
}


int ApacheStreamBuf::writeToDevice(const char* buffer, std::streamsize length)
{
	_pApacheRequest->writeResponse(buffer, length);
	return length;
}


//
// ApacheIOS
//


ApacheIOS::ApacheIOS(ApacheRequestRec* pApacheRequest, bool haveData):
	_buf(pApacheRequest, haveData)
{
	poco_ios_init(&_buf);
}


ApacheIOS::~ApacheIOS()
{
	try
	{
		_buf.sync();
	}
	catch (...)
	{
	}
}


ApacheStreamBuf* ApacheIOS::rdbuf()
{
	return &_buf;
}


void ApacheIOS::close()
{
	_buf.sync();
}


//
// ApacheOutputStream
//


ApacheOutputStream::ApacheOutputStream(ApacheRequestRec* pApacheRequest):
	ApacheIOS(pApacheRequest),
	std::ostream(&_buf)
{
}


ApacheOutputStream::~ApacheOutputStream()
{
}


//
// ApacheInputStream
//


ApacheInputStream::ApacheInputStream(ApacheRequestRec* pApacheRequest):
	ApacheIOS(pApacheRequest, pApacheRequest->haveRequestBody()),
	std::istream(&_buf)
{
}


ApacheInputStream::~ApacheInputStream()
{
}
