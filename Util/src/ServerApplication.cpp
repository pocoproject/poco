//
// ServerApplication.cpp
//
// Library: Util
// Package: Application
// Module:  ServerApplication
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/OptionException.h"
#include "Poco/TemporaryFile.h"
#include "Poco/FileStream.h"
#include "Poco/Exception.h"
#if !defined(POCO_VXWORKS)
#include "Poco/Process.h"
#include "Poco/NamedEvent.h"
#endif
#include "Poco/NumberFormatter.h"
#include "Poco/Logger.h"
#include "Poco/String.h"
#if defined(POCO_OS_FAMILY_UNIX) && !defined(POCO_VXWORKS)
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <sys/stat.h>
#include <fstream>
#elif defined(POCO_OS_FAMILY_WINDOWS)
#include "Poco/Util/WinService.h"
#include "Poco/Util/WinRegistryKey.h"
#include "Poco/UnWindows.h"
#include <cstring>
#endif
#include "Poco/UnicodeConverter.h"
#include "Poco/Format.h"


using Poco::NumberFormatter;
using Poco::Exception;
using Poco::SystemException;
using namespace std::string_literals;


namespace Poco {
namespace Util {


#if defined(POCO_OS_FAMILY_WINDOWS)
Poco::NamedEvent      ServerApplication::_terminate(Poco::ProcessImpl::terminationEventName(Poco::Process::id()));
Poco::Event           ServerApplication::_terminated;
SERVICE_STATUS        ServerApplication::_serviceStatus;
SERVICE_STATUS_HANDLE ServerApplication::_serviceStatusHandle = 0;
#endif
#if defined(POCO_VXWORKS) || POCO_OS == POCO_OS_ANDROID
Poco::Event ServerApplication::_terminate;
#endif


ServerApplication::ServerApplication()
{
#if defined(POCO_OS_FAMILY_WINDOWS)
	_action = SRV_RUN;
	std::memset(&_serviceStatus, 0, sizeof(_serviceStatus));
#endif
}


ServerApplication::~ServerApplication()
{
}


bool ServerApplication::isInteractive() const
{
	bool runsInBackground = config().getBool("application.runAsDaemon"s, false) || config().getBool("application.runAsService"s, false);
	return !runsInBackground;
}


int ServerApplication::run()
{
	return Application::run();
}


void ServerApplication::terminate()
{
#if defined(POCO_OS_FAMILY_WINDOWS)
	_terminate.set();
#elif defined(POCO_VXWORKS) || POCO_OS == POCO_OS_ANDROID
	_terminate.set();
#else
	Poco::Process::requestTermination(Process::id());
#endif
}


#if defined(POCO_OS_FAMILY_WINDOWS)


//
// Windows specific code
//
BOOL ServerApplication::ConsoleCtrlHandler(DWORD ctrlType)
{
	switch (ctrlType)
	{
	case CTRL_C_EVENT:
	case CTRL_CLOSE_EVENT:
	case CTRL_BREAK_EVENT:
		terminate();
		return _terminated.tryWait(10000) ? TRUE : FALSE;
	default:
		return FALSE;
	}
}


void ServerApplication::ServiceControlHandler(DWORD control)
{
	switch (control)
	{
	case SERVICE_CONTROL_STOP:
	case SERVICE_CONTROL_SHUTDOWN:
		terminate();
		_serviceStatus.dwCurrentState = SERVICE_STOP_PENDING;
		break;
	case SERVICE_CONTROL_INTERROGATE:
		break;
	}
	SetServiceStatus(_serviceStatusHandle,  &_serviceStatus);
}


void ServerApplication::ServiceMain(DWORD argc, LPWSTR* argv)
{
	ServerApplication& app = static_cast<ServerApplication&>(Application::instance());

	app.config().setBool("application.runAsService"s, true);

	_serviceStatusHandle = RegisterServiceCtrlHandlerW(L"", ServiceControlHandler);
	if (!_serviceStatusHandle)
		throw SystemException("cannot register service control handler");

	_serviceStatus.dwServiceType             = SERVICE_WIN32;
	_serviceStatus.dwCurrentState            = SERVICE_START_PENDING;
	_serviceStatus.dwControlsAccepted        = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
	_serviceStatus.dwWin32ExitCode           = 0;
	_serviceStatus.dwServiceSpecificExitCode = 0;
	_serviceStatus.dwCheckPoint              = 0;
	_serviceStatus.dwWaitHint                = 0;
	SetServiceStatus(_serviceStatusHandle, &_serviceStatus);

	try
	{
		std::vector<std::string> args;
		for (DWORD i = 0; i < argc; ++i)
		{
			std::string arg;
			Poco::UnicodeConverter::toUTF8(argv[i], arg);
			args.push_back(arg);
		}
		app.init(args);
		_serviceStatus.dwCurrentState = SERVICE_RUNNING;
		SetServiceStatus(_serviceStatusHandle, &_serviceStatus);
		int rc = app.run();
		_serviceStatus.dwWin32ExitCode           = rc ? ERROR_SERVICE_SPECIFIC_ERROR : 0;
		_serviceStatus.dwServiceSpecificExitCode = rc;
	}
	catch (Exception& exc)
	{
		app.logger().log(exc);
		_serviceStatus.dwWin32ExitCode           = ERROR_SERVICE_SPECIFIC_ERROR;
		_serviceStatus.dwServiceSpecificExitCode = EXIT_CONFIG;
	}
	catch (...)
	{
		app.logger().error("fatal error - aborting"s);
		_serviceStatus.dwWin32ExitCode           = ERROR_SERVICE_SPECIFIC_ERROR;
		_serviceStatus.dwServiceSpecificExitCode = EXIT_SOFTWARE;
	}
	_serviceStatus.dwCurrentState = SERVICE_STOPPED;
	SetServiceStatus(_serviceStatusHandle, &_serviceStatus);
}


void ServerApplication::waitForTerminationRequest()
{
	SetConsoleCtrlHandler(ConsoleCtrlHandler, TRUE);
	_terminate.wait();
	_terminated.set();
}


int ServerApplication::run(int argc, char** argv)
{
	if (!hasConsole() && isService())
	{
		return 0;
	}
	else
	{
		int rc = EXIT_OK;
		try
		{
			init(argc, argv);
			switch (_action)
			{
			case SRV_REGISTER:
				registerService();
				rc = EXIT_OK;
				break;
			case SRV_UNREGISTER:
				unregisterService();
				rc = EXIT_OK;
				break;
			default:
				rc = run();
			}
		}
		catch (Exception& exc)
		{
			logger().log(exc);
			rc = EXIT_SOFTWARE;
		}
		return rc;
	}
}


int ServerApplication::run(const std::vector<std::string>& args)
{
	if (!hasConsole() && isService())
	{
		return 0;
	}
	else
	{
		int rc = EXIT_OK;
		try
		{
			init(args);
			switch (_action)
			{
			case SRV_REGISTER:
				registerService();
				rc = EXIT_OK;
				break;
			case SRV_UNREGISTER:
				unregisterService();
				rc = EXIT_OK;
				break;
			default:
				rc = run();
			}
		}
		catch (Exception& exc)
		{
			logger().log(exc);
			rc = EXIT_SOFTWARE;
		}
		return rc;
	}
}


int ServerApplication::run(int argc, wchar_t** argv)
{
	if (!hasConsole() && isService())
	{
		return 0;
	}
	else
	{
		int rc = EXIT_OK;
		try
		{
			init(argc, argv);
			switch (_action)
			{
			case SRV_REGISTER:
				registerService();
				rc = EXIT_OK;
				break;
			case SRV_UNREGISTER:
				unregisterService();
				rc = EXIT_OK;
				break;
			default:
				rc = run();
			}
		}
		catch (Exception& exc)
		{
			logger().log(exc);
			rc = EXIT_SOFTWARE;
		}
		return rc;
	}
}


bool ServerApplication::isService()
{
	SERVICE_TABLE_ENTRYW svcDispatchTable[2];
	svcDispatchTable[0].lpServiceName = L"";
	svcDispatchTable[0].lpServiceProc = ServiceMain;
	svcDispatchTable[1].lpServiceName = NULL;
	svcDispatchTable[1].lpServiceProc = NULL;
	return StartServiceCtrlDispatcherW(svcDispatchTable) != 0;
}


bool ServerApplication::hasConsole()
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	return hStdOut != INVALID_HANDLE_VALUE && hStdOut != NULL;
}


void ServerApplication::registerService()
{
	std::string name = config().getString("application.baseName"s);
	std::string path = Poco::format("\"%s\""s, config().getString("application.path"s));

	WinService service(name);
	if (_displayName.empty())
		service.registerService(path);
	else
		service.registerService(path, _displayName);
	if (_startup == "auto")
		service.setStartup(WinService::SVC_AUTO_START);
	else if (_startup == "manual")
		service.setStartup(WinService::SVC_MANUAL_START);
	if (!_description.empty())
		service.setDescription(_description);
	logger().information("The application has been successfully registered as a service.");
}


void ServerApplication::unregisterService()
{
	std::string name = config().getString("application.baseName"s);

	WinService service(name);
	service.unregisterService();
	logger().information("The service has been successfully unregistered."s);
}


void ServerApplication::defineOptions(OptionSet& options)
{
	Application::defineOptions(options);

	options.addOption(
		Option("registerService"s, ""s, "Register the application as a service."s)
			.required(false)
			.repeatable(false)
			.callback(OptionCallback<ServerApplication>(this, &ServerApplication::handleRegisterService)));

	options.addOption(
		Option("unregisterService"s, ""s, "Unregister the application as a service."s)
			.required(false)
			.repeatable(false)
			.callback(OptionCallback<ServerApplication>(this, &ServerApplication::handleUnregisterService)));

	options.addOption(
		Option("displayName"s, ""s, "Specify a display name for the service (only with /registerService)."s)
			.required(false)
			.repeatable(false)
			.argument("name"s)
			.callback(OptionCallback<ServerApplication>(this, &ServerApplication::handleDisplayName)));

	options.addOption(
		Option("description"s, ""s, "Specify a description for the service (only with /registerService)."s)
			.required(false)
			.repeatable(false)
			.argument("text"s)
			.callback(OptionCallback<ServerApplication>(this, &ServerApplication::handleDescription)));

	options.addOption(
		Option("startup"s, ""s, "Specify the startup mode for the service (only with /registerService)."s)
			.required(false)
			.repeatable(false)
			.argument("automatic|manual"s)
			.callback(OptionCallback<ServerApplication>(this, &ServerApplication::handleStartup)));
	
	options.addOption(
		Option("pidfile"s, ""s, "Write the process ID of the application to given file."s)
			.required(false)
			.repeatable(false)
			.argument("path")
			.callback(OptionCallback<ServerApplication>(this, &ServerApplication::handlePidFile)));
}


void ServerApplication::handleRegisterService(const std::string& name, const std::string& value)
{
	_action = SRV_REGISTER;
}


void ServerApplication::handleUnregisterService(const std::string& name, const std::string& value)
{
	_action = SRV_UNREGISTER;
}


void ServerApplication::handleDisplayName(const std::string& name, const std::string& value)
{
	_displayName = value;
}


void ServerApplication::handleDescription(const std::string& name, const std::string& value)
{
	_description = value;
}


void ServerApplication::handleStartup(const std::string& name, const std::string& value)
{
	if (Poco::icompare(value, 4, "auto"s) == 0)
		_startup = "auto";
	else if (Poco::icompare(value, "manual"s) == 0)
		_startup = "manual";
	else
		throw InvalidArgumentException("argument to startup option must be 'auto[matic]' or 'manual'");
}


#elif defined(POCO_VXWORKS)
//
// VxWorks specific code
//
void ServerApplication::waitForTerminationRequest()
{
	_terminate.wait();
}


int ServerApplication::run(int argc, char** argv)
{
	try
	{
		init(argc, argv);
	}
	catch (Exception& exc)
	{
		logger().log(exc);
		return EXIT_CONFIG;
	}
	return run();
}


int ServerApplication::run(const std::vector<std::string>& args)
{
	try
	{
		init(args);
	}
	catch (Exception& exc)
	{
		logger().log(exc);
		return EXIT_CONFIG;
	}
	return run();
}


void ServerApplication::defineOptions(OptionSet& options)
{
	Application::defineOptions(options);
}


#elif defined(POCO_OS_FAMILY_UNIX)


//
// Unix specific code
//
void ServerApplication::waitForTerminationRequest()
{
#if POCO_OS != POCO_OS_ANDROID
	sigset_t sset;
	sigemptyset(&sset);
	if (!std::getenv("POCO_ENABLE_DEBUGGER"))
	{
		sigaddset(&sset, SIGINT);
	}
	sigaddset(&sset, SIGQUIT);
	sigaddset(&sset, SIGTERM);
	sigprocmask(SIG_BLOCK, &sset, NULL);
	int sig;
	sigwait(&sset, &sig);
#else // POCO_OS != POCO_OS_ANDROID
	_terminate.wait();
#endif
}


int ServerApplication::run(int argc, char** argv)
{
	bool runAsDaemon = isDaemon(argc, argv);
	if (runAsDaemon)
	{
		beDaemon();
	}
	try
	{
		init(argc, argv);
		if (runAsDaemon)
		{
			int rc = chdir("/");
			if (rc != 0) return EXIT_OSERR;
		}
	}
	catch (Exception& exc)
	{
		logger().log(exc);
		return EXIT_CONFIG;
	}
	return run();
}


int ServerApplication::run(const std::vector<std::string>& args)
{
	bool runAsDaemon = false;
	for (const auto& arg: args)
	{
		if (arg == "--daemon")
		{
			runAsDaemon = true;
			break;
		}
	}
	if (runAsDaemon)
	{
		beDaemon();
	}
	try
	{
		init(args);
		if (runAsDaemon)
		{
			int rc = chdir("/");
			if (rc != 0) return EXIT_OSERR;
		}
	}
	catch (Exception& exc)
	{
		logger().log(exc);
		return EXIT_CONFIG;
	}
	return run();
}


bool ServerApplication::isDaemon(int argc, char** argv)
{
	std::string option("--daemon"s);
	for (int i = 1; i < argc; ++i)
	{
		if (option == argv[i])
			return true;
	}
	return false;
}


void ServerApplication::beDaemon()
{
#if !defined(POCO_NO_FORK_EXEC)
	pid_t pid;
	if ((pid = fork()) < 0)
		throw SystemException("cannot fork daemon process");
	else if (pid != 0)
		exit(0);

	setsid();
	umask(027);

	// attach stdin, stdout, stderr to /dev/null
	// instead of just closing them. This avoids
	// issues with third party/legacy code writing
	// stuff to stdout/stderr.
	FILE* fin  = freopen("/dev/null", "r+", stdin);
	if (!fin) throw Poco::OpenFileException("Cannot attach stdin to /dev/null");
	FILE* fout = freopen("/dev/null", "r+", stdout);
	if (!fout) throw Poco::OpenFileException("Cannot attach stdout to /dev/null");
	FILE* ferr = freopen("/dev/null", "r+", stderr);
	if (!ferr) throw Poco::OpenFileException("Cannot attach stderr to /dev/null");
#else
	throw Poco::NotImplementedException("platform does not allow fork/exec");
#endif
}


void ServerApplication::defineOptions(OptionSet& options)
{
	Application::defineOptions(options);

	options.addOption(
		Option("daemon"s, ""s, "Run application as a daemon."s)
			.required(false)
			.repeatable(false)
			.callback(OptionCallback<ServerApplication>(this, &ServerApplication::handleDaemon)));

	options.addOption(
		Option("umask"s, ""s, "Set the daemon's umask (octal, e.g. 027)."s)
			.required(false)
			.repeatable(false)
			.argument("mask")
			.callback(OptionCallback<ServerApplication>(this, &ServerApplication::handleUMask)));

	options.addOption(
		Option("pidfile"s, ""s, "Write the process ID of the application to given file."s)
			.required(false)
			.repeatable(false)
			.argument("path")
			.callback(OptionCallback<ServerApplication>(this, &ServerApplication::handlePidFile)));
}


void ServerApplication::handleDaemon(const std::string& name, const std::string& value)
{
	config().setBool("application.runAsDaemon"s, true);
}


void ServerApplication::handleUMask(const std::string& name, const std::string& value)
{
	int mask = 0;
	for (const auto ch: value)
	{
		mask *= 8;
		if (ch >= '0' && ch <= '7')
			mask += ch - '0';
		else
			throw Poco::InvalidArgumentException("umask contains non-octal characters", value);
	}
	umask(mask);
}


#endif


void ServerApplication::handlePidFile(const std::string& name, const std::string& value)
{
	Poco::FileOutputStream ostr(value);
	if (ostr.good())
		ostr << Poco::Process::id() << std::endl;
	else
		throw Poco::CreateFileException("Cannot write PID to file", value);
	Poco::TemporaryFile::registerForDeletion(value);
}


} } // namespace Poco::Util
