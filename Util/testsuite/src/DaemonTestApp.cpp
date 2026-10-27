//
// DaemonTestApp.cpp
//
// Helper program for ServerApplicationTest: writes a line to stdout, registers an
// atexit handler and runs a ServerApplication that returns at once.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Util/ServerApplication.h"
#include "Poco/Exception.h"
#include "Poco/Process.h"
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <string>


using Poco::Util::Application;
using Poco::Util::ServerApplication;


namespace
{
	[[nodiscard]] std::string& markerPath()
		/// Returns the path of the file that writeMarker() appends to.
	{
		static std::string path;
		return path;
	}


	void writeMarker()
		/// Appends the ID of the process that runs the atexit handlers to the marker file.
	{
		std::ofstream ostr(markerPath(), std::ios::app);
		ostr << Poco::Process::id() << '\n';
	}


	class DaemonTestApp: public ServerApplication
		/// A ServerApplication whose main() returns at once.
	{
	public:
		DaemonTestApp() = default;
			/// Creates the DaemonTestApp.

		~DaemonTestApp() override = default;
			/// Destroys the DaemonTestApp.

	protected:
		int main(const ArgVec&) override;
			/// Returns EXIT_OK.
	};


	int DaemonTestApp::main(const ArgVec&)
	{
		return Application::EXIT_OK;
	}
}


int main(int argc, char** argv)
	/// Usage: DaemonTestApp [--daemon] <marker file>
	/// The marker file is given by an absolute path, because the daemon
	/// changes its working directory to the root directory.
{
	static constexpr int MIN_ARGC = 2;

	if (argc < MIN_ARGC) return Application::EXIT_USAGE;
	markerPath() = argv[argc - 1];
	if (std::atexit(writeMarker) != 0) return Application::EXIT_SOFTWARE;
	std::printf("before daemon\n");
	try
	{
		DaemonTestApp app;
		return app.run(argc, argv);
	}
	catch (Poco::Exception& exc)
	{
		std::cerr << exc.displayText() << '\n';
		return Application::EXIT_SOFTWARE;
	}
	catch (std::exception& exc)
	{
		std::cerr << exc.what() << '\n';
		return Application::EXIT_SOFTWARE;
	}
}
