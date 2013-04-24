//
// Client.cpp
//
// $Id: //poco/1.4/Shell/Client/src/Client.cpp#4 $
//
// The client application for the Shell.
//
// Copyright (c) 2009-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "Poco/Util/Application.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/IntValidator.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/DialogSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/FileStream.h"
#include "Poco/String.h"
#include "Poco/NumberParser.h"
#include "Poco/Shell/Connection.h"
#include <iostream>
#include <cctype>
#if defined(POCO_OS_FAMILY_WINDOWS)
#include <windows.h>
#elif defined(POCO_OS_FAMILY_UNIX)
#include <termios.h>
#endif
#if defined(POCOSH_HAVE_EDITLINE)
#include <histedit.h>
#include <cstdio>
#endif


using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;
using Poco::Util::AbstractConfiguration;
using Poco::Util::OptionCallback;
using Poco::Shell::Connection;


class ShellClientApp: public Application
{
public:	
	ShellClientApp():
		_showHelp(false),
		_host(SERVER_HOST),
		_port(SERVER_PORT),
		_prompt(true),
		_auth(true),
		_pFileStream(0)
	{
	}
	
	~ShellClientApp()
	{
		delete _pFileStream;
	}

protected:
	static const Poco::UInt16 SERVER_PORT = 22023;
	static const std::string SERVER_HOST;
	static const std::string PROMPT;

	void defineOptions(OptionSet& options)
	{
		Application::defineOptions(options);

		options.addOption(
			Option("help", "h", "Display help information on command line arguments.")
				.required(false)
				.repeatable(false)
				.callback(OptionCallback<ShellClientApp>(this, &ShellClientApp::handleHelp)));

		options.addOption(
			Option("username", "u", "Specify the user name for logging in to the POCO Shell server.")
				.required(false)
				.repeatable(false)
				.argument("<username>")
				.callback(OptionCallback<ShellClientApp>(this, &ShellClientApp::handleUserName)));

		options.addOption(
			Option("password", "p", "Specify the password for logging in to the POCO Shell server.")
				.required(false)
				.repeatable(false)
				.argument("<password>")
				.callback(OptionCallback<ShellClientApp>(this, &ShellClientApp::handlePassword)));

		options.addOption(
			Option("host", "H", "Specify the POCO Shell server hostname or IP address.")
				.required(false)
				.repeatable(false)
				.argument("<host>")
				.callback(OptionCallback<ShellClientApp>(this, &ShellClientApp::handleHost)));

		options.addOption(
			Option("port", "P", "Specify the POCO Shell server port number.")
				.required(false)
				.repeatable(false)
				.argument("<port>")
				.validator(new Poco::Util::IntValidator(1, 65535))
				.callback(OptionCallback<ShellClientApp>(this, &ShellClientApp::handlePort)));

		options.addOption(
			Option("file", "f", "Read commands from the given file.")
				.required(false)
				.repeatable(false)
				.argument("<path>")
				.callback(OptionCallback<ShellClientApp>(this, &ShellClientApp::handleFile)));
				
		options.addOption(
			Option("noprompt", "n", "Do not display prompt and welcome message at startup.")
				.required(false)
				.repeatable(false)
				.callback(OptionCallback<ShellClientApp>(this, &ShellClientApp::handleNoPrompt)));

		options.addOption(
			Option("noauth", "N", "Do not authenticate against the server.")
				.required(false)
				.repeatable(false)
				.callback(OptionCallback<ShellClientApp>(this, &ShellClientApp::handleNoAuth)));
	}
	
	void handleHelp(const std::string& name, const std::string& value)
	{
		_showHelp = true;
		displayHelp();
		stopOptionsProcessing();
	}

	void handleUserName(const std::string& name, const std::string& value)
	{
		_userName = value;
	}

	void handlePassword(const std::string& name, const std::string& value)
	{
		_password = value;
	}
	
	void handleHost(const std::string& name, const std::string& value)
	{
		_host = value;
	}

	void handlePort(const std::string& name, const std::string& value)
	{
		_port = Poco::NumberParser::parse(value);
	}

	void handleFile(const std::string& name, const std::string& value)
	{
		_pFileStream = new Poco::FileInputStream(value);
		_prompt = false;
	}
	
	void handleNoPrompt(const std::string& name, const std::string& value)
	{
		_prompt = false;
	}


	void handleNoAuth(const std::string& name, const std::string& value)
	{
		_auth = false;
	}

	void displayHelp()
	{
		HelpFormatter helpFormatter(options());
		helpFormatter.setCommand(commandName());
		helpFormatter.setUsage("[<option> ...]");
		helpFormatter.setHeader(
			"\n"
			"POCO Shell Client.\n"
			"Copyright (c) 2009-2013 by Applied Informatics Software Engineering GmbH "
			"and POCO project contributors.\n"
			"All rights reserved.\n\n"
			"This program allows you to interact with a POCO Shell.\n\n"
			"The following command line options are supported:"
		);
		helpFormatter.setFooter(
			"For more information, please see the Open Service Platform "
			"documentation at <http://pocoproject.org/documentation>."
		);
		helpFormatter.setIndent(8);
		helpFormatter.format(std::cout);
	}
	
	void writeResponse(const std::string& response, std::ostream& ostr)
	{
		std::string::const_iterator it(response.begin());
		std::string::const_iterator end(response.end());
		while (it != end)
		{
			while (it != end && std::isdigit(*it)) ++it;
			if (it != end && (*it == ' ' || *it == '-')) ++it;
			while (it != end && *it != '\n') ostr << *it++;
			while (it != end && std::isspace(*it)) ++it;
			ostr << std::endl;
		}
	}
	
	void writeResponse(int status, const std::string& response, std::ostream& ostr)
	{
		switch (status)
		{
		case Connection::STATUS_CLIENT_ERROR:
		case Connection::STATUS_CLIENT_USAGE:
			break;
		case Connection::STATUS_CLIENT_DENIED:
			ostr << "Not authorized." << std::endl;
			break;
		default:
			writeResponse(response, ostr);
			break;
		}
	}
	
	int sendRequest(const std::string& request, std::ostream& out, std::ostream& err)
	{
		_socket.sendMessage(request);
		std::string response;
		int status = _socket.receiveStatusMessage(response);
		while (status == Connection::STATUS_INTERMEDIATE)
		{
			writeResponse(response, out);
			status = _socket.receiveStatusMessage(response);
		}
		if (status >= Connection::STATUS_CLIENT_ERROR)
		{
			writeResponse(status, response, err);
		}
		return status;
	}

	void promptLogin()
	{
		if (_userName.empty())
		{
			std::cout << "Username: " << std::flush;
			std::getline(std::cin, _userName);
		}
		if (_password.empty())
		{
			std::cout << "Password: " << std::flush;
			echo(false);
			std::getline(std::cin, _password);
			echo(true);
			std::cout << std::endl;
		}
	}
	
	void echo(bool status)
	{
#if defined(POCO_OS_FAMILY_WINDOWS)
		HANDLE stdIn = GetStdHandle(STD_INPUT_HANDLE);
		DWORD mode;
		GetConsoleMode(stdIn, &mode);
		mode = status ? mode | ENABLE_ECHO_INPUT : mode & ~ENABLE_ECHO_INPUT;
		SetConsoleMode(stdIn, mode);
#elif defined(POCO_OS_FAMILY_UNIX)
		struct termios tio;
		tcgetattr(0, &tio);
		tio.c_lflag = status ? tio.c_lflag | ECHO : tio.c_lflag & ~(ECHO);
		tcsetattr(0, TCSANOW, &tio);
#endif
	}

#if defined(POCOSH_HAVE_EDITLINE)
	static const char* prompt(EditLine*)
	{
		return PROMPT.c_str();
	}
#endif
	
	int main(const std::vector<std::string>& args)
	{
		if (!_showHelp)
		{
			if (_prompt && _auth) promptLogin();

			Poco::Net::SocketAddress sa(_host, _port);
			_socket.connect(sa);
			
			std::string greeting;
			_socket.receiveStatusMessage(greeting);
			if (_prompt) 
			{
				std::cout << "\n";
				writeResponse(greeting, std::cout);
				std::cout << std::endl;
			}
			
			int status(Connection::STATUS_OK);
			if (_auth)
			{
				status = sendRequest("login \"" + _userName + "\" \"" + _password + "\"", std::cout, std::cerr);
			}
			if (status == Connection::STATUS_OK)
			{
				std::istream* pStream = _pFileStream;
				if (!pStream) pStream = &std::cin;
				bool useEditLine = false;

#if defined(POCOSH_HAVE_EDITLINE)
				EditLine* editLine = NULL;
				History* elHist = NULL;
				HistEvent elHistEvent;
				if (!_pFileStream)
				{
					editLine = el_init("pocosh", stdin, stdout, stderr);
					if (!editLine) throw Poco::IOException("Failed to initialize editline");
					el_set(editLine, EL_PROMPT, &ShellClientApp::prompt);
					el_set(editLine, EL_EDITOR, "emacs");
					el_set(editLine, EL_SIGNAL, 1);
					elHist = history_init();
					if (elHist)
					{
						history(elHist, &elHistEvent, H_SETSIZE, 64);
						el_set(editLine, EL_HIST, history, elHist);
					}
					useEditLine = true;
				}
#else			
				if (_prompt) std::cout << PROMPT << std::flush;
#endif

#if defined(POCOSH_HAVE_EDITLINE)
				if (useEditLine)
				{
					int count;
					const char* line;
					while ((line = el_gets(editLine, &count)) != NULL)
					{
						std::string request(line);
						Poco::trimRightInPlace(request);
						int status = sendRequest(request, std::cout, std::cerr);
						if (status == Connection::STATUS_GOODBYE) break;
						if (elHist && !request.empty()) history(elHist, &elHistEvent, H_ENTER, line);
					}
					if (elHist) history_end(elHist);
					el_end(editLine);
					return Application::EXIT_OK;
				}
#endif			
				std::string request;
				while (!std::getline(*pStream, request).eof())
				{
					Poco::trimRightInPlace(request);
					int status = sendRequest(request, std::cout, std::cerr);
					if (status == Connection::STATUS_GOODBYE) break;
					if (_prompt) std::cout << PROMPT << std::flush;
				}
			}
		}
		return Application::EXIT_OK;
	}

private:
	bool         _showHelp;
	std::string  _userName;
	std::string  _password;
	std::string  _host;
	Poco::UInt16 _port;
	bool         _prompt;
	bool         _auth;
	Poco::FileInputStream* _pFileStream;
	Poco::Net::DialogSocket _socket;
};


const std::string ShellClientApp::SERVER_HOST("localhost");
const std::string ShellClientApp::PROMPT("poco> ");


POCO_APP_MAIN(ShellClientApp)
