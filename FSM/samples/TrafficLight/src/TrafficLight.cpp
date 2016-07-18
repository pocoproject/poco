#include <sstream>
#include <fstream>
#include <iostream>
#include "Poco/DateTime.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/Util/LoggingConfigurator.h"
#include "Poco/Util/PropertyFileConfiguration.h"
#include "Poco/Exception.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/Util/Application.h"
#include "Poco/Timer.h"
#include "Poco/Thread.h"
#include "Poco/Stopwatch.h"

using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;
using Poco::Util::LoggingConfigurator;
using Poco::Util::PropertyFileConfiguration;
using Poco::AutoPtr;
using Poco::TimerCallback;
using Poco::Stopwatch;
using Poco::Thread;

using namespace std;

#include "stoplight.h"
using namespace cpp_ex4;


class TrafficLight : public Application {
	/// The main application class.
	///
	/// This class handles command-line arguments and configuration files.
	/// Start the TrafficLight executable with the help
	/// option (/help on Windows, --help on Unix) for
	/// the available command line options.
	///
  protected:
	void initialize(Application& self) {
		loadConfiguration(); // load default configuration files, if present
		Application::initialize(self);
	}

	void uninitialize() {
		Application::uninitialize();
	}

	void defineOptions(OptionSet& options) {
		Application::defineOptions(options);

		options.addOption(
			Option("help", "h", "display help information on command line arguments")
			.required(false)
			.repeatable(false));
	}

	void handleOption(const std::string& name, const std::string& value) {
		Application::handleOption(name, value);

		if (name == "help")
			help = true;
	}

	void displayHelp() {
		HelpFormatter helpFormatter(options());
		helpFormatter.setCommand(commandName());
		helpFormatter.setUsage("OPTIONS");
		helpFormatter.setHeader("A US TrafficLight as a FSM.");
		helpFormatter.format(std::cout);
	}

	int main(const std::vector<std::string>& args) {
		if (help) {
			displayHelp();
		} else {
			Stoplight stoplight(NORTH_SOUTH, NSGreenTimer, EWGreenTimer);
			stoplight.start(YellowTimer);
		}
		return Application::EXIT_OK;
	}
  private:
	int KeepGoing = 1;

	int YellowTimer = 2;    // Yellow lights last 2 seconds.
	int NSGreenTimer = 8;    // North-south green lasts 8 seconds.
	int EWGreenTimer = 5;    // East-west green lasts 5 seconds.
	bool help = false;
};

POCO_APP_MAIN(TrafficLight)
