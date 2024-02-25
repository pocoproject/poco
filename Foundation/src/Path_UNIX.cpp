//
// Path_UNIX.cpp
//
// Library: Foundation
// Package: Filesystem
// Module:  Path
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Path_UNIX.h"
#include "Poco/Exception.h"
#include "Poco/Environment_UNIX.h"
#include "Poco/Ascii.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <climits>


#if !defined(POCO_VXWORKS)
#include <pwd.h>
#endif


#if POCO_OS == POCO_OS_MAC_OS_X
#include <mach-o/dyld.h>
#elif POCO_OS == POCO_OS_FREE_BSD
#include <sys/sysctl.h>
#elif POCO_OS == POCO_OS_LINUX
#include <fcntl.h>
#endif


#ifndef PATH_MAX
#define PATH_MAX 4096 // fallback
#endif


using namespace std::string_literals;


namespace Poco {


std::string PathImpl::selfImpl()
{
	std::string path;
	char buf[PATH_MAX + 1] {0};

#if POCO_OS == POCO_OS_MAC_OS_X
	std::uint32_t size = sizeof(buf);
	if (_NSGetExecutablePath(buf, &size) == 0)
		path = buf;
	else
		throw Poco::SystemException("Cannot get path of the current process.");
#elif POCO_OS == POCO_OS_FREE_BSD
	int mib[4];
	mib[0] = CTL_KERN;
	mib[1] = KERN_PROC;
	mib[2] = KERN_PROC_PATHNAME;
	mib[3] = -1;
	std::size_t size = sizeof(buf);
	if (sysctl(mib, 4, buf, &size, NULL, 0) == 0)
		path = buf;
	else
		throw Poco::SystemException("Cannot get path of the current process.");
#elif POCO_OS == POCO_OS_NET_BSD
	std::size_t size = sizeof(buf);
	int n = readlink("/proc/curproc/exe", buf, size);
	if (n > 0 && n < PATH_MAX)
		path = buf;
#elif POCO_OS == POCO_OS_SOLARIS
	char * execName = getexecname();
	if (execName)
		path = execName;
	else
		throw Poco::SystemException("Cannot get path of the current process.");
#elif POCO_OS == POCO_OS_LINUX || POCO_OS == POCO_OS_ANDROID
	const std::size_t size = sizeof(buf);
	int n = readlink("/proc/self/exe", buf, size);
	if (n > 0 && n < PATH_MAX)
		path = buf;
	else
		throw Poco::SystemException("Cannot get path of the current process.");
#else
	throw Poco::NotImplementedException("File path of the current process not implemented on this platform.");
#endif

	return path;
}


std::string PathImpl::currentImpl()
{
	std::string path;
	char cwd[PATH_MAX];
	if (getcwd(cwd, sizeof(cwd)))
		path = cwd;
	else
		throw SystemException("cannot get current directory");
	std::string::size_type n = path.size();
	if (n > 0 && path[n - 1] != '/') path.append("/");
	return path;
}


std::string PathImpl::homeImpl()
{
#if defined(POCO_VXWORKS)
	if (EnvironmentImpl::hasImpl("HOME"))
	{
		std::string path = EnvironmentImpl::getImpl("HOME");
		std::string::size_type n = path.size();
		if (n > 0 && path[n - 1] != '/') path.append("/");
		return path;
	}
	else return "/";
#else
	std::string path;
	if (EnvironmentImpl::hasImpl("HOME"s))
	{
		path = EnvironmentImpl::getImpl("HOME"s);
	}
	else
	{
		struct passwd* pwd = getpwuid(getuid());
		if (pwd)
		{
			path = pwd->pw_dir;
		}
		else
		{
			pwd = getpwuid(geteuid());
			if (pwd)
				path = pwd->pw_dir;
			else
				path = "/";
		}
	}
	std::string::size_type n = path.size();
	if (n > 0 && path[n - 1] != '/') path.append("/");
	return path;
#endif
}


std::string PathImpl::configHomeImpl()
{
#if defined(POCO_VXWORKS)
	return PathImpl::homeImpl();
#elif POCO_OS == POCO_OS_MAC_OS_X
	std::string path = PathImpl::homeImpl();
	std::string::size_type n = path.size();
	if (n > 0 && path[n - 1] == '/')
		path.append("Library/Preferences/");
	return path;
#else
	std::string path;
	if (EnvironmentImpl::hasImpl("XDG_CONFIG_HOME"s))
		path = EnvironmentImpl::getImpl("XDG_CONFIG_HOME"s);
	if (!path.empty())
		return path;

	path = PathImpl::homeImpl();
	std::string::size_type n = path.size();
	if (n > 0 && path[n - 1] == '/')
		path.append(".config/");

	return path;
#endif
}


std::string PathImpl::dataHomeImpl()
{
#if defined(POCO_VXWORKS)
	return PathImpl::homeImpl();
#elif POCO_OS == POCO_OS_MAC_OS_X
	std::string path = PathImpl::homeImpl();
	std::string::size_type n = path.size();
	if (n > 0 && path[n - 1] == '/')
		path.append("Library/Application Support/");
	return path;
#else
	std::string path;
	if (EnvironmentImpl::hasImpl("XDG_DATA_HOME"s))
		path = EnvironmentImpl::getImpl("XDG_DATA_HOME"s);
	if (!path.empty())
		return path;

	path = PathImpl::homeImpl();
	std::string::size_type n = path.size();
	if (n > 0 && path[n - 1] == '/')
		path.append(".local/share/");

	return path;
#endif
}


std::string PathImpl::cacheHomeImpl()
{
#if defined(POCO_VXWORKS)
	return PathImpl::tempImpl();
#elif POCO_OS == POCO_OS_MAC_OS_X
	std::string path = PathImpl::homeImpl();
	std::string::size_type n = path.size();
	if (n > 0 && path[n - 1] == '/')
		path.append("Library/Caches/");
	return path;
#else
	std::string path;
	if (EnvironmentImpl::hasImpl("XDG_CACHE_HOME"s))
		path = EnvironmentImpl::getImpl("XDG_CACHE_HOME"s);
	if (!path.empty())
		return path;

	path = PathImpl::homeImpl();
	std::string::size_type n = path.size();
	if (n > 0 && path[n - 1] == '/')
		path.append(".cache/");

	return path;
#endif
}


std::string PathImpl::tempHomeImpl()
{
#if defined(POCO_VXWORKS)
	return PathImpl::tempImpl();
#else
	std::string path = PathImpl::homeImpl();
	std::string::size_type n = path.size();
	if (n > 0 && path[n - 1] == '/')
#if POCO_OS == POCO_OS_MAC_OS_X
	  path.append("Library/Caches/");
#else
	  path.append(".local/tmp/");
#endif

	return path;
#endif
}


std::string PathImpl::tempImpl()
{
	std::string path;
	char* tmp = getenv("TMPDIR");
	if (tmp)
	{
		path = tmp;
		std::string::size_type n = path.size();
		if (n > 0 && path[n - 1] != '/') path.append("/");
	}
	else
	{
		path = "/tmp/";
	}
	return path;
}


std::string PathImpl::configImpl()
{
	std::string path;

#if POCO_OS == POCO_OS_MAC_OS_X
	  path = "/Library/Preferences/";
#else
	  path = "/etc/";
#endif
	return path;
}


std::string PathImpl::nullImpl()
{
#if defined(POCO_VXWORKS)
	return "/null";
#else
	return "/dev/null";
#endif
}


std::string PathImpl::expandImpl(const std::string& path)
{
	std::string result;
	std::string::const_iterator it  = path.begin();
	std::string::const_iterator end = path.end();
	if (it != end && *it == '~')
	{
		++it;
		if (it != end && *it == '/')
		{
			const char* homeEnv = getenv("HOME");
			if (homeEnv)
			{
				result += homeEnv;
				std::string::size_type resultSize = result.size();
				if (resultSize > 0 && result[resultSize - 1] != '/')
					result.append("/");
			}
			else
			{
				result += homeImpl();
			}
			++it;
		}
		else result += '~';
	}
	while (it != end)
	{
		if (*it == '\\')
		{
			++it;
			if (*it == '$')
			{
				result += *it++;
			}
		}
		else if (*it == '$')
		{
			std::string var;
			++it;
			if (it != end && *it == '{')
			{
				++it;
				while (it != end && *it != '}') var += *it++;
				if (it != end) ++it;
			}
			else
			{
				while (it != end && (Ascii::isAlphaNumeric(*it) || *it == '_')) var += *it++;
			}
			char* val = getenv(var.c_str());
			if (val) result += val;
		}
		else result += *it++;
	}
	std::string::size_type found = result.find("//");
	while (found != std::string::npos)
	{
		result.replace(found, 2, "/");
		found = result.find("//", found+1);
	}
	return result;
}


void PathImpl::listRootsImpl(std::vector<std::string>& roots)
{
	roots.clear();
	roots.push_back("/");
}


} // namespace Poco
