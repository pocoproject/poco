//
// LogFile.cpp
//
// Library: Foundation
// Package: Logging
// Module:  LogFile
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/LogFile.h"


#if defined(POCO_OS_FAMILY_WINDOWS)
#include "LogFile_WIN32.cpp"
#else
#include "LogFile_STD.cpp"
#endif


namespace Poco {


LogFile::LogFile(const std::string& rPath): LogFileImpl(rPath)
{
}


LogFile::~LogFile()
{
}


} // namespace Poco
