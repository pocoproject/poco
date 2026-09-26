//
// main.cpp
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties,
// copied or duplicated in any form, in whole or in part.
//


#include "Poco/NumberFormatter.h"
#include "Poco/URIStreamOpener.h"
#include "Poco/StreamCopier.h"
#include "Poco/StringTokenizer.h"
#include "Poco/String.h"
#include "Poco/Path.h"
#include "Poco/File.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"
#include "Poco/Net/HTTPStreamFactory.h"
#include "Poco/Net/FTPStreamFactory.h"
#if POCO_XSD_ENABLE_HTTPS
#include "Poco/Net/HTTPSStreamFactory.h"
#endif
#include "Poco/XSD/Parser/XSDContentHandler.h"
#include "Poco/XSD/Types/TypesManager.h"
#include "Poco/XSD/Types/XSDException.h"
#include "Poco/SAX/SAXParser.h"
#include "Poco/SAX/InputSource.h"
#include "Poco/Util/Application.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/Util/AbstractConfiguration.h"
#include "Poco/Util/OptionException.h"
#include "Poco/AutoPtr.h"
#include "CppGen.h"
#include "CppWriter.h"
#include <memory>
#include <fstream>
#include <iostream>


using Poco::NumberFormatter;
using Poco::URIStreamOpener;
using Poco::StreamCopier;
using Poco::StringTokenizer;
using Poco::Path;
using Poco::URI;
using Poco::Exception;
using Poco::XSD::Parser::XSDContentHandler;
using Poco::XSD::Types::TypesManager;
using Poco::XML::SAXParser;
using Poco::XML::InputSource;
using Poco::XML::XMLReader;
using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;
using Poco::Util::AbstractConfiguration;
using Poco::Util::OptionCallback;
using Poco::AutoPtr;


class GenApp: public Application
{
public:
	GenApp():
		_helpRequested(false),
		_generateAllBindings(false),
		_ignoreParameterOrder(true),
		_namespaceInSourceFileNames(false),
		_namespaceInHeaderFileNames(false),
		_alwaysUseOptional(false),
		_useStd(false),
		_inlineAll(false),
		_amalgamateSources(false)
	{
	}

protected:
	void initialize(Application& self)
	{
		Application::initialize(self);
		Poco::Net::HTTPStreamFactory::registerFactory();
		Poco::Net::FTPStreamFactory::registerFactory();
#ifdef POCO_XSD_ENABLE_HTTPS
		Poco::Net::HTTPSStreamFactory::registerFactory();
#endif
	}

	void uninitialize()
	{
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
			Option("help", "h", "Display help information on command line arguments.")
				.required(false)
				.repeatable(false)
				.callback(OptionCallback<GenApp>(this, &GenApp::handleHelp)));

		options.addOption(
			Option("config-file", "c", "Load XSD code generator configuration data from the given file.")
				.required(false)
				.repeatable(false)
				.argument("file")
				.callback(OptionCallback<GenApp>(this, &GenApp::handleConfig)));

		options.addOption(
			Option("generate-all-bindings", "a", "Generate service classes for all bindings.")
				.required(false)
				.repeatable(false)
				.callback(OptionCallback<GenApp>(this, &GenApp::handleGenerateAllBindings)));
	}

	void handleHelp(const std::string& name, const std::string& value)
	{
		_helpRequested = true;
		displayHelp();
		stopOptionsProcessing();
	}

	void handleConfig(const std::string& name, const std::string& value)
	{
		Poco::Path aPath(value);
		aPath.makeAbsolute();
		std::string fullPath = aPath.toString();
		loadConfiguration(fullPath);
	}

	void handleGenerateAllBindings(const std::string& name, const std::string& value)
	{
		_generateAllBindings = true;
	}

	void displayHelp()
	{
		HelpFormatter helpFormatter(options());
		helpFormatter.setCommand(commandName());
		helpFormatter.setUsage("[<option> ...] <file> ...");
		helpFormatter.setHeader(
			"\n"
			"The Applied Informatics XML Schema and WSDL C++ Code Generator.\n"
			"Copyright (c) 2008-2024 by Applied Informatics Software Engineering GmbH.\n"
			"All rights reserved.\n\n"
			"This program parses XML Schema and WSDL files and generates "
			"C++ classes annotated with Remoting attributes "
			"from the types and services defined therein.\n\n"
			"The following command line options are supported:"
		);
		helpFormatter.setFooter(
#ifdef POCO_XSD_ENABLE_HTTPS
#ifdef POCO_NETSSL_WIN
			"Built with HTTPS support (Schannel).\n"
#else
			"Built with HTTPS support (OpenSSL).\n"
#endif
#endif
			"For more information, please see the Remoting NG "
			"documentation at <https://www.appinf.com/docs>."
		);
		helpFormatter.setIndent(8);
		helpFormatter.format(std::cout);
	}

	void parseConfig()
	{
		_ignoreParameterOrder = !config().getBool("XSDGen.options.honorParameterOrder", false);
		_namespaceInSourceFileNames = config().getBool("XSDGen.options.namespaceInSourceFileNames", false);
		_namespaceInHeaderFileNames = config().getBool("XSDGen.options.namespaceInHeaderFileNames", false);
		_alwaysUseOptional = config().getBool("XSDGen.options.alwaysUseOptional", false);
		_useStd = config().getBool("XSDGen.options.useStdUtilities", false);
		_inlineAll = config().getBool("XSDGen.options.inlineAllMethods", false);
		_amalgamateSources = config().getBool("XSDGen.options.amalgamateSources", false);
		_amalgamatedSourceFileName = config().getString("XSDGen.options.amalgamatedSourceFileName", "Amalgamated");
		_generateAllBindings = config().getBool("XSDGen.options.generateAllBindings", _generateAllBindings);

		SchemaInfo def = parseDefault();
		_schemas.insert(std::make_pair(def.id(), def));

		int pos = 0;
		std::string prefix = "XSDGen.schema[";
		prefix += NumberFormatter::format(pos);
		prefix += "][@targetNamespace]";
		while (config().hasProperty(prefix))
		{
			SchemaInfo tmp = parseSchema(def, pos++);
			_schemas.insert(std::make_pair(tmp.id(), tmp));
			prefix = "XSDGen.schema[";
			prefix += NumberFormatter::format(pos);
			prefix += "][@targetNamespace]";
		}
	}

	SchemaInfo parseDefault()
	{
		/*
		<default>
			<namespace></namespace>
			<library></library>
			<include></incude>
			<src></src>
			<includeRoot></includeRoot>
			<alwaysInclude></alwaysInclude>
			<copyright></copyright>
			<attributes></attributes>
		</default>
		*/
		std::string cppNS = config().getString("XSDGen.default.namespace", "");
		std::string declSpec = config().getString("XSDGen.default.library", "");
		if (!declSpec.empty()) declSpec += "_API";
		std::string incDir = Path::expand(config().getString("XSDGen.default.include", ""));
		std::string srcDir = Path::expand(config().getString("XSDGen.default.src", ""));
		std::string rootIncDir = Path::expand(config().getString("XSDGen.default.includeRoot", ""));
		std::string cr = config().getString("XSDGen.default.copyright", "");
		bool remHeaders = config().getBool("XSDGen.default.attributes", true);
		bool preserveOptional = config().getBool("XSDGen.default.preserveOptional", false);
		bool timestamps = config().getBool("XSDGen.default.timestamps", false);
		// parse copyright into a vector of lines
		std::vector<std::string> copyright = splitTextBlock(cr);
		std::set<std::string> extraIncludes = splitIncludes(config().getString("XSDGen.default.alwaysInclude", ""));
		return SchemaInfo("", cppNS, declSpec, Poco::Path(incDir), Poco::Path(srcDir), Poco::Path(rootIncDir), copyright, extraIncludes, remHeaders, preserveOptional, _namespaceInHeaderFileNames, timestamps);
	}

	static std::vector<std::string> splitTextBlock(const std::string& block)
	{
		std::vector<std::string> result;
		StringTokenizer tok(block, "\n", StringTokenizer::TOK_TRIM);
		StringTokenizer::Iterator it = tok.begin();
		for (; it != tok.end(); ++it)
		{
			result.push_back(*it);
		}
		return result;
	}

	static std::set<std::string> splitIncludes(const std::string& aList)
	{
		std::set<std::string> result;
		StringTokenizer tok(aList, ",\r\n;", StringTokenizer::TOK_IGNORE_EMPTY | StringTokenizer::TOK_TRIM);
		StringTokenizer::Iterator it = tok.begin();
		for (; it != tok.end(); ++it)
		{
			result.insert(*it);
		}
		return result;
	}

	SchemaInfo parseSchema(const SchemaInfo& def, int pos)
	{
		/*
		<schema targetNamespace="tns">
			<namespace></namespace>
			<library></library>
			<include></include>
			<src></src>
			<includeRoot></includeRoot>
			<alwaysInclude></alwaysInclude>
			<copyright></copyright>
		</schema>
		*/
		std::string prefix = "XSDGen.schema[";
		prefix += NumberFormatter::format(pos);
		prefix += "]";

		std::string tns = config().getString(prefix + "[@targetNamespace]", "");
		if (tns.empty())
			throw Poco::Util::EmptyOptionException("targetNamespace must be specified for schema element");
		std::string loc = config().getString(prefix + "[@location]", "");
		if (!loc.empty())
		{
			_schemaMap[tns] = loc;
		}
		prefix += '.';
		std::string cppNS = config().getString(prefix + "namespace");
		std::string declSpec = config().getString(prefix + "library", "");
		if (!declSpec.empty())
			declSpec += "_API";
		else
			declSpec = def.dllPrefix();
		std::string incDir = Path::expand(config().getString(prefix + "include", def.includeDir().toString()));
		std::string srcDir = Path::expand(config().getString(prefix + "src", def.sourceDir().toString()));
		std::string rootIncDir = Path::expand(config().getString(prefix + "includeRoot", def.rootInclude().toString()));
		std::vector<std::string> copyright = def.copyright();
		if (config().hasProperty(prefix + "copyright"))
		{
			std::string cr = config().getString(prefix + "copyright", "");
			Poco::trimInPlace(cr);
			// parse copyright into a vector of lines
			copyright = splitTextBlock(cr);
		}
		std::set<std::string> extraIncludes = def.extraIncludes();
		if (config().hasProperty(prefix + "alwaysInclude"))
		{
			extraIncludes = splitIncludes(config().getString("XSDGen.default.alwaysInclude", ""));
		}
		bool remHeaders = config().getBool(prefix + "attributes", def.remotingAttributes());
		bool preserveOptional = config().getBool(prefix + "preserveOptional", def.preserveOptional());
		bool timestamps = config().getBool(prefix + "timestamps", def.timestamps());

		return SchemaInfo(tns, cppNS, declSpec, Poco::Path(incDir), Poco::Path(srcDir), Poco::Path(rootIncDir), copyright, extraIncludes, remHeaders, preserveOptional, _namespaceInHeaderFileNames, timestamps);
	}

	int main(const std::vector<std::string>& args)
	{
		if (!_helpRequested)
		{
			if (args.empty())
			{
				displayHelp();
				return 1;
			}

			parseConfig();

			std::vector<std::string>::const_iterator it = args.begin();
			for (; it != args.end(); ++it)
			{
				URI uri;
				if (it->find("://") != std::string::npos)
				{
					uri = *it;
				}
				else
				{
					Poco::Path p(*it);
					p.makeAbsolute();
					uri = "file://" + p.toString(Poco::Path::PATH_UNIX);
				}
				std::unique_ptr<std::istream> pStr(URIStreamOpener::defaultOpener().open(uri));
				InputSource in(*pStr);
				in.setSystemId(uri.toString());
				XSDContentHandler xsd(uri, _schemaMap);
				SAXParser parser;
				parser.setFeature(XMLReader::FEATURE_NAMESPACES, true);
				parser.setFeature(XMLReader::FEATURE_NAMESPACE_PREFIXES, true);
				parser.setContentHandler(&xsd);
				parser.parse(&in);
			}
			// now iterate over schema (except the builtin one)
			// and first visit all, then set includes, then generate code
			TypesManager& tm = TypesManager::instance();
			tm.fixupSchemas();
			int options = 0;
			if (_generateAllBindings) options |= CppGen::OPT_GENERATE_ALL_BINDINGS;
			if (_ignoreParameterOrder) options |= CppGen::OPT_IGNORE_PARAMETER_ORDER;

			std::string dllExportMacro = config().getString("XSDGen.default.library", "");
			CppGen gen(_schemas, dllExportMacro, options);
			const TypesManager::Schemas& allSchemas = tm.getSchemas();
			TypesManager::Schemas::const_iterator itS = allSchemas.begin();
			for (; itS != allSchemas.end(); ++itS)
			{
				if (!Utility::isBuiltinNamespace(itS->second->targetNamespace()))
				{
					gen.prepare(*itS->second);
				}
			}
			itS = allSchemas.begin();
			for (; itS != allSchemas.end(); ++itS)
			{
				if (!Utility::isBuiltinNamespace(itS->second->targetNamespace()))
				{
					gen.visit(*itS->second);
				}
			}
			const TypesManager::Definitionss& allDefinitions = tm.getDefinitions();
			TypesManager::Definitionss::const_iterator itD = allDefinitions.begin();
			for (; itD != allDefinitions.end(); ++itD)
			{
				gen.visit(*itD->second);
			}

			gen.postProcess();
			const CppGen::Schemas& schemas = gen.getSchemas();
			CppGen::Schemas::const_iterator itC = schemas.begin();
			bool isFirstSchema = true;
			for (; itC != schemas.end(); ++itC)
			{
				std::map<std::string, SchemaInfo>::const_iterator itAS = _schemas.find(itC->first);
				if (itAS == _schemas.end())
					throw Poco::XSD::Types::XSDException("no schema info for " + itC->first);
				
				int options = 0;
				if (itAS->second.preserveOptional())
					options |= CppWriter::OPT_PRESERVE_OPTIONAL;
				
				if (itAS->second.timestamps())
					options |= CppWriter::OPT_HEADER_TIMESTAMPS;

				if (_namespaceInSourceFileNames)
					options |= CppWriter::OPT_INCLUDE_NAMESPACE_IN_SOURCE_FILENAME;

				if (_namespaceInHeaderFileNames)
					options |= CppWriter::OPT_INCLUDE_NAMESPACE_IN_HEADER_FILENAME;

				if (_alwaysUseOptional)
					options |= CppWriter::OPT_ALWAYS_USE_OPTIONAL;

				if (_useStd)
					options |= CppWriter::OPT_USE_STD;

				if (_inlineAll)
					options |= CppWriter::OPT_INLINE_ALL;

				if (_amalgamateSources)
					options |= CppWriter::OPT_AMALGAMATE_SOURCES;

				CppWriter writer(itAS->second, options, _amalgamatedSourceFileName);
				if (_amalgamateSources && isFirstSchema)
				{
					Poco::Path p(itAS->second.sourceDir());
					p.makeDirectory();
					p.setBaseName(_amalgamatedSourceFileName);
					p.setExtension("cpp");
					std::ofstream cppStr(p.toString());
					writer.writeHeader(cppStr, p.getFileName());
				}
				writer.generate(itC->second);
				isFirstSchema = false;
			}

		}
		return Application::EXIT_OK;
	}


private:
	bool _helpRequested;
	bool _generateAllBindings;
	bool _ignoreParameterOrder;
	bool _namespaceInSourceFileNames;
	bool _namespaceInHeaderFileNames;
	bool _alwaysUseOptional;
	bool _useStd;
	bool _inlineAll;
	bool _amalgamateSources;
	std::string _amalgamatedSourceFileName;
	std::map<std::string, SchemaInfo> _schemas;
	Poco::XSD::Parser::XSDContentHandler::SchemaNSToLocationMap _schemaMap;
};


POCO_APP_MAIN(GenApp)
