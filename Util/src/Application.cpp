//
// Application.cpp
//
// Library: Util
// Package: Application
// Module:  Application
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Util/Application.h"
#include "Poco/Util/SystemConfiguration.h"
#include "Poco/Util/MapConfiguration.h"
#include "Poco/Util/PropertyFileConfiguration.h"
#include "Poco/Util/IniFileConfiguration.h"
#ifndef POCO_UTIL_NO_XMLCONFIGURATION
#include "Poco/Util/XMLConfiguration.h"
#endif
#ifndef POCO_UTIL_NO_JSONCONFIGURATION
#include "Poco/Util/JSONConfiguration.h"
#endif
#include "Poco/Util/LoggingSubsystem.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionProcessor.h"
#include "Poco/Util/Validator.h"
#include "Poco/Environment.h"
#include "Poco/Exception.h"
#include "Poco/NumberFormatter.h"
#include "Poco/File.h"
#include "Poco/Path.h"
#include "Poco/String.h"
#include "Poco/ConsoleChannel.h"
#include "Poco/AutoPtr.h"
#if defined(POCO_OS_FAMILY_WINDOWS)
#include "Poco/UnWindows.h"
#endif
#if defined(POCO_OS_FAMILY_UNIX) && !defined(POCO_VXWORKS)
#include "Poco/SignalHandler.h"
#include <stdio.h>
#include <sys/ioctl.h>
#if POCO_OS == POCO_OS_SOLARIS
#include <stropts.h>
#include <termios.h>
#endif
#endif
#include "Poco/UnicodeConverter.h"


using Poco::Logger;
using Poco::Path;
using Poco::File;
using Poco::Environment;
using Poco::SystemException;
using Poco::ConsoleChannel;
using Poco::NumberFormatter;
using Poco::AutoPtr;
using Poco::icompare;


using namespace std::string_literals;


namespace Poco {
namespace Util {


Application* Application::_pInstance = 0;


Application::Application():
	_pConfig(new LayeredConfiguration),
	_initialized(false),
	_unixOptions(true),
	_pLogger(&Logger::get("ApplicationStartup"s)),
	_stopOptionsProcessing(false)
{
	setup();
}


Application::Application(int argc, char* argv[]):
	_pConfig(new LayeredConfiguration),
	_initialized(false),
	_unixOptions(true),
	_pLogger(&Logger::get("ApplicationStartup"s)),
	_stopOptionsProcessing(false)
{
	setup();
	init(argc, argv);
}


Application::~Application()
{
	_pInstance = 0;
}


void Application::setup()
{
	poco_assert (_pInstance == 0);

	_pConfig->add(new SystemConfiguration, PRIO_SYSTEM, false);
	_pConfig->add(new MapConfiguration, PRIO_APPLICATION, true);

	addSubsystem(new LoggingSubsystem);

#if defined(POCO_OS_FAMILY_UNIX) && !defined(POCO_VXWORKS)
	_workingDirAtLaunch = Path::current();

	#if !defined(_DEBUG)
	Poco::SignalHandler::install();
	#endif
#else
	setUnixOptions(false);
#endif

	_pInstance = this;

	AutoPtr<ConsoleChannel> pCC = new ConsoleChannel;
	Logger::setChannel(""s, pCC);
}


void Application::addSubsystem(Subsystem* pSubsystem)
{
	poco_check_ptr (pSubsystem);

	_subsystems.push_back(pSubsystem);
}


void Application::init(int argc, char* argv[])
{
	setArgs(argc, argv);
	init();
}


#if defined(_WIN32)
void Application::init(int argc, wchar_t* argv[])
{
	std::vector<std::string> args;
	for (int i = 0; i < argc; ++i)
	{
		std::string arg;
		Poco::UnicodeConverter::toUTF8(argv[i], arg);
		args.push_back(arg);
	}
	init(args);
}
#endif


void Application::init(const ArgVec& args)
{
	setArgs(args);
	init();
}


void Application::init()
{
	Path appPath;
	getApplicationPath(appPath);
	_pConfig->setString("application.path"s, appPath.toString());
	_pConfig->setString("application.name"s, appPath.getFileName());
	_pConfig->setString("application.baseName"s, appPath.getBaseName());
	_pConfig->setString("application.dir"s, appPath.parent().toString());
	_pConfig->setString("application.configDir"s, Path::configHome() + appPath.getBaseName() + Path::separator());
	_pConfig->setString("application.cacheDir"s, Path::cacheHome() + appPath.getBaseName() + Path::separator());
	_pConfig->setString("application.tempDir"s, Path::tempHome() + appPath.getBaseName() + Path::separator());
	_pConfig->setString("application.dataDir"s, Path::dataHome() + appPath.getBaseName() + Path::separator());
	processOptions();
}


const char* Application::name() const
{
	return "Application";
}


void Application::initialize(Application& self)
{
	for (auto& pSub: _subsystems)
	{
		_pLogger->debug("Initializing subsystem: "s + pSub->name());
		pSub->initialize(self);
	}
	_initialized = true;
}


void Application::uninitialize()
{
	if (_initialized)
	{
		for (SubsystemVec::reverse_iterator it = _subsystems.rbegin(); it != _subsystems.rend(); ++it)
		{
			_pLogger->debug("Uninitializing subsystem: "s + (*it)->name());
			(*it)->uninitialize();
		}
		_initialized = false;
	}
}


void Application::reinitialize(Application& self)
{
	for (auto& pSub: _subsystems)
	{
		_pLogger->debug("Re-initializing subsystem: "s + pSub->name());
		pSub->reinitialize(self);
	}
}


void Application::setUnixOptions(bool flag)
{
	_unixOptions = flag;
}


int Application::loadConfiguration(int priority)
{
	int n = 0;
	Path appPath;
	getApplicationPath(appPath);
	Path confPath;
	if (findAppConfigFile(appPath.getBaseName(), "properties"s, confPath))
	{
		_pConfig->add(new PropertyFileConfiguration(confPath.toString()), priority, false);
		++n;
	}
#ifndef POCO_UTIL_NO_INIFILECONFIGURATION
	if (findAppConfigFile(appPath.getBaseName(), "ini"s, confPath))
	{
		_pConfig->add(new IniFileConfiguration(confPath.toString()), priority, false);
		++n;
	}
#endif
#ifndef POCO_UTIL_NO_JSONCONFIGURATION
	if (findAppConfigFile(appPath.getBaseName(), "json"s, confPath))
	{
		_pConfig->add(new JSONConfiguration(confPath.toString()), priority, false);
		++n;
	}
#endif
#ifndef POCO_UTIL_NO_XMLCONFIGURATION
	if (findAppConfigFile(appPath.getBaseName(), "xml"s, confPath))
	{
		_pConfig->add(new XMLConfiguration(confPath.toString()), priority, false);
		++n;
	}
#endif
	if (n > 0)
	{
		if (!confPath.isAbsolute())
			_pConfig->setString("application.configDir"s, confPath.absolute().parent().toString());
		else
			_pConfig->setString("application.configDir"s, confPath.parent().toString());
	}
	return n;
}


void Application::loadConfiguration(const std::string& path, int priority)
{
	int n = 0;
	Path confPath(path);
	std::string ext = confPath.getExtension();
	if (icompare(ext, "properties") == 0)
	{
		_pConfig->add(new PropertyFileConfiguration(confPath.toString()), priority, false);
		++n;
	}
#ifndef POCO_UTIL_NO_INIFILECONFIGURATION
	else if (icompare(ext, "ini") == 0)
	{
		_pConfig->add(new IniFileConfiguration(confPath.toString()), priority, false);
		++n;
	}
#endif
#ifndef POCO_UTIL_NO_JSONCONFIGURATION
	else if (icompare(ext, "json") == 0)
	{
		_pConfig->add(new JSONConfiguration(confPath.toString()), priority, false);
		++n;
	}
#endif
#ifndef POCO_UTIL_NO_XMLCONFIGURATION
	else if (icompare(ext, "xml") == 0)
	{
		_pConfig->add(new XMLConfiguration(confPath.toString()), priority, false);
		++n;
	}
#endif
	else throw Poco::InvalidArgumentException("Unsupported configuration file type", ext);

	if (n > 0 && !_pConfig->has("application.configDir"s))
	{
		if (!confPath.isAbsolute())
			_pConfig->setString("application.configDir"s, confPath.absolute().parent().toString());
		else
			_pConfig->setString("application.configDir"s, confPath.parent().toString());
	}
}


std::string Application::commandName() const
{
	return _pConfig->getString("application.baseName"s);
}


std::string Application::commandPath() const
{
	return _pConfig->getString("application.path"s);
}


void Application::stopOptionsProcessing()
{
	_stopOptionsProcessing = true;
}


Application::WindowSize Application::windowSize()
{
	WindowSize size{0, 0};

#if defined(POCO_OS_FAMILY_WINDOWS)
	CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
    {
    	size.width  = csbi.srWindow.Right  - csbi.srWindow.Left + 1;
    	size.height = csbi.srWindow.Bottom - csbi.srWindow.Top  + 1;
    }
#elif defined(POCO_OS_FAMILY_UNIX)
	struct winsize winsz;
    if (ioctl(0, TIOCGWINSZ , &winsz) != -1)
    {
    	size.width  = winsz.ws_col;
    	size.height = winsz.ws_row;
    }
#endif

	return size;
}


int Application::run()
{
	int rc = EXIT_CONFIG;

	try
	{
		initialize(*this);
		rc = EXIT_SOFTWARE;
		rc = main(_unprocessedArgs);
	}
	catch (Poco::Exception& exc)
	{
		logger().log(exc);
	}
	catch (std::exception& exc)
	{
		logger().error(exc.what());
	}
	catch (...)
	{
		logger().fatal("system exception"s);
	}

	uninitialize();
	return rc;
}


int Application::main(const ArgVec& args)
{
	return EXIT_OK;
}


void Application::setArgs(int argc, char* argv[])
{
	_command = argv[0];
	_pConfig->setInt("application.argc"s, argc);
	_unprocessedArgs.reserve(argc);
	std::string argvKey = "application.argv[";
	for (int i = 0; i < argc; ++i)
	{
		std::string arg(argv[i]);
		_pConfig->setString(argvKey + NumberFormatter::format(i) + "]", arg);
		_unprocessedArgs.push_back(arg);
	}
}


void Application::setArgs(const ArgVec& args)
{
	poco_assert (!args.empty());

	_command = args[0];
	_pConfig->setInt("application.argc"s, (int) args.size());
	_unprocessedArgs = args;
	std::string argvKey = "application.argv[";
	for (int i = 0; i < args.size(); ++i)
	{
		_pConfig->setString(argvKey + NumberFormatter::format(i) + "]", args[i]);
	}
}


void Application::processOptions()
{
	defineOptions(_options);
	OptionProcessor processor(_options);
	processor.setUnixStyle(_unixOptions);
	_argv = _unprocessedArgs;
	_unprocessedArgs.erase(_unprocessedArgs.begin());
	ArgVec::iterator it = _unprocessedArgs.begin();
	while (it != _unprocessedArgs.end() && !_stopOptionsProcessing)
	{
		std::string name;
		std::string value;
		if (processor.process(*it, name, value))
		{
			if (!name.empty()) // "--" option to end options processing or deferred argument
			{
				handleOption(name, value);
			}
			it = _unprocessedArgs.erase(it);
		}
		else ++it;
	}
	if (!_stopOptionsProcessing)
		processor.checkRequired();
}


void Application::getApplicationPath(Poco::Path& appPath) const
{
#if defined(POCO_OS_FAMILY_UNIX) && !defined(POCO_VXWORKS)
	if (_command.find('/') != std::string::npos)
	{
		Path path(_command);
		if (path.isAbsolute())
		{
			appPath = path;
		}
		else
		{
			appPath = _workingDirAtLaunch;
			appPath.append(path);
		}
	}
	else
	{
		if (!Path::find(Environment::get("PATH"s), _command, appPath))
			appPath = Path(_workingDirAtLaunch, _command);
		appPath.makeAbsolute();
	}
#elif defined(POCO_OS_FAMILY_WINDOWS)
		wchar_t path[1024];
		int n = GetModuleFileNameW(0, path, sizeof(path)/sizeof(wchar_t));
		if (n > 0)
		{
			std::string p;
			Poco::UnicodeConverter::toUTF8(path, p);
			appPath = p;
		}
		else throw SystemException("Cannot get application file name.");
#else
	appPath = _command;
#endif
}


bool Application::findFile(Poco::Path& path) const
{
	if (path.isAbsolute()) return true;

	Path appPath;
	getApplicationPath(appPath);
	Path base = appPath.parent();
	do
	{
		Path p(base, path);
		File f(p);
		if (f.exists())
		{
			path = p;
			return true;
		}
		if (base.depth() > 0) base.popDirectory();
	}
	while (base.depth() > 0);
	return false;
}


bool Application::findAppConfigFile(const std::string& appName, const std::string& extension, Path& path) const
{
	poco_assert (!appName.empty());

	Path p(appName);
	p.setExtension(extension);
	bool found = findFile(p);
	if (!found)
	{
#if defined(_DEBUG)
		if (appName[appName.length() - 1] == 'd')
		{
			p.setBaseName(appName.substr(0, appName.length() - 1));
			found = findFile(p);
		}
#endif
	}
	if (found)
		path = p;
	return found;
}


bool Application::findAppConfigFile(const Path& basePath, const std::string& appName, const std::string& extension, Path& path) const
{
	poco_assert (!appName.empty());

	Path p(basePath,appName);
	p.setExtension(extension);
	bool found = findFile(p);
	if (!found)
	{
#if defined(_DEBUG)
		if (appName[appName.length() - 1] == 'd')
		{
			p.setBaseName(appName.substr(0, appName.length() - 1));
			found = findFile(p);
		}
#endif
	}
	if (found)
		path = p;
	return found;
}


void Application::defineOptions(OptionSet& options)
{
	for (auto& pSub: _subsystems)
	{
		pSub->defineOptions(options);
	}
}


void Application::handleOption(const std::string& name, const std::string& value)
{
	const Option& option = _options.getOption(name);
	if (option.validator())
	{
		option.validator()->validate(option, value);
	}
	if (!option.binding().empty())
	{
		AbstractConfiguration* pConfig = option.config();
		if (!pConfig) pConfig = &config();
		pConfig->setString(option.binding(), value);
	}
	if (option.callback())
	{
		option.callback()->invoke(name, value);
	}
}


void Application::setLogger(Logger& logger)
{
	_pLogger = &logger;
}


} } // namespace Poco::Util
