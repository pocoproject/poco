//
// NullStream.cpp
//
// Library: Foundation
// Package: Streams
// Module:  NullStream
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/NullStream.h"


namespace Poco {


NullStreamBuf::NullStreamBuf()
= default;


NullStreamBuf::~NullStreamBuf()
= default;


int NullStreamBuf::readFromDevice()
{
	return -1;
}


int NullStreamBuf::writeToDevice(char c)
{
	return charToInt(c);
}


NullIOS::NullIOS()
{
	poco_ios_init(&_buf);
}


NullIOS::~NullIOS()
= default;


NullInputStream::NullInputStream(): std::istream(&_buf)
{
}


NullInputStream::~NullInputStream()
= default;


NullOutputStream::NullOutputStream(): std::ostream(&_buf)
{
}


NullOutputStream::~NullOutputStream()
= default;


} // namespace Poco
