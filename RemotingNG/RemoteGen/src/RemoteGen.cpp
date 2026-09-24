//
// RemoteGen.cpp
//
// Copyright (c) 2006-2024, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/StringTokenizer.h"
#include "Poco/Path.h"
#include "Poco/String.h"
#include "Poco/File.h"
#include "Poco/Process.h"
#include "Poco/Environment.h"
#include "Poco/NumberFormatter.h"
#include "Poco/Exception.h"
#include "Poco/NullStream.h"
#include "Poco/FileStream.h"
#include "Poco/Delegate.h"
#include "Poco/Format.h"
#include "Poco/XML/XMLWriter.h"
#include "Poco/Util/Application.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/Util/AbstractConfiguration.h"
#include "Poco/Util/OptionException.h"
#include "Poco/CppParser/Parser.h"
#include "Poco/CppParser/NameSpace.h"
#include "Poco/CppParser/BuiltIn.h"
#include "Poco/CppParser/Struct.h"
#include "Poco/CppParser/Utility.h"
#include "Poco/CppParser/Utility.h"
#include "Poco/CodeGeneration/Utility.h"
#include "Poco/CodeGeneration/GeneratorEngine.h"
#include "Poco/CodeGeneration/MethodPropertyFilter.h"
#include "InterfaceGenerator.h"
#include "RemoteObjectGenerator.h"
#include "RemoteBridgeGenerator.h"
#include "ProxyGenerator.h"
#include "BundleActivatorGenerator.h"
#include "ProxyFactoryGenerator.h"
#include "SkeletonGenerator.h"
#include "ClientHelperGenerator.h"
#include "ServerHelperGenerator.h"
#include "SerializerGenerator.h"
#include "DeserializerGenerator.h"
#include "XSDGenerator.h"
#include "WSDLGenerator.h"
#include "SwaggerGenerator.h"
#include "ExtensionsGenerator.h"
#include "EventDispatcherGenerator.h"
#include "EventSubscriberGenerator.h"
#include <set>
#include <fstream>
#include <iostream>


using Poco::StringTokenizer;
using Poco::Path;
using Poco::File;
using Poco::Process;
using Poco::ProcessHandle;
using Poco::Environment;
using Poco::NumberFormatter;
using Poco::Exception;
using Poco::format;
using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionCallback;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;
using Poco::Util::AbstractConfiguration;
using Poco::CppParser::NameSpace;
using Poco::CppParser::Function;
using Poco::CppParser::Struct;
using Poco::CppParser::Symbol;
using Poco::CodeGeneration::GeneratorEngine;
using Poco::CodeGeneration::MethodPropertyFilter;
using namespace std::string_literals;


class RemoteGenApp: public Application
{
public:
	RemoteGenApp():
	  _helpRequested(false),
	  _noGlobalConfig(false),
	  _enableOSP(false),
	  _enableBridge(false),
	  _enableBridgeSync(true),
	  _bundlePath("bundle"),
	  _enableTimestamps(true)
	{
		Poco::CppParser::NameSpace* pNS = new Poco::CppParser::NameSpace("Poco", Poco::CppParser::NameSpace::root());
		new Poco::CppParser::BuiltIn("Int8"s, pNS);
		new Poco::CppParser::BuiltIn("UInt8"s, pNS);
		new Poco::CppParser::BuiltIn("Int16"s, pNS);
		new Poco::CppParser::BuiltIn("UInt16"s, pNS);
		new Poco::CppParser::BuiltIn("Int32"s, pNS);
		new Poco::CppParser::BuiltIn("UInt32"s, pNS);
		new Poco::CppParser::BuiltIn("Int64"s, pNS);
		new Poco::CppParser::BuiltIn("UInt64"s, pNS);
		new Poco::CppParser::BuiltIn("IntPtr"s, pNS);
		new Poco::CppParser::BuiltIn("UIntPtr"s, pNS);
	}

	~RemoteGenApp()
	{
	}

protected:
	void initialize(Application& self)
	{
		if (!_noGlobalConfig)
		{
			loadConfiguration();
		}

		Application::initialize(self);
		AbstractGenerator::EError += Poco::Delegate<RemoteGenApp, const std::string>(this, &RemoteGenApp::onError);
	}

	void uninitialize()
	{
		AbstractGenerator::EError -= Poco::Delegate<RemoteGenApp, const std::string>(this, &RemoteGenApp::onError);
		Application::uninitialize();
	}

	void reinitialize(Application& self)
	{
		Application::reinitialize(self);
	}

	void defineOptions(OptionSet& options)
	{
		Application::defineOptions(options);

		options.addOption(
			Option("help"s, "h"s, "Display help information on command line arguments."s)
				.required(false)
				.repeatable(false)
				.callback(OptionCallback<RemoteGenApp>(this, &RemoteGenApp::handleHelp)));

		options.addOption(
			Option("define"s, "D"s,
				"Define a configuration property. A configuration property "
				"defined with this option can be referenced in the configuration "
				"file, using the following syntax: ${<name>}."s)
				.required(false)
				.repeatable(true)
				.argument("<name>=<value>"s)
				.callback(OptionCallback<RemoteGenApp>(this, &RemoteGenApp::handleDefine)));

		options.addOption(
			Option("config"s, "c"s,
				"Load configuration from the given file specified by <file>. "
				"This option is supported for backwards compatibility only."s)
				.required(false)
				.repeatable(false)
				.argument("<file>"s)
				.callback(OptionCallback<RemoteGenApp>(this, &RemoteGenApp::handleConfig)));

		options.addOption(
			Option("mode"s, "m"s,
				"Override generation mode specified in configuration file. "
				"Valid values for <mode> are \"client\", \"server\", \"both\", \"skeleton\" or \"interface\"."s)
				.required(false)
				.repeatable(false)
				.argument("<mode>"s)
				.callback(OptionCallback<RemoteGenApp>(this, &RemoteGenApp::handleMode)));

		options.addOption(
			Option("compiler"s, "C"s,
				"Specify the compiler to use. Compilers are defined in the (global) configuration file. "
				"If not specified, the first compiler defined in the configuration file will be used."s)
				.required(false)
				.repeatable(false)
				.argument("<id>"s)
				.callback(OptionCallback<RemoteGenApp>(this, &RemoteGenApp::handleCompiler)));

		options.addOption(
			Option("osp"s, "o"s, "Create services for Applied Informatics Open Service Platform (OSP)."s)
				.required(false)
				.repeatable(false)
				.callback(OptionCallback<RemoteGenApp>(this, &RemoteGenApp::handleOSP)));

		options.addOption(
			Option("bridge"s, "b"s, "Create remote bridge classes."s)
				.required(false)
				.repeatable(false)
				.callback(OptionCallback<RemoteGenApp>(this, &RemoteGenApp::handleBridge)));

		options.addOption(
			Option("noglobal"s, "n"s, "Do not load global configuration. If specified, compilers must be configured in the project configuration."s)
				.required(false)
				.repeatable(false)
				.callback(OptionCallback<RemoteGenApp>(this, &RemoteGenApp::handleNoGlobal)));
	}

	void handleConfig(const std::string& name, const std::string& value)
	{
		loadConfiguration(value);
	}

	void handleCompiler(const std::string& name, const std::string& value)
	{
		_compiler = value;
	}

	void handleNoGlobal(const std::string& name, const std::string& value)
	{
		_noGlobalConfig = true;
	}

	void handleDefine(const std::string& name, const std::string& value)
	{
		defineProperty(value);
	}

	void defineProperty(const std::string& def)
	{
		std::string name;
		std::string value;
		std::string::size_type pos = def.find('=');
		if (pos != std::string::npos)
		{
			name.assign(def, 0, pos);
			value.assign(def, pos + 1, def.length() - pos);
		}
		else name = def;
		config().setString(name, value);
	}

	void handleMode(const std::string& name, const std::string& value)
	{
		_mode = Poco::toLower(value);
		if (_mode != BOTH && _mode != SERVER && _mode != CLIENT && _mode != INTERFACE && _mode != SKELETON)
		{
			throw Poco::InvalidArgumentException("invalid option value for mode: only use client|server|both|skeleton|interface");
			_mode.clear();
		}
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
		helpFormatter.setUsage("[<option> ...] <file> ..."s);
		helpFormatter.setHeader(
			"\n"
			"The Applied Informatics Remoting NG Code Generator.\n"
			"Copyright (c) 2006-2024 by Applied Informatics Software Engineering GmbH.\n"
			"All rights reserved.\n\n"
			"This program parses C++ header files annotated with "
			"Remoting attributes and generates C++ code for "
			"making C++ objects available remotely over a network.\n\n"
			"The following command line options are supported:"s
		);
		helpFormatter.setFooter(
			"For more information, please see the Remoting NG "
			"documentation at <https://www.appinf.com/docs>."s
		);
		helpFormatter.setIndent(8);
		helpFormatter.format(std::cout);
	}

	void handleOSP(const std::string& name, const std::string& value)
	{
		_enableOSP = true;
	}

	void handleBridge(const std::string& name, const std::string& value)
	{
		_enableBridge = true;
	}

	void generateAll()
	{
		std::string compilerKey = "RemoteGen.compiler[";
		if (_compiler.empty())
		{
			compilerKey += "0";
		}
		else
		{
			compilerKey += "@id=";
			compilerKey += _compiler;
		}
		compilerKey += "]";
		std::string compiler = config().getString(compilerKey + ".exec", ""s);
		std::string path = config().getString(compilerKey + ".path", ""s);
		std::string sysIncludes = config().getString("RemoteGen.system.include"s, ""s);
		std::string includes = config().getString("RemoteGen.files.include"s);
		std::string excludes = config().getString("RemoteGen.files.exclude"s, ""s);
		std::string includePaths = config().getString("RemoteGen.files.include-paths"s, ""s);
		_enableOSP = _enableOSP || config().getBool("RemoteGen.output.osp.enable"s, false);
		_enableBridge = _enableBridge || config().getBool("RemoteGen.output.bridge.enable"s, false);
		_enableBridgeSync = config().getBool("RemoteGen.output.bridge.synchronized"s, _enableBridgeSync);
		_bundlePath = config().getString("RemoteGen.output.bundle"s, _bundlePath);
		_bundleActivator = config().getString("RemoteGen.output.osp.bundleActivator"s, ""s);
		_enableTimestamps = config().getBool("RemoteGen.output.timestamps"s, true);

		if (compiler.empty()) throw Poco::NotFoundException("No compiler definition found in configuration file"s, _compiler);

		if (config().hasProperty("RemoteGen.output.namespace"s))
			checkNameSpaceExists(config().getString("RemoteGen.output.namespace"s));

		std::string serializationOrder = config().getString("RemoteGen.options.memberSerializationOrder"s, ""s);
		if (serializationOrder == "lexical")
			SerializerGenerator::setSerializationOrder(SerializerGenerator::SERIALIZE_LEXICAL);
		else if (serializationOrder == "as-declared")
			SerializerGenerator::setSerializationOrder(SerializerGenerator::SERIALIZE_AS_DECLARED);
		else if (serializationOrder != "")
			throw Poco::InvalidArgumentException("options.memberSerializationOrder must be \"lexical\" or \"as-declared\"");

		std::vector<std::string> inc;
		StringTokenizer sysIncludesTokenizer(sysIncludes, ",;\n"s, StringTokenizer::TOK_IGNORE_EMPTY | StringTokenizer::TOK_TRIM);
		for (const auto& p: sysIncludesTokenizer)
		{
			inc.push_back(Poco::Path::expand(p));
		}
		StringTokenizer includesTokenizer(includes, ",;\n"s, StringTokenizer::TOK_IGNORE_EMPTY | StringTokenizer::TOK_TRIM);
		for (const auto& p: includesTokenizer)
		{
			inc.push_back(Poco::Path::expand(p));
		}
		std::vector<std::string> exc;
		StringTokenizer excludesTokenizer(excludes, ",;\n"s, StringTokenizer::TOK_IGNORE_EMPTY | StringTokenizer::TOK_TRIM);
		for (const auto& p: excludesTokenizer)
		{
			exc.push_back(Poco::Path::expand(p));
		}

		std::string gccIncludes;
		std::string msvcIncludes;
		StringTokenizer includePathsTokenizer(includePaths, ",;\n"s, StringTokenizer::TOK_IGNORE_EMPTY | StringTokenizer::TOK_TRIM);
		for (const auto& p: includePathsTokenizer)
		{
			Poco::Path fullPath(Poco::Path::expand(p));
			gccIncludes += "-I;";
			gccIncludes += fullPath.toString();
			gccIncludes += ";";
			msvcIncludes += "/I;";
			msvcIncludes += fullPath.toString();
			msvcIncludes += ";";
		}
		config().setString("RemoteGen.internal.gcc.includes"s, gccIncludes);
		config().setString("RemoteGen.internal.msvc.includes"s, msvcIncludes);

		std::string opts = config().getString(compilerKey + ".options", ""s);
		Poco::CppParser::Utility::parseDir(inc, exc, _gst, compiler, opts, path);

		// now generate the code
		NameSpace::SymbolTable::const_iterator itS = _gst.begin();
		NameSpace::SymbolTable::const_iterator itSEnd = _gst.end();
		for (; itS != itSEnd; ++itS)
		{
			if (itS->second->kind() == Symbol::SYM_NAMESPACE)
			{
				generateSymbolTable(static_cast<const Poco::CppParser::NameSpace*>(itS->second));
			}
			else if (itS->second->kind() == Symbol::SYM_STRUCT)
			{
				// Note that Codegeneration will change the content of _gst, thus copy all structs to a seperate vector
				_structsToProcess.push_back(static_cast<const Poco::CppParser::Struct*>(itS->second));
			}
		}

		// now check that the Poco::RemotingNG namespace is known
		std::string helpMsg(
			"The configuration file element \"RemoteGen.files.include\" must contain the header files RemoteObject.h, Proxy.h and Skeleton.h. "
			"Check the configuration file for the headers and verify their paths.");
		Poco::CppParser::Symbol* pS = Poco::CppParser::NameSpace::root()->lookup("Poco::RemotingNG");
		if (!pS) throw Poco::NotFoundException("namespace Poco::RemotingNG", helpMsg);
		pS = Poco::CppParser::NameSpace::root()->lookup("Poco::RemotingNG::RemoteObject");
		if (!pS) throw Poco::NotFoundException("class Poco::RemotingNG::RemoteObject", helpMsg);
		pS = Poco::CppParser::NameSpace::root()->lookup("Poco::RemotingNG::Proxy");
		if (!pS) throw Poco::NotFoundException("class Poco::RemotingNG::Proxy", helpMsg);
		pS = Poco::CppParser::NameSpace::root()->lookup("Poco::RemotingNG::Skeleton");
		if (!pS) throw Poco::NotFoundException("class Poco::RemotingNG::Skeleton", helpMsg);

		std::vector<const Poco::CppParser::Struct*>::const_iterator itV = _structsToProcess.begin();
		std::vector<const Poco::CppParser::Struct*>::const_iterator itVEnd = _structsToProcess.end();
		for (; itV != itVEnd; ++itV)
		{
			handleStruct(*itV);
		}

		std::string swaggerPath = config().getString("RemoteGen.output.openAPI.path", "");
		if (!swaggerPath.empty())
		{
			generateSwagger(swaggerPath);
		}
		_structsToProcess.clear();

		generateWSDL();

		// now generate bundle
		if (_enableOSP && !_bundleServices.empty())
		{
			const std::string& outFile = _bundleServices[0].pStruct->getFile();
			Poco::Path incPath(outFile);
			const std::string extension = incPath.getExtension();
			incPath.setFileName("");

			Poco::Path srcPath(incPath);

			while (srcPath.depth() > 0 && Poco::toLower(srcPath.directory(srcPath.depth()-1)).find(INCLUDE) == std::string::npos)
				srcPath.popDirectory();
			if (srcPath.depth() > 0)
			{
				srcPath.popDirectory(); // pop INCLUDE
				srcPath.pushDirectory(SRC);
			}
			else
				srcPath = incPath;

			std::string incFileDir = config().getString("RemoteGen.output.include", incPath.toString());
			std::string srcFileDir = config().getString("RemoteGen.output.src", srcPath.toString());
			incPath = Poco::Path(incFileDir);
			srcPath = Poco::Path(srcFileDir);
			std::string defaultNameSpace = config().getString("RemoteGen.output.namespace", _bundleServices[0].pStruct->nameSpace()->fullName());
			std::string libraryName = config().getString("RemoteGen.output.library", config().getString("RemoteGen.output.libraryname", ""));
			std::string copyright = config().getString("RemoteGen.output.copyright", "");
			bool usePocoIncludeStyle = config().getBool("RemoteGen.output.pocostyleincludes", true);
			generateBundleActivator(incPath, srcPath, defaultNameSpace, libraryName, usePocoIncludeStyle, copyright);
		}
		_schemaGen.clear();
	}

	void generateSymbolTable(const Poco::CppParser::NameSpace* st)
	{
		// now generate the code
		NameSpace::Iterator it = st->begin();
		NameSpace::Iterator itEnd = st->end();
		for (; it != itEnd; ++it)
		{
			if (it->second->kind() == Symbol::SYM_NAMESPACE)
			{
				generateSymbolTable(static_cast<const Poco::CppParser::NameSpace*>(it->second));
			}
			else if (it->second->kind() == Symbol::SYM_STRUCT)
			{
				// Note that Codegeneration will change the content of _gst, thus copy all structs to a seperate vector
				_structsToProcess.push_back(static_cast<const Poco::CppParser::Struct*>(it->second));
			}
		}
	}

	void checkNameSpaceExists(const std::string& ns)
	{
		Poco::StringTokenizer aStr(ns, "::", Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY);
		Poco::StringTokenizer::Iterator it = aStr.begin();
		Poco::CppParser::NameSpace* pRoot = Poco::CppParser::NameSpace::root();

		while (it != aStr.end())
		{
			Poco::CppParser::NameSpace::SymbolTable nameSpaces;
			pRoot->nameSpaces(nameSpaces);
			Poco::CppParser::NameSpace::Iterator itNS = nameSpaces.find(*it);
			if (itNS == nameSpaces.end())
			{
				Poco::CppParser::NameSpace* pNS = new Poco::CppParser::NameSpace(*it, pRoot);
				nameSpaces.clear();
				pRoot->nameSpaces(nameSpaces);
				itNS = nameSpaces.find(*it);
				poco_assert (itNS != nameSpaces.end() && itNS->second == pNS);
			}
			pRoot = const_cast<Poco::CppParser::NameSpace*>(static_cast <const Poco::CppParser::NameSpace*> (itNS->second));
			++it;
		}
	}

	void handleStruct(const Poco::CppParser::Struct* pStruct)
	{
		const std::string& outFile = pStruct->getFile();
		Poco::Path incPath(outFile);
		const std::string extension = incPath.getExtension();
		incPath.setFileName("");

		Poco::Path srcPath(incPath);

		while (srcPath.depth() > 0 && Poco::toLower(srcPath.directory(srcPath.depth()-1)).find(INCLUDE) == std::string::npos)
			srcPath.popDirectory();
		if (srcPath.depth() > 0)
		{
			srcPath.popDirectory(); // pop INCLUDE
			srcPath.pushDirectory(SRC);
		}
		else
			srcPath = incPath;

		std::string incFileDir = AbstractGenerator::includePath(config(), pStruct);
		std::string srcFileDir = config().getString("RemoteGen.output.src"s, srcPath.toString());
		incPath = Poco::Path(incFileDir);
		srcPath = Poco::Path(srcFileDir);
		Poco::File aFile(incPath);
		aFile.createDirectories();
		aFile = Poco::File(srcPath);
		aFile.createDirectories();

		std::string defaultNameSpace = config().getString("RemoteGen.output.namespace"s, pStruct->nameSpace()->fullName());
		std::string libraryName = config().getString("RemoteGen.output.library"s, config().getString("RemoteGen.output.libraryname"s, ""s));
		std::string copyright = config().getString("RemoteGen.output.copyright"s, ""s);
		std::string mode = config().getString("RemoteGen.output.mode"s, BOTH);

		bool genBundle = !_bundleActivator.empty();

		if (!_mode.empty())
			mode = _mode;
		Poco::trimInPlace(mode);
		Poco::toLowerInPlace(mode);
		bool genInterface = true;
		bool genClient = true;
		bool genServer = true;
		if (mode == CLIENT || mode == INTERFACE)
			genServer = false;
		if (mode == SERVER || mode == INTERFACE || mode == SKELETON)
			genClient = false;
		if (mode == INTERFACE)
			genBundle = false;
		if (mode == SKELETON)
			genInterface = false;
		bool usePocoIncludeStyle = config().getBool("RemoteGen.output.pocostyleincludes"s, true);
		if (extension == H_EXT)
		{
			if (Poco::CodeGeneration::Utility::hasAnyRemoteProperty(pStruct))
			{
				if (genInterface)
				{
					generateInterface(pStruct, incPath, srcPath, defaultNameSpace, libraryName, usePocoIncludeStyle, copyright, _enableOSP);
				}
				else
				{
					generateInternalInterface(pStruct, incPath, srcPath, defaultNameSpace, libraryName, usePocoIncludeStyle, copyright, _enableOSP);
				}
				if (genBundle)
				{
					BundleActivatorGenerator::BundleService bs(pStruct, genClient, genServer);
					this->_bundleServices.push_back(bs);
				}

				if (genClient)
				{
					generateProxy(pStruct, incPath, srcPath, defaultNameSpace, libraryName, usePocoIncludeStyle, copyright);
					generateProxyFactory(pStruct, incPath, srcPath, defaultNameSpace, libraryName, usePocoIncludeStyle, copyright);
					generateClientHelper(pStruct, incPath, srcPath, defaultNameSpace, libraryName, usePocoIncludeStyle, copyright);
					bool hasEvents = GenUtility::hasEvents(pStruct);
					if (hasEvents)
					{
						std::string helpMsg(
							"The configuration file element \"RemoteGen.files.include\" must contain the header file EventSubscriber.h if events are being used in a remote interface. "
							"Check the configuration file for the header and verify its path.");
						Poco::CppParser::Symbol* pS = Poco::CppParser::NameSpace::root()->lookup("Poco::RemotingNG::EventSubscriber"s);
						if (!pS) throw Poco::NotFoundException("Poco::RemotingNG::EventSubscriber"s, helpMsg);
						generateEventSubscriber(pStruct, incPath, srcPath, defaultNameSpace, libraryName, usePocoIncludeStyle, copyright);
					}
				}

				if (genServer)
				{
					bool hasEvents = generateRemoteObject(pStruct, incPath, srcPath, defaultNameSpace, libraryName, usePocoIncludeStyle, copyright);
					if (hasEvents)
					{
						std::string helpMsg(
							"The configuration file element \"RemoteGen.files.include\" must contain the header file EventDispatcher.h if events are being used in a remote interface. "
							"Check the configuration file for the header and verify its path.");
						Poco::CppParser::Symbol* pS = Poco::CppParser::NameSpace::root()->lookup("Poco::RemotingNG::EventDispatcher"s);
						if (!pS) throw Poco::NotFoundException("Poco::RemotingNG::EventDispatcher"s, helpMsg);
						generateEventDispatcher(pStruct, incPath, srcPath, defaultNameSpace, libraryName, usePocoIncludeStyle, copyright);
					}
					if (_enableBridge)
					{
						generateRemoteBridge(pStruct, incPath, srcPath, defaultNameSpace, libraryName, usePocoIncludeStyle, copyright, _enableBridgeSync);
					}
					generateSkeleton(pStruct, incPath, srcPath, defaultNameSpace, libraryName, usePocoIncludeStyle, copyright);
					generateServerHelper(pStruct, incPath, srcPath, defaultNameSpace, libraryName, usePocoIncludeStyle, copyright);
				}

				std::string cfgPath("RemoteGen.schema.");
				cfgPath.append(pStruct->name());
				if (config().hasProperty(cfgPath))
				{
					std::string endPoints;
					endPoints = config().getString(cfgPath+".serviceLocation");

					Poco::StringTokenizer aTok(endPoints, ",; \n", Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY);
					Poco::StringTokenizer::Iterator it = aTok.begin();
					Poco::StringTokenizer::Iterator itEnd = aTok.end();
					if (it == itEnd)
						throw CodeGenerationException("At least one service location is required for class " + pStruct->name() + ".");
					//we can only generate schemas after we have seen all complex types, delay it until all data was processed!
					_schemaGen.push_back(pStruct);
				}
			}

			if (genClient || genServer)
			{
				Poco::CodeGeneration::CodeGenerator::Properties props;
				Poco::CodeGeneration::GeneratorEngine::parseProperties(pStruct, props);
				bool doSerialize = false;
				Poco::CodeGeneration::GeneratorEngine::getBoolProperty(props, Poco::CodeGeneration::Utility::SERIALIZE, doSerialize);
				if (doSerialize)
				{
					generateSerializer(pStruct, incPath, usePocoIncludeStyle, copyright);
					generateDeserializer(pStruct, incPath, usePocoIncludeStyle, copyright);
				}
			}
		}
	}

	void generateInterface(const Poco::CppParser::Struct* pStruct, const Poco::Path& incPath, const Poco::Path& srcPath, const std::string& defaultNameSpace, const std::string& libraryName, bool usePocoIncludeStyle, const std::string& copyright, bool enableOSP)
	{
		std::ofstream hOut;
		std::ofstream cppOut;
		openFiles(incPath, srcPath, InterfaceGenerator::generateClassName(pStruct), hOut, cppOut);
		Poco::CodeGeneration::CppGenerator cppGen(defaultNameSpace, libraryName, usePocoIncludeStyle, copyright, hOut, cppOut);
		cppGen.enableTimestamps(_enableTimestamps);
		InterfaceGenerator* pI = new InterfaceGenerator(cppGen, enableOSP);
		Poco::CodeGeneration::MethodPropertyFilter mi(pI, Poco::CodeGeneration::Utility::REMOTE);

		GeneratorEngine e;
		e.generate(pStruct, mi);

		hOut.close();
		cppOut.close();
	}

	void generateInternalInterface(const Poco::CppParser::Struct* pStruct, const Poco::Path& incPath, const Poco::Path& srcPath, const std::string& defaultNameSpace, const std::string& libraryName, bool usePocoIncludeStyle, const std::string& copyright, bool enableOSP)
	{
		Poco::NullOutputStream hOut;
		Poco::NullOutputStream cppOut;
		Poco::CodeGeneration::CppGenerator cppGen(defaultNameSpace, libraryName, usePocoIncludeStyle, copyright, hOut, cppOut);
		InterfaceGenerator* pI = new InterfaceGenerator(cppGen, enableOSP);
		Poco::CodeGeneration::MethodPropertyFilter mi(pI, Poco::CodeGeneration::Utility::REMOTE);

		GeneratorEngine e;
		e.generate(pStruct, mi);
	}

	void generateBundleActivator(const Poco::Path& incPath, const Poco::Path& srcPath, const std::string& defaultNameSpace, const std::string& libraryName, bool usePocoIncludeStyle, const std::string& copyright)
	{
		Poco::Path implPath(srcPath);
		implPath.makeDirectory();
		implPath.setBaseName(_bundleActivator + "Impl");
		implPath.setExtension(CPP_EXT);
		std::ofstream hOut;
		std::ofstream cppOut;
		std::ofstream implOut;
		std::ostream* pImplOut(0);
		openFiles(incPath, srcPath, _bundleActivator, hOut, cppOut);
		Poco::File impl(implPath);
		if (!impl.exists())
		{
			implOut.open(implPath.toString().c_str());
			pImplOut = &implOut;
			Poco::CodeGeneration::Utility::writeStdHeader(implOut, implPath.getFileName(), libraryName, libraryName, implPath.getBaseName(), copyright);
		}
		if (!_bundleServices.empty() && _bundleServices[0].clientCode)
		{
			if (_bundlePath.empty())
			{
				if (incPath.toString() == srcPath.toString())
					_bundlePath = srcPath.toString();
				else
					_bundlePath = srcPath.parent().toString();
			}

			Poco::Path bundle(_bundlePath);
			bundle.makeDirectory();
			bundle.setFileName("extensions.xml"s);
			Poco::File fBundle(bundle);
			if (!fBundle.exists())
			{
				Poco::File bP(_bundlePath);
				bP.createDirectories();
				std::ofstream out(bundle.toString().c_str());
				ExtensionsGenerator gen(out);
				gen.generate(_bundleServices);
			}
		}

		Poco::CodeGeneration::CppGenerator cppGen(defaultNameSpace, libraryName, usePocoIncludeStyle, copyright, hOut, cppOut);
		cppGen.enableTimestamps(_enableTimestamps);
		BundleActivatorGenerator* pB = new BundleActivatorGenerator(cppGen, pImplOut, _bundleActivator, _bundleServices);
		Poco::CodeGeneration::MethodPropertyFilter mi(pB, Poco::CodeGeneration::Utility::REMOTE);

		GeneratorEngine e;
		e.generate(_bundleServices[0].pStruct, mi);

		hOut.close();
		cppOut.close();
		if (pImplOut)
		{
			implOut.close();
		}
	}

	bool generateRemoteObject(const Poco::CppParser::Struct* pStruct, const Poco::Path& incPath, const Poco::Path& srcPath, const std::string& defaultNameSpace, const std::string& libraryName, bool usePocoIncludeStyle, const std::string& copyright)
	{
		std::ofstream hOut;
		std::ofstream cppOut;
		openFiles(incPath, srcPath, RemoteObjectGenerator::generateClassName(pStruct), hOut, cppOut);
		Poco::CodeGeneration::CppGenerator cppGen(defaultNameSpace, libraryName, usePocoIncludeStyle, copyright, hOut, cppOut);
		cppGen.enableTimestamps(_enableTimestamps);
		RemoteObjectGenerator* pGen = new RemoteObjectGenerator(cppGen);
		Poco::CodeGeneration::MethodPropertyFilter mi(pGen, Poco::CodeGeneration::Utility::REMOTE);

		GeneratorEngine e;
		e.generate(pStruct, mi);

		hOut.close();
		cppOut.close();
		return pGen->eventsFound();
	}

	void generateRemoteBridge(const Poco::CppParser::Struct* pStruct, const Poco::Path& incPath, const Poco::Path& srcPath, const std::string& defaultNameSpace, const std::string& libraryName, bool usePocoIncludeStyle, const std::string& copyright, bool sync)
	{
		std::ofstream hOut;
		std::ofstream cppOut;
		openFiles(incPath, srcPath, RemoteBridgeGenerator::generateClassName(pStruct), hOut, cppOut);
		Poco::CodeGeneration::CppGenerator cppGen(defaultNameSpace, libraryName, usePocoIncludeStyle, copyright, hOut, cppOut);
		cppGen.enableTimestamps(_enableTimestamps);
		RemoteBridgeGenerator* pGen = new RemoteBridgeGenerator(cppGen, sync);
		Poco::CodeGeneration::MethodPropertyFilter mi(pGen, Poco::CodeGeneration::Utility::REMOTE);

		GeneratorEngine e;
		e.generate(pStruct, mi);

		hOut.close();
		cppOut.close();
	}

	void generateEventDispatcher(const Poco::CppParser::Struct* pStruct, const Poco::Path& incPath, const Poco::Path& srcPath, const std::string& defaultNameSpace, const std::string& libraryName, bool usePocoIncludeStyle, const std::string& copyright)
	{
		std::ofstream hOut;
		std::ofstream cppOut;
		openFiles(incPath, srcPath, EventDispatcherGenerator::generateClassName(pStruct), hOut, cppOut);
		Poco::CodeGeneration::CppGenerator cppGen(defaultNameSpace, libraryName, usePocoIncludeStyle, copyright, hOut, cppOut);
		cppGen.enableTimestamps(_enableTimestamps);
		EventDispatcherGenerator* pGen = new EventDispatcherGenerator(cppGen);
		Poco::CodeGeneration::MethodPropertyFilter mi(pGen, Poco::CodeGeneration::Utility::REMOTE);

		GeneratorEngine e;
		e.generate(pStruct, mi);

		hOut.close();
		cppOut.close();
	}

	void generateEventSubscriber(const Poco::CppParser::Struct* pStruct, const Poco::Path& incPath, const Poco::Path& srcPath, const std::string& defaultNameSpace, const std::string& libraryName, bool usePocoIncludeStyle, const std::string& copyright)
	{
		std::ofstream hOut;
		std::ofstream cppOut;
		openFiles(incPath, srcPath, EventSubscriberGenerator::generateClassName(pStruct), hOut, cppOut);
		Poco::CodeGeneration::CppGenerator cppGen(defaultNameSpace, libraryName, usePocoIncludeStyle, copyright, hOut, cppOut);
		cppGen.enableTimestamps(_enableTimestamps);
		EventSubscriberGenerator* pGen = new EventSubscriberGenerator(cppGen);
		Poco::CodeGeneration::MethodPropertyFilter mi(pGen, Poco::CodeGeneration::Utility::REMOTE);

		GeneratorEngine e;
		e.generate(pStruct, mi);

		hOut.close();
		cppOut.close();
	}

	void generateProxy(const Poco::CppParser::Struct* pStruct, const Poco::Path& incPath, const Poco::Path& srcPath, const std::string& defaultNameSpace, const std::string& libraryName, bool usePocoIncludeStyle, const std::string& copyright)
	{
		std::ofstream hOut;
		std::ofstream cppOut;
		openFiles(incPath, srcPath, ProxyGenerator::generateClassName(pStruct), hOut, cppOut);
		Poco::CodeGeneration::CppGenerator cppGen(defaultNameSpace, libraryName, usePocoIncludeStyle, copyright, hOut, cppOut);
		cppGen.enableTimestamps(_enableTimestamps);
		ProxyGenerator* pGen = new ProxyGenerator(cppGen);
		Poco::CodeGeneration::MethodPropertyFilter mi(pGen, Poco::CodeGeneration::Utility::REMOTE);

		GeneratorEngine e;
		e.generate(pStruct, mi);

		hOut.close();
		cppOut.close();
	}

	void generateProxyFactory(const Poco::CppParser::Struct* pStruct, const Poco::Path& incPath, const Poco::Path& srcPath, const std::string& defaultNameSpace, const std::string& libraryName, bool usePocoIncludeStyle, const std::string& copyright)
	{
		std::ofstream hOut;
		std::ofstream cppOut;
		openFiles(incPath, srcPath, ProxyFactoryGenerator::generateClassName(pStruct), hOut, cppOut);
		Poco::CodeGeneration::CppGenerator cppGen(defaultNameSpace, libraryName, usePocoIncludeStyle, copyright, hOut, cppOut);
		cppGen.enableTimestamps(_enableTimestamps);
		ProxyFactoryGenerator* pGen = new ProxyFactoryGenerator(cppGen);
		Poco::CodeGeneration::MethodPropertyFilter mi(pGen, Poco::CodeGeneration::Utility::REMOTE);

		GeneratorEngine e;
		e.generate(pStruct, mi);

		hOut.close();
		cppOut.close();
	}

	void generateSkeleton(const Poco::CppParser::Struct* pStruct, const Poco::Path& incPath, const Poco::Path& srcPath, const std::string& defaultNameSpace, const std::string& libraryName, bool usePocoIncludeStyle, const std::string& copyright)
	{
		std::ofstream hOut;
		std::ofstream cppOut;
		openFiles(incPath, srcPath, SkeletonGenerator::generateClassName(pStruct), hOut, cppOut);
		Poco::CodeGeneration::CppGenerator cppGen(defaultNameSpace, libraryName, usePocoIncludeStyle, copyright, hOut, cppOut);
		cppGen.enableTimestamps(_enableTimestamps);
		SkeletonGenerator* pGen = new SkeletonGenerator(cppGen);
		Poco::CodeGeneration::MethodPropertyFilter mi(pGen, Poco::CodeGeneration::Utility::REMOTE);

		GeneratorEngine e;
		e.generate(pStruct, mi);

		hOut.close();
		cppOut.close();
	}

	void generateClientHelper(const Poco::CppParser::Struct* pStruct, const Poco::Path& incPath, const Poco::Path& srcPath, const std::string& defaultNameSpace, const std::string& libraryName, bool usePocoIncludeStyle, const std::string& copyright)
	{
		std::ofstream hOut;
		std::ofstream cppOut;
		openFiles(incPath, srcPath, ClientHelperGenerator::generateClassName(pStruct), hOut, cppOut);
		Poco::CodeGeneration::CppGenerator cppGen(defaultNameSpace, libraryName, usePocoIncludeStyle, copyright, hOut, cppOut);
		cppGen.enableTimestamps(_enableTimestamps);
		ClientHelperGenerator* pGen = new ClientHelperGenerator(cppGen);
		Poco::CodeGeneration::MethodPropertyFilter mi(pGen, Poco::CodeGeneration::Utility::REMOTE);

		GeneratorEngine e;
		e.generate(pStruct, mi);

		hOut.close();
		cppOut.close();
	}

	void generateServerHelper(const Poco::CppParser::Struct* pStruct, const Poco::Path& incPath, const Poco::Path& srcPath, const std::string& defaultNameSpace, const std::string& libraryName, bool usePocoIncludeStyle, const std::string& copyright)
	{
		std::ofstream hOut;
		std::ofstream cppOut;
		openFiles(incPath, srcPath, ServerHelperGenerator::generateClassName(pStruct), hOut, cppOut);
		Poco::CodeGeneration::CppGenerator cppGen(defaultNameSpace, libraryName, usePocoIncludeStyle, copyright, hOut, cppOut);
		cppGen.enableTimestamps(_enableTimestamps);
		ServerHelperGenerator* pGen = new ServerHelperGenerator(cppGen);
		Poco::CodeGeneration::MethodPropertyFilter mi(pGen, Poco::CodeGeneration::Utility::REMOTE);

		GeneratorEngine e;
		e.generate(pStruct, mi);

		hOut.close();
		cppOut.close();
	}

	void generateSerializer(const Poco::CppParser::Struct* pStruct, const Poco::Path& incPath, bool usePocoIncludeStyle, const std::string& copyright)
	{
		std::ofstream hOut;
		Poco::NullOutputStream cppOut;
		openTemplateFiles(incPath, SerializerGenerator::generateClassName(pStruct), hOut);
		Poco::CodeGeneration::CppGenerator cppGen("Poco::RemotingNG"s, "RemotingNG"s, usePocoIncludeStyle, copyright, hOut, cppOut);
		cppGen.enableTimestamps(_enableTimestamps);
		SerializerGenerator gen(cppGen);

		GeneratorEngine e;
		e.generate(pStruct, gen);

		hOut.close();
	}

	void generateDeserializer(const Poco::CppParser::Struct* pStruct, const Poco::Path& incPath, bool usePocoIncludeStyle, const std::string& copyright)
	{
		std::ofstream hOut;
		Poco::NullOutputStream cppOut;
		openTemplateFiles(incPath, DeserializerGenerator::generateClassName(pStruct), hOut);
		Poco::CodeGeneration::CppGenerator cppGen("Poco::RemotingNG"s, "RemotingNG"s, usePocoIncludeStyle, copyright, hOut, cppOut);
		cppGen.enableTimestamps(_enableTimestamps);
		DeserializerGenerator gen(cppGen);

		GeneratorEngine e;
		e.generate(pStruct, gen);

		hOut.close();
	}

	void generateWSDL()
	{
		auto itV = _schemaGen.begin();
		auto itVEnd = _schemaGen.end();
		for (; itV != itVEnd; ++itV)
		{
			const Poco::CppParser::Struct* pStruct = *itV;
			const std::string& outFile = pStruct->getFile();
			Poco::Path incPath(outFile);

			while (incPath.depth() > 0 &&
				Poco::toLower(incPath.directory(incPath.depth()-1)).find("include"s) == std::string::npos &&
				Poco::toLower(incPath.directory(incPath.depth()-1)).find("src"s) == std::string::npos)
				incPath.popDirectory();
			if (incPath.depth() > 0)
			{
				incPath.popDirectory();
			}
			else
				incPath = outFile;

			incPath.setFileName("");

			std::string schemaPath = config().getString("RemoteGen.output.schema"s, ""s);
			if (!schemaPath.empty())
			{
				Poco::File schemaDir(schemaPath);
				schemaDir.createDirectories();
			}
			else schemaPath = Poco::Path::current();

			std::string cfgPath("RemoteGen.schema.");
			cfgPath.append(pStruct->name());
			// targetNS must be set at the class
			Poco::CodeGeneration::CodeGenerator::Properties structProps;
			Poco::CodeGeneration::GeneratorEngine::parseProperties(pStruct, structProps);
			std::string targetNS;
			Poco::CodeGeneration::GeneratorEngine::getStringProperty(structProps, Poco::CodeGeneration::Utility::NAMESPACE, targetNS);
			if (targetNS.empty())
				throw CodeGenerationException("A SOAP/WSDL web service requires an XML namespace. Add a @namespace attribute to class " + pStruct->fullName() + ".");

			generateWSDL(pStruct, schemaPath, targetNS);
		}
	}

	void generateWSDL(const Poco::CppParser::Struct* pStruct, const Poco::Path& aPath, const std::string& targetNamespace)
	{
		Poco::NullOutputStream hOut;
		Poco::NullOutputStream cppOut;
		Poco::Path wsdlPath(aPath);
		wsdlPath.makeDirectory();
		wsdlPath.setFileName(WSDLGenerator::generateFileName(pStruct));

		Poco::FileOutputStream wsdlOut(wsdlPath.toString().c_str());
		if (!wsdlOut)
			throw Poco::FileException("Failed to create file " + wsdlPath.toString());

		Poco::XML::XMLWriter aWriter(wsdlOut, Poco::XML::XMLWriter::PRETTY_PRINT);
		Poco::CodeGeneration::CppGenerator cppGen("Poco::RemotingNG"s, "RemotingNG"s, true, ""s, hOut, cppOut);
		cppGen.enableTimestamps(_enableTimestamps);
		WSDLGenerator gen(cppGen, targetNamespace, aWriter);

		GeneratorEngine e;
		e.generate(pStruct, gen);

		wsdlOut.close();
	}

	void generateSwagger(const std::string& swaggerPath)
	{
		Poco::NullOutputStream hOut;
		Poco::NullOutputStream cppOut;
		Poco::CodeGeneration::CppGenerator cppGen("Poco::RemotingNG"s, "RemotingNG"s, true, ""s, hOut, cppOut);
		cppGen.enableTimestamps(_enableTimestamps);

		SwaggerGenerator::Info info;
		info.title = config().getString("RemoteGen.output.openAPI.info.title"s);
		info.description = config().getString("RemoteGen.output.openAPI.info.description"s, ""s);
		info.version = config().getString("RemoteGen.output.openAPI.info.version"s, "1.0.0"s);
		info.termsOfService = config().getString("RemoteGen.output.openAPI.info.termsOfService"s, ""s);
		info.contactName = config().getString("RemoteGen.output.openAPI.info.contact.name"s, ""s);
		info.contactUrl = config().getString("RemoteGen.output.openAPI.info.contact.url"s, ""s);
		info.contactEmail = config().getString("RemoteGen.output.openAPI.info.contact.email"s, ""s);
		info.licenseName = config().getString("RemoteGen.output.openAPI.info.license.name"s, ""s);
		info.licenseUrl = config().getString("RemoteGen.output.openAPI.info.license.url"s, ""s);

		std::vector<SwaggerGenerator::Server> servers;
		int serverIndex = 0;
		std::string serverUrlFmt = "RemoteGen.output.openAPI.server[%d].url";
		std::string serverDescriptionFmt = "RemoteGen.output.openAPI.server[%d].description";
		while (config().has(Poco::format(serverUrlFmt, serverIndex)))
		{
			SwaggerGenerator::Server server;
			server.url = config().getString(Poco::format(serverUrlFmt, serverIndex));
			server.description = config().getString(Poco::format(serverDescriptionFmt, serverIndex), ""s);
			servers.push_back(server);
			serverIndex++;
		}

		SwaggerGenerator swaggerGen(cppGen, info, servers);

		for (auto pStruct: _structsToProcess)
		{
			GeneratorEngine e;
			e.generate(pStruct, swaggerGen);
		}

		Poco::Path path(swaggerPath);
		path.makeAbsolute();
		Poco::Path dirPath(path.parent());
		if (dirPath.depth() > 0)
		{
			Poco::File dir(dirPath.toString());
			dir.createDirectories();
		}
		Poco::FileOutputStream swaggerStream(swaggerPath);

		int indent = config().getInt("RemoteGen.output.openAPI.json.indent"s, 2);
		swaggerGen.json()->stringify(swaggerStream, indent, indent);
	}

	void openFiles(const Poco::Path& incPath, const Poco::Path& srcPath, const std::string& baseName, std::ofstream& hOut, std::ofstream& cppOut)
	{
		Poco::Path hPath(incPath);
		hPath.makeDirectory();
		hPath.setBaseName(baseName);
		hPath.setExtension(H_EXT);
		hOut.open(hPath.toString().c_str());

		if (hOut)
		{
			Poco::Path cppPath(srcPath);
			cppPath.makeDirectory();
			cppPath.setBaseName(baseName);
			cppPath.setExtension(CPP_EXT);
			cppOut.open(cppPath.toString().c_str());
			if (!cppOut)
			{
				hOut.close();
				Poco::CppParser::Utility::removeFile(hPath.toString());
				throw Poco::FileException("Failed to create file " + cppPath.toString() + ". Please make sure the file is writable.");
			}
		}
		else
			throw Poco::FileException("Failed to create file " + hPath.toString() + ". Please make sure the file is writable.");
	}

	void openTemplateFiles(const Poco::Path& path, const std::string& baseName, std::ofstream& hOut)
	{
		Poco::Path hPath(path);
		hPath.makeDirectory();
		hPath.setBaseName(baseName);
		hPath.setExtension(H_EXT);
		hOut.open(hPath.toString().c_str());

		if (!hOut)
			throw Poco::FileException("Failed to create file " + hPath.toString());
	}

	void onError(const void*, const std::string& errMsg)
	{
		_errMsgs.push_back(errMsg);
	}

	int main(const std::vector<std::string>& args)
	{
		if (!_helpRequested)
		{
			try
			{
				for (std::vector<std::string>::const_iterator it = args.begin(); it != args.end(); ++it)
				{
					loadConfiguration(*it);
				}

				if (config().hasProperty("RemoteGen.files.include"s))
				{
					generateAll();
				}
				else
				{
					std::cerr << "Error: no configuration found.\n" << std::endl;
					displayHelp();
				}
			}
			catch (Exception& exc)
			{
				_errMsgs.push_back(exc.displayText());
			}
			std::vector<std::string>::const_iterator it = _errMsgs.begin();
			std::vector<std::string>::const_iterator itEnd = _errMsgs.end();
			for (; it != itEnd; ++it)
			{
				std::cerr << *it << std::endl;
			}
		}
		return _errMsgs.empty() ? Application::EXIT_OK : Application::EXIT_SOFTWARE;
	}

private:
	bool _helpRequested;
	bool _noGlobalConfig;
	bool _enableOSP;
	bool _enableBridge;
	bool _enableBridgeSync;
	std::string _compiler;
	std::string _bundleActivator;
	std::string _bundlePath;
	Poco::CppParser::NameSpace::SymbolTable _gst;
	std::vector<std::string> _errMsgs;
	std::vector<const Poco::CppParser::Struct*> _schemaGen;
	std::vector<const Poco::CppParser::Struct*> _structsToProcess;
	std::vector<BundleActivatorGenerator::BundleService> _bundleServices;
	std::string  _mode;
	bool _enableTimestamps;
	static const std::string H_EXT;
	static const std::string CPP_EXT;
	static const std::string INCLUDE;
	static const std::string SRC;
	static const std::string BOTH;
	static const std::string CLIENT;
	static const std::string SERVER;
	static const std::string SKELETON;
	static const std::string INTERFACE;
};


const std::string RemoteGenApp::H_EXT("h");
const std::string RemoteGenApp::CPP_EXT("cpp");
const std::string RemoteGenApp::INCLUDE("include");
const std::string RemoteGenApp::SRC("src");
const std::string RemoteGenApp::BOTH("both");
const std::string RemoteGenApp::CLIENT("client");
const std::string RemoteGenApp::SERVER("server");
const std::string RemoteGenApp::SKELETON("skeleton");
const std::string RemoteGenApp::INTERFACE("interface");


int main(int argc, char** argv)
{
	RemoteGenApp app;
	try
	{
		app.init(argc, argv);
	}
	catch (Poco::Exception& exc)
	{
		app.logger().log(exc);
		return Application::EXIT_CONFIG;
	}
	return app.run();
}
