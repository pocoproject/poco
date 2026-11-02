//
// Utility.cpp
//
// Library: Data/MySQL
// Package: MySQL
// Module:  Utility
//
// Implementation of Utility
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Data/MySQL/Utility.h"
#include <mysql/mysql.h>


namespace Poco::Data::MySQL {


std::string Utility::serverInfo(MYSQL* pHandle)
{
	poco_check_ptr (pHandle);
	const char* pInfo = mysql_get_server_info(pHandle);
	poco_check_ptr (pInfo);
	std::string info(pInfo);
	return info;
}


std::string Utility::serverInfo(Session& session)
{
	return serverInfo(handle(session));
}


unsigned long Utility::serverVersion(MYSQL* pHandle)
{
	poco_check_ptr (pHandle);
	return mysql_get_server_version(pHandle);
}


unsigned long Utility::serverVersion(Session& session)
{
	return serverVersion(handle(session));
}


std::string Utility::hostInfo(MYSQL* pHandle)
{
	poco_check_ptr (pHandle);
	const char* pInfo = mysql_get_host_info(pHandle);
	poco_check_ptr (pInfo);
	std::string info(pInfo);
	return info;
}


std::string Utility::hostInfo(Session& session)
{
	return hostInfo(handle(session));
}


} // namespace Poco::Data::MySQL
