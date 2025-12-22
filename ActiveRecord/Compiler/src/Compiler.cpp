//
// CompilerApp.cpp
//
// Copyright (c) 2020, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Util/Application.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/FileStream.h"
#include "Poco/Path.h"
#include "Poco/File.h"
#include "Parser.h"
#include "HeaderGenerator.h"
#include "ImplGenerator.h"
#include <iostream>
#include <sstream>


using namespace std::string_literals;


using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;
using Poco::Util::OptionCallback;


namespace Poco {
namespace ActiveRecord {
namespace Compiler {


class CompilerApp: public Application
{
public:
	CompilerApp()
	{
	}

protected:
	void initialize(Application& self)
	{
		loadConfiguration(); // load default configuration files, if present
		Application::initialize(self);
	}

	void defineOptions(OptionSet& options)
	{
		Application::defineOptions(options);

		options.addOption(
			Option("help", "h", "Display help information on command line arguments."s)
				.required(false)
				.repeatable(false)
				.callback(OptionCallback<CompilerApp>(this, &CompilerApp::handleHelp)));
	}

	void handleHelp(const std::string& name, const std::string& value)
	{
		_helpRequested = true;
		displayHelp();
		stopOptionsProcessing();
	}

	void displayHelp()
	{
		HelpFormatter helpFormatter(options());
		helpFormatter.setCommand(commandName());
		helpFormatter.setUsage("[<option> ...] <file>");
		helpFormatter.setHeader("POCO C++ Libraries ActiveRecord ORM Compiler");
		helpFormatter.setHeader(
			"\n"
			"The POCO C++ Libraries ActiveRecord ORM Compiler.\n"
			"Copyright (c) 2020-2023 by Applied Informatics Software Engineering GmbH.\n"
			"All rights reserved.\n\n"
			"This program generates C++ source code from an ActiveRecord "
			"XML definition. "
			"The following command line options are supported:"
		);
		helpFormatter.setFooter(
			"For more information, please see the POCO C++ Libraries "
			"documentation at <https://pocoproject.org/docs/>."
		);
		helpFormatter.setIndent(8);
		helpFormatter.format(std::cout);
	}

	void resolveReferences(const Class& clazz, const ClassMap& classes)
	{
		for (const auto& r: clazz.references)
		{
			auto it = classes.find(r);
			if (it == classes.end())
			{
				throw Poco::NotFoundException(Poco::format("class %s has a reference to unknown class %s"s, clazz.name, r));
			}
			else
			{
				const Class& refClass = it->second;
				for (const auto& rr: refClass.references)
				{
					if (rr == clazz.name)
					{
						throw Poco::CircularReferenceException(Poco::format("class %s has a circular reference to class %s"s, clazz.name, refClass.name));
					}
				}
			}
		}
	}

	void resolveForeignKeys(const Class& clazz, const ClassMap& classes)
	{
		for (const auto& p: clazz.properties)
		{
			if (!p.foreignKey.empty())
			{
				auto it = classes.find(p.referencedClass);
				if (it == classes.end())
				{
					throw Poco::NotFoundException(Poco::format("class %s has a reference to unknown class %s"s, clazz.name, p.referencedClass));
				}
				else
				{
					const Class& refClass = it->second;
					bool found = false;
					for (const auto& rp: refClass.properties)
					{
						if (rp.column == p.foreignKey)
						{
							found = true;
							break;
						}
					}
					if (!found) throw Poco::NotFoundException(Poco::format("class %s has a reference to unknown foreign key column %s in table %s"s, clazz.name, p.foreignKey, refClass.table));
				}
			}
		}
	}

	void resolve(const ClassMap& classes)
	{
		for (const auto& p: classes)
		{
			const Class& clazz = p.second;
			resolveReferences(clazz, classes);
			resolveForeignKeys(clazz, classes);
		}
	}

	void compile(const std::string& path)
	{
		Parser parser;
		Poco::FileInputStream xmlStream(path);
		ClassMap classes = parser.parse(path, xmlStream);
		resolve(classes);
		for (const auto& p: classes)
		{
			Poco::Path headerPath;
			headerPath.pushDirectory("include"s);
			auto ns = CodeGenerator::splitNameSpace(p.second.nameSpace);
			for (const auto& n: ns)
			{
				headerPath.pushDirectory(n);
			}

			Poco::File headerDir(headerPath.toString());
			headerDir.createDirectories();

			headerPath.setFileName(p.first);
			headerPath.setExtension("h");

			Poco::FileOutputStream headerStream(headerPath.toString());
			HeaderGenerator hg(path, headerStream, p.second, classes);
			hg.generate();
			headerStream.close();

			Poco::Path implPath;
			implPath.pushDirectory("src"s);

			Poco::File implDir(implPath.toString());
			implDir.createDirectories();

			implPath.setFileName(p.first);
			implPath.setExtension("cpp");

			Poco::FileOutputStream implStream(implPath.toString());
			ImplGenerator ig(path, implStream, p.second, classes);
			ig.generate();
			implStream.close();
		}
	}

	int main(const ArgVec& args)
	{
		if (!_helpRequested)
		{
			if (args.empty())
			{
				displayHelp();
			}
			else
			{
				try
				{
					for (const auto& a: args)
					{
						compile(a);
					}
				}
				catch (Poco::Exception& exc)
				{
					std::cout << exc.displayText() << std::endl;
				}
			}
		}
		return Application::EXIT_OK;
	}

private:
	bool _helpRequested = false;
};


} } } // namespace Poco::ActiveRecord::Compiler


POCO_APP_MAIN(Poco::ActiveRecord::Compiler::CompilerApp)
