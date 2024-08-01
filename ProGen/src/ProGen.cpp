//
// ProGen.cpp
//
// Visual Studio project file generator.
//
// Copyright (c) 2010-2022, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Util/Application.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/Util/AbstractConfiguration.h"
#include "Poco/Util/MapConfiguration.h"
#include "Poco/Util/PropertyFileConfiguration.h"
#include "Poco/DOM/Document.h"
#include "Poco/DOM/Element.h"
#include "Poco/DOM/NamedNodeMap.h"
#include "Poco/DOM/Attr.h"
#include "Poco/DOM/Text.h"
#include "Poco/DOM/DOMParser.h"
#include "Poco/DOM/DOMSerializer.h"
#include "Poco/DOM/DOMWriter.h"
#include "Poco/DOM/TreeWalker.h"
#include "Poco/DOM/NodeFilter.h"
#include "Poco/DOM/NodeList.h"
#include "Poco/XML/XMLWriter.h"
#include "Poco/Windows1252Encoding.h"
#include "Poco/AutoPtr.h"
#include "Poco/FileStream.h"
#include "Poco/Path.h"
#include "Poco/File.h"
#include "Poco/StringTokenizer.h"
#include "Poco/UUIDGenerator.h"
#include "Poco/UUID.h"
#include "Poco/String.h"
#include "Poco/Process.h"
#include "VSXMLWriter.h"
#include <set>
#include <iostream>


using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;
using Poco::Util::AbstractConfiguration;
using Poco::Util::OptionCallback;
using Poco::AutoPtr;
using Poco::FileInputStream;
using Poco::FileOutputStream;
using Poco::Path;
using Poco::StringTokenizer;


class ProGenApp: public Application
{
public:
	ProGenApp():
		_helpRequested(false),
		_outputDir(Poco::Path::current())
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
			Option("help", "h", "Display help information on command line arguments.")
				.required(false)
				.repeatable(false)
				.callback(OptionCallback<ProGenApp>(this, &ProGenApp::handleHelp)));

		options.addOption(
			Option("define", "D",
				"Define a configuration property. A configuration property "
				"defined with this option can be referenced in the project "
				"properties file, using the following syntax: ${<name>}.")
				.required(false)
				.repeatable(true)
				.argument("<name>=<value>")
				.callback(OptionCallback<ProGenApp>(this, &ProGenApp::handleDefine)));

		options.addOption(
			Option("output-dir", "o", "Write project files to directory <dir>.")
				.required(false)
				.repeatable(false)
				.argument("<dir>")
				.callback(OptionCallback<ProGenApp>(this, &ProGenApp::handleOutputDir)));

		options.addOption(
			Option("platform", "p", "Generate project files for specified platforms only.")
				.required(false)
				.repeatable(true)
				.argument("<platform>{,<platform>}")
				.callback(OptionCallback<ProGenApp>(this, &ProGenApp::handlePlatform)));

		options.addOption(
			Option("tool", "t", "Generate project files for specified tools only.")
				.required(false)
				.repeatable(true)
				.argument("<tool>{,<tool>}")
				.callback(OptionCallback<ProGenApp>(this, &ProGenApp::handleTool)));
	}

	void handleHelp(const std::string& name, const std::string& value)
	{
		_helpRequested = true;
		stopOptionsProcessing();
	}

	void handleDefine(const std::string& name, const std::string& value)
	{
		defineProperty(value);
	}

	void handleOutputDir(const std::string& name, const std::string& value)
	{
		_outputDir = value;
	}

	void handlePlatform(const std::string& name, const std::string& value)
	{
		Poco::StringTokenizer tok(value, ",;", Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY);
		_platforms.insert(tok.begin(), tok.end());
	}

	void handleTool(const std::string& name, const std::string& value)
	{
		Poco::StringTokenizer tok(value, ",;", Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY);
		_tools.insert(tok.begin(), tok.end());
	}

	void displayHelp()
	{
		HelpFormatter helpFormatter(options());
		helpFormatter.setCommand(commandName());
		helpFormatter.setUsage("[<option> ...] <file> ...");
		helpFormatter.setHeader(
			"\n"
			"The POCO C++ Libraries Visual Studio Project File Generator.\n"
			"Copyright (c) 2010-2024 by Applied Informatics Software Engineering GmbH.\n"
			"All rights reserved.\n\n"
			"This program generates project and solution files "
			"for Visual Studio 2019 - 2022 from global project "
			"templates and project-specific property files."
		);
		helpFormatter.setFooter(
			"For more information, please see the POCO C++ Libraries "
			"documentation at <https://pocoproject.org/docs/>."
		);
		helpFormatter.format(std::cout);
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

	Poco::AutoPtr<Poco::Util::PropertyFileConfiguration> loadProjectConfig(const Poco::Path& configPath)
	{
		Poco::AutoPtr<Poco::Util::PropertyFileConfiguration> pConfig = new Poco::Util::PropertyFileConfiguration(configPath.toString());
		pConfig->setString("vc.project.baseName", configPath.getBaseName());
		Poco::UUID nsUUID(config().getString("progen.project.guidFromName.namespaceUUID"));
		Poco::UUIDGenerator uuidGenerator;
		Poco::UUID nameUUID(uuidGenerator.createFromName(nsUUID, pConfig->getString("vc.project.name", configPath.getBaseName())));
		pConfig->setString("vc.project.guidFromName", Poco::toUpper(nameUUID.toString()));
		return pConfig;
	}

	void expandAttributes(Poco::XML::Element* pRootElement, Poco::Util::AbstractConfiguration& properties)
	{
		Poco::XML::TreeWalker walker(pRootElement, Poco::XML::NodeFilter::SHOW_ELEMENT);
		Poco::XML::Node* pNode = walker.currentNode();
		while (pNode)
		{
			Poco::AutoPtr<Poco::XML::NamedNodeMap> pAttrs = pNode->attributes();
			for (unsigned long i = 0; i < pAttrs->length(); i++)
			{
				Poco::XML::Attr* pAttr = static_cast<Poco::XML::Attr*>(pAttrs->item(i));
				std::string expandedText = properties.expand(pAttr->nodeValue());
				pAttr->setValue(expandedText);
			}
			pNode = walker.nextNode();
		}
	}

	void setProperty(Poco::Util::AbstractConfiguration& properties, const std::string& name, const Poco::Util::AbstractConfiguration& projectConfig, const std::string& projectConfigName, const std::string& platformName, const std::string& archName, const std::string& configName, const std::string& delim = ";")
	{
		std::string value = projectConfig.getString(projectConfigName, "");

		std::string configSpecificValue = projectConfig.getString(projectConfigName + "." + configName, "");
		if (!value.empty() && !configSpecificValue.empty())
		{
			value += delim;
		}
		value += configSpecificValue;

		std::string platformSpecificValue = projectConfig.getString(projectConfigName + "." + platformName, "");
		if (!value.empty() && !platformSpecificValue.empty())
		{
			value += delim;
		}
		value += platformSpecificValue;

		std::string platformArchSpecificValue = projectConfig.getString(projectConfigName + "." + platformName + "." + archName, "");
		if (!value.empty() && !platformArchSpecificValue.empty())
		{
			value += delim;
		}
		value += platformArchSpecificValue;

		std::string platformConfigSpecificValue = projectConfig.getString(projectConfigName + "." + platformName + "." + configName, "");
		if (!value.empty() && !platformConfigSpecificValue.empty())
		{
			value += delim;
		}
		value += platformConfigSpecificValue;

		std::string platformArchConfigSpecificValue = projectConfig.getString(projectConfigName + "." + platformName + "." + archName + "." + configName, "");
		if (!value.empty() && !platformArchConfigSpecificValue.empty())
		{
			value += delim;
		}
		value += platformArchConfigSpecificValue;

		properties.setString(name, value);
	}

	void fixFileConfigurations(Poco::XML::Node* pFilesElem, const std::string& configs, const std::set<std::string>& archs)
	{
		Poco::AutoPtr<Poco::XML::NodeList> pFileElems = static_cast<Poco::XML::Element*>(pFilesElem)->getElementsByTagName("File");
		for (int fileIndex = 0; fileIndex < pFileElems->length(); fileIndex++)
		{
			Poco::XML::Element* pFileElem = static_cast<Poco::XML::Element*>(pFileElems->item(fileIndex));
			Poco::XML::Element* pFileConfigElem = pFileElem->getChildElement("FileConfiguration");
			Poco::Path relativePath = pFileElem->getAttribute("RelativePath");
			if (pFileConfigElem)
			{
				Poco::AutoPtr<Poco::XML::Element> pPrototypeFileConfigElem = static_cast<Poco::XML::Element*>(pFileConfigElem->cloneNode(true));
				while (pFileConfigElem)
				{
					pFileElem->removeChild(pFileConfigElem);
					pFileConfigElem = pFileElem->getChildElement("FileConfiguration");
				}
				Poco::StringTokenizer configsTokenizer(configs, ",;", Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY);
				for (const auto& arch: archs)
				{
					for (const auto& conf: configsTokenizer)
					{
						Poco::AutoPtr<Poco::XML::Element> pNewFileConfigElem = static_cast<Poco::XML::Element*>(pPrototypeFileConfigElem->cloneNode(true));
						pNewFileConfigElem->setAttribute("Name", conf + "|" + arch);
						if (relativePath.getExtension() == "rc")
						{
							pNewFileConfigElem->setAttribute("ExcludedFromBuild", conf.find("static") != std::string::npos ? "true" : "false");
						}
						pFileElem->appendChild(pNewFileConfigElem);
					}
				}
			}
		}
	}

	void replaceFiles(Poco::XML::Node* pFilesElem, const std::string& replacements)
	{
		Poco::StringTokenizer replacementsTok(replacements, ",;", Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY);
		for (const auto& repl: replacementsTok)
		{
			Poco::StringTokenizer stmtTok(repl, ">", Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY);
			if (stmtTok.count() == 2)
			{
				std::string oldPath = stmtTok[0];
				std::string newPath = stmtTok[1];
				Poco::AutoPtr<Poco::XML::NodeList> pList = static_cast<Poco::XML::Element*>(pFilesElem)->getElementsByTagName("File");
				for (unsigned long i = 0; i < pList->length(); i++)
				{
					if (static_cast<Poco::XML::Element*>(pList->item(i))->getAttribute("RelativePath") == oldPath)
					{
						static_cast<Poco::XML::Element*>(pList->item(i))->setAttribute("RelativePath", newPath);
						break;
					}
				}
			}
		}
	}

	void generateSolution(const Poco::Path& solutionPath, const Poco::Util::AbstractConfiguration& projectConfig, const Poco::Util::AbstractConfiguration& templateProps, const std::string& platform, const std::string& tool)
	{
		std::string solutionGUID(config().getString("progen.solution.applicationGUID"));

		Poco::File solutionFile(solutionPath.toString());
		if (solutionFile.exists())
		{
			solutionFile.setWriteable(true);
		}
		Poco::FileOutputStream solutionStream(solutionPath.toString());
		if (tool == "vs160")
		{
			solutionStream << "Microsoft Visual Studio Solution File, Format Version 12.00\r\n# Visual Studio Version 16\r\n";
			generateSolution80(solutionStream, solutionPath, solutionGUID, projectConfig, templateProps, platform, tool);
		}
		else if (tool == "vs170")
		{
			solutionStream << "Microsoft Visual Studio Solution File, Format Version 12.00\r\n# Visual Studio Version 17\r\n";
			generateSolution80(solutionStream, solutionPath, solutionGUID, projectConfig, templateProps, platform, tool);
		}
	}

	void generateSolution80(std::ostream& solutionStream, const Poco::Path& solutionPath, const std::string& solutionGUID, const Poco::Util::AbstractConfiguration& projectConfig, const Poco::Util::AbstractConfiguration& templateProps, const std::string& platform, const std::string& tool)
	{
		std::vector<std::string> dependencies;
		std::string projectName = projectConfig.getString("vc.project.name", "");
		std::string projectGUID = projectConfig.getString("vc.project.guid", "");
		std::string projectPlatform = templateProps.getString("project.platform", platform);
		std::string projectSuffix = templateProps.getString("project.finalSuffix",  templateProps.getString("project.suffix"));
		bool includesHaveDependencies = projectConfig.getBool("vc.solution.fixedBuildOrder", false);
		if (!projectName.empty())
		{
			solutionStream << "Project(\"{" << solutionGUID << "}\") = \"" << projectName << "\", \"" << projectName << projectSuffix << "\", \"{" << projectGUID << "}\"\r\n";
			solutionStream << "EndProject\r\n";
			dependencies.push_back(projectGUID);
			includesHaveDependencies = true;
		}
		std::string includes = projectConfig.getString("vc.solution.include", "");
		Poco::StringTokenizer includesTokenizer(includes, ",;", Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY);
		for (const auto& incl: includesTokenizer)
		{
			Poco::Path basePath(solutionPath.parent());
			Poco::Path inclPath(basePath, Poco::Path(incl));
			inclPath.setExtension("progen");
			Poco::AutoPtr<Poco::Util::PropertyFileConfiguration> pInclConfig = loadProjectConfig(inclPath);
			projectName = pInclConfig->getString("vc.project.name");
			projectGUID = pInclConfig->getString("vc.project.guid");
			solutionStream << "Project(\"{" << solutionGUID << "}\") = \"" << projectName << "\", \"" << incl << projectSuffix << "\", \"{" << projectGUID << "}\"\r\n";
			if (includesHaveDependencies)
			{
				solutionStream << "\tProjectSection(ProjectDependencies) = postProject\r\n";
				for (const auto& dep: dependencies)
				{
					solutionStream << "\t\t{" << dep << "} = {" << dep << "}\r\n";
				}
				solutionStream << "\tEndProjectSection\r\n";
			}
			solutionStream << "EndProject\r\n";
			dependencies.push_back(projectGUID);
		}

		solutionStream << "Global\r\n";
		solutionStream << "\tGlobalSection(SolutionConfigurationPlatforms) = preSolution\r\n";

		Poco::StringTokenizer archTok(templateProps.getString("project.architectures"), ";,", Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY);
		std::set<std::string> archs(archTok.begin(), archTok.end());

		Poco::StringTokenizer configsTokenizer(projectConfig.getString("vc.project.configurations", ""), ",;", Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY);
		for (const auto& arch: archs)
		{
			for (const auto& conf: configsTokenizer)
			{
				solutionStream << "\t\t" << conf << "|" << arch << " = " << conf << "|" << arch << "\r\n";
			}
		}
		solutionStream << "\tEndGlobalSection\r\n";
		solutionStream << "\tGlobalSection(ProjectConfigurationPlatforms) = postSolution\r\n";

		for (const auto& dep: dependencies)
		{
			for (const auto& arch: archs)
			{
				for (const auto& conf: configsTokenizer)
				{
					solutionStream << "\t\t{" << dep << "}." << conf << "|" << arch << ".ActiveCfg = " << conf << "|" << arch << "\r\n";
					solutionStream << "\t\t{" << dep << "}." << conf << "|" << arch << ".Build.0 = " << conf << "|" << arch << "\r\n";
					solutionStream << "\t\t{" << dep << "}." << conf << "|" << arch << ".Deploy.0 = " << conf << "|" << arch << "\r\n";
				}
			}
		}
		solutionStream << "\tEndGlobalSection\r\n";
		solutionStream << "\tGlobalSection(SolutionProperties) = preSolution\r\n";
		solutionStream << "\t\tHideSolutionNode = FALSE\r\n";
		solutionStream << "\tEndGlobalSection\r\n";
		solutionStream << "EndGlobal\r\n";
	}

	void fix2010Project(Poco::AutoPtr<Poco::XML::Document> pProjectDoc, const std::set<std::string>& configSet, const std::string& platform, const Poco::Util::AbstractConfiguration& projectProps, const Poco::Util::AbstractConfiguration& templateProps)
	{
		std::set<std::string> validConfigs;
		std::set<std::string> validConditions;

		Poco::StringTokenizer archTok(templateProps.getString("project.architectures"), ";,", Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY);
		std::set<std::string> archs(archTok.begin(), archTok.end());

		for (const auto& arch: archs)
		{
			for (const auto& c: configSet)
			{
				std::string config = c;
				config += "|";
				config += arch;
				std::string condition("'$(Configuration)|$(Platform)'=='");
				condition += config;
				condition += "'";
				validConfigs.insert(config);
				validConditions.insert(condition);
			}
		}
		std::vector<Poco::XML::Element*> elementsToRemove;
		Poco::AutoPtr<Poco::XML::NodeList> pProjectConfigurationList = pProjectDoc->getElementsByTagName("ProjectConfiguration");
		for (unsigned long i = 0; i < pProjectConfigurationList->length(); i++)
		{
			Poco::XML::Element* pProjectConfigurationElem = static_cast<Poco::XML::Element*>(pProjectConfigurationList->item(i));
			std::string config = pProjectConfigurationElem->getAttribute("Include");
			if (validConfigs.find(config) == validConfigs.end())
			{
				elementsToRemove.push_back(pProjectConfigurationElem);
			}
		}
		Poco::AutoPtr<Poco::XML::NodeList> pPropertyGroupList = pProjectDoc->getElementsByTagName("PropertyGroup");
		for (unsigned long i = 0; i < pPropertyGroupList->length(); i++)
		{
			Poco::XML::Element* pPropertyGroupElem = static_cast<Poco::XML::Element*>(pPropertyGroupList->item(i));
			std::string cond = pPropertyGroupElem->getAttribute("Condition");
			if (!cond.empty() && validConditions.find(cond) == validConditions.end())
			{
				elementsToRemove.push_back(pPropertyGroupElem);
			}
		}
		Poco::AutoPtr<Poco::XML::NodeList> pImportGroupList = pProjectDoc->getElementsByTagName("ImportGroup");
		for (unsigned long i = 0; i < pImportGroupList->length(); i++)
		{
			Poco::XML::Element* pImportGroupElem = static_cast<Poco::XML::Element*>(pImportGroupList->item(i));
			std::string cond = pImportGroupElem->getAttribute("Condition");
			if (!cond.empty() && validConditions.find(cond) == validConditions.end())
			{
				elementsToRemove.push_back(pImportGroupElem);
			}
		}
		for (std::vector<Poco::XML::Element*>::iterator it = elementsToRemove.begin(); it != elementsToRemove.end(); ++it)
		{
			(*it)->parentNode()->removeChild(*it);
		}
		Poco::AutoPtr<Poco::XML::NodeList> pProjectFileVersionList = pProjectDoc->getElementsByTagName("_ProjectFileVersion");
		if (pProjectFileVersionList->length() > 0)
		{
			Poco::XML::Element* pPropertyGroup = static_cast<Poco::XML::Element*>(pProjectFileVersionList->item(0)->parentNode());
			for (const auto& arch: archs)
			{
				for (const auto& config: configSet)
				{
					Poco::AutoPtr<Poco::XML::Element> pTargetName = pProjectDoc->createElement("TargetName");
					pTargetName->setAttribute("Condition", Poco::format("'$(Configuration)|$(Platform)'=='%s|%s'", config, arch));
					std::string target = projectProps.getString("project.target");
					target += templateProps.getString(Poco::format("project.targetSuffix.%s.%s", config, arch), templateProps.getString(Poco::format("project.targetSuffix.%s", config), ""));
					Poco::AutoPtr<Poco::XML::Text> pText = pProjectDoc->createTextNode(target);
					pTargetName->appendChild(pText);
					pPropertyGroup->appendChild(pTargetName);
				}
			}
		}
		Poco::AutoPtr<Poco::XML::NodeList> pClCompileList = pProjectDoc->getElementsByTagName("ClCompile");
		for (unsigned long i = 0; i < pClCompileList->length(); i++)
		{
			Poco::XML::Element* pClCompileElem = static_cast<Poco::XML::Element*>(pClCompileList->item(i));
			Poco::AutoPtr<Poco::XML::Element> pMultiProcessorCompilationElem = pProjectDoc->createElement("MultiProcessorCompilation");
			Poco::AutoPtr<Poco::XML::Text> pTrueText = pProjectDoc->createTextNode("true");
			pMultiProcessorCompilationElem->appendChild(pTrueText);
			pClCompileElem->appendChild(pMultiProcessorCompilationElem);
		}
	}

	void addVcpkgProperties(Poco::AutoPtr<Poco::XML::Document> pProjectDoc, const std::set<std::string>& configSet, const std::string& platform, const Poco::Util::AbstractConfiguration& projectProps, const Poco::Util::AbstractConfiguration& templateProps)
	{
		Poco::XML::Node* pInsertAfterNode = nullptr;
		Poco::AutoPtr<Poco::XML::NodeList> pPropertyGroups = pProjectDoc->getElementsByTagName("PropertyGroup");
		if (pPropertyGroups->length() > 0)
		{
			pInsertAfterNode = pPropertyGroups->item(pPropertyGroups->length() - 1);
		}
		Poco::AutoPtr<Poco::XML::Element> pPropertyGroupElem = pProjectDoc->createElement("PropertyGroup");
		pPropertyGroupElem->setAttribute("Label", "Vcpkg");
		appendElement(pPropertyGroupElem, "VcpkgEnableManifest", "true");
		pProjectDoc->documentElement()->insertAfterNP(pPropertyGroupElem, pInsertAfterNode);
		pInsertAfterNode = pPropertyGroupElem;

		Poco::StringTokenizer archTok(templateProps.getString("project.architectures"), ";,", Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY);
		std::set<std::string> archs(archTok.begin(), archTok.end());

		for (const auto& config: configSet)
		{
			for (const auto& arch: archs)
			{
				Poco::AutoPtr<Poco::XML::Element> pPropertyGroupElem = pProjectDoc->createElement("PropertyGroup");
				pPropertyGroupElem->setAttribute("Label", "Vcpkg");
				pPropertyGroupElem->setAttribute("Condition", Poco::format("'$(Configuration)|$(Platform)'=='%s|%s'", config, arch));
				appendElement(pPropertyGroupElem, "VcpkgConfiguration", config.find("debug") == 0 ? "Debug" : "Release");
				if (config.find("_static") != std::string::npos)
				{
					appendElement(pPropertyGroupElem, "VcpkgUseStatic", "true");
				}
				if (config.find("_static_md") != std::string::npos)
				{
					appendElement(pPropertyGroupElem, "VcpkgUseMD", "true");
				}
				pProjectDoc->documentElement()->insertAfterNP(pPropertyGroupElem, pInsertAfterNode);
				pInsertAfterNode = pPropertyGroupElem;
			}
		}
	}

	void fix20XXProject(Poco::AutoPtr<Poco::XML::Document> pProjectDoc, const std::set<std::string>& configSet, const std::string& platform, const Poco::Util::AbstractConfiguration& projectProps, const Poco::Util::AbstractConfiguration& templateProps, const std::string& platformToolset)
	{
		fix2010Project(pProjectDoc, configSet, platform, projectProps, templateProps);
		Poco::AutoPtr<Poco::XML::NodeList> pConfigurationTypeList = pProjectDoc->getElementsByTagName("ConfigurationType");
		for (unsigned long i = 0; i < pConfigurationTypeList->length(); i++)
		{
			Poco::XML::Element* pConfigurationTypeElem = static_cast<Poco::XML::Element*>(pConfigurationTypeList->item(i));
			removeElement(pConfigurationTypeElem->parentNode(), "PlatformToolset");
			appendElement(pConfigurationTypeElem->parentNode(), "PlatformToolset", platformToolset);
		}
		if (projectProps.getBool("project.vcpkg", false))
		{
			addVcpkgProperties(pProjectDoc, configSet, platform, projectProps, templateProps);
		}
	}

	void fix2019Project(Poco::AutoPtr<Poco::XML::Document> pProjectDoc, const std::set<std::string>& configSet, const std::string& platform, const Poco::Util::AbstractConfiguration& projectProps, const Poco::Util::AbstractConfiguration& templateProps)
	{
		fix20XXProject(pProjectDoc, configSet, platform, projectProps, templateProps, "v142");
		Poco::AutoPtr<Poco::XML::NodeList> pLinkList = pProjectDoc->getElementsByTagName("Link");
		for (unsigned long i = 0; i < pLinkList->length(); i++)
		{
			Poco::XML::Element* pLinkElem = static_cast<Poco::XML::Element*>(pLinkList->item(i));
			Poco::XML::Element* pItemDefinitionGroupElem = static_cast<Poco::XML::Element*>(pLinkElem->parentNode());
			Poco::XML::XMLString condition = pItemDefinitionGroupElem->getAttribute("Condition");
			if (Poco::endsWith(condition, Poco::XML::XMLString("ARM64'")))
			{
				appendElement(pLinkElem, "TargetMachine", "MachineARM64");
			}
		}
		Poco::AutoPtr<Poco::XML::NodeList> pClCompileList = pProjectDoc->getElementsByTagName("ClCompile");
		for (unsigned long i = 0; i < pClCompileList->length(); i++)
		{
			Poco::XML::Element* pClCompileElem = static_cast<Poco::XML::Element*>(pClCompileList->item(i));
			Poco::AutoPtr<Poco::XML::Element> pLanguageStandard = pProjectDoc->createElement("LanguageStandard");
			Poco::AutoPtr<Poco::XML::Text> pStandardText = pProjectDoc->createTextNode("stdcpp17");
			pLanguageStandard->appendChild(pStandardText);
			pClCompileElem->appendChild(pLanguageStandard);

			pClCompileElem = static_cast<Poco::XML::Element*>(pClCompileList->item(i));
			pLanguageStandard = pProjectDoc->createElement("LanguageStandard_C");
			pStandardText = pProjectDoc->createTextNode("stdc11");
			pLanguageStandard->appendChild(pStandardText);
			pClCompileElem->appendChild(pLanguageStandard);
		}
	}

	void fix2022Project(Poco::AutoPtr<Poco::XML::Document> pProjectDoc, const std::set<std::string>& configSet, const std::string& platform, const Poco::Util::AbstractConfiguration& projectProps, const Poco::Util::AbstractConfiguration& templateProps)
	{
		fix20XXProject(pProjectDoc, configSet, platform, projectProps, templateProps, "v143");
		Poco::AutoPtr<Poco::XML::NodeList> pLinkList = pProjectDoc->getElementsByTagName("Link");
		for (unsigned long i = 0; i < pLinkList->length(); i++)
		{
			Poco::XML::Element* pLinkElem = static_cast<Poco::XML::Element*>(pLinkList->item(i));
			Poco::XML::Element* pItemDefinitionGroupElem = static_cast<Poco::XML::Element*>(pLinkElem->parentNode());
			Poco::XML::XMLString condition = pItemDefinitionGroupElem->getAttribute("Condition");
			if (Poco::endsWith(condition, Poco::XML::XMLString("ARM64'")))
			{
				appendElement(pLinkElem, "TargetMachine", "MachineARM64");
			}
		}
		Poco::AutoPtr<Poco::XML::NodeList> pClCompileList = pProjectDoc->getElementsByTagName("ClCompile");
		for (unsigned long i = 0; i < pClCompileList->length(); i++)
		{
			Poco::XML::Element* pClCompileElem = static_cast<Poco::XML::Element*>(pClCompileList->item(i));
			Poco::AutoPtr<Poco::XML::Element> pLanguageStandard = pProjectDoc->createElement("LanguageStandard");
			Poco::AutoPtr<Poco::XML::Text> pStandardText = pProjectDoc->createTextNode("stdcpp17");
			pLanguageStandard->appendChild(pStandardText);
			pClCompileElem->appendChild(pLanguageStandard);

			pClCompileElem = static_cast<Poco::XML::Element*>(pClCompileList->item(i));
			pLanguageStandard = pProjectDoc->createElement("LanguageStandard_C");
			pStandardText = pProjectDoc->createTextNode("stdc11");
			pLanguageStandard->appendChild(pStandardText);
			pClCompileElem->appendChild(pLanguageStandard);
		}
	}

	void appendElement(Poco::XML::Node* pParentNode, const std::string& elemName, const std::string& text)
	{
		Poco::AutoPtr<Poco::XML::Element> pElement = pParentNode->ownerDocument()->createElement(elemName);
		Poco::AutoPtr<Poco::XML::Text> pText = pParentNode->ownerDocument()->createTextNode(text);
		pElement->appendChild(pText);
		pParentNode->appendChild(pElement);
	}

	void removeElement(Poco::XML::Node* pParentNode, const std::string& elemName)
	{
		Poco::XML::Node* pNode = pParentNode->getNodeByPath(elemName);
		if (pNode)
		{
			pParentNode->removeChild(pNode);
		}
	}

	void writeProject(Poco::AutoPtr<Poco::XML::Document> pProjectDoc, const std::string& path)
	{
		Poco::XML::DOMWriter writer;
		writer.setOptions(Poco::XML::XMLWriter::PRETTY_PRINT | Poco::XML::XMLWriter::WRITE_XML_DECLARATION);
		writer.setNewLine(Poco::XML::XMLWriter::NEWLINE_CRLF);
		writer.setIndent("  ");
		writer.writeNode(path, pProjectDoc);
	}

	void generateProject(const Poco::Util::AbstractConfiguration& projectConfig, const Poco::Path& projectPath, const Poco::Path& templatePath, const std::string& platform, const std::string& tool)
	{
		Poco::File templateDir(templatePath);
		if (templateDir.exists())
		{
			logger().information("Generating using templates from: " + templatePath.toString());

			Poco::Path projectTemplatePath(templatePath);
			projectTemplatePath.setFileName("project.template");

			Poco::Path templatePropsPath(templatePath);
			templatePropsPath.setFileName("project.properties");
			Poco::AutoPtr<Poco::Util::PropertyFileConfiguration> pTemplateProps = new Poco::Util::PropertyFileConfiguration(templatePropsPath.toString());

			if (projectConfig.hasProperty("vc.project.name"))
			{
				Poco::StringTokenizer archTok(pTemplateProps->getString("project.architectures"), ";,", Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY);
				std::set<std::string> archs(archTok.begin(), archTok.end());

				Poco::Path prototypePath(projectPath);
				prototypePath.setFileName(projectConfig.getString("vc.project.prototype"));

				Poco::XML::DOMParser domParser;
				domParser.setFeature(Poco::XML::XMLReader::FEATURE_NAMESPACES, false);
				domParser.setFeature(Poco::XML::XMLReader::FEATURE_NAMESPACE_PREFIXES, false);
				domParser.setFeature(Poco::XML::DOMParser::FEATURE_FILTER_WHITESPACE, true);
				Poco::AutoPtr<Poco::XML::Document> pPrototypeDoc = domParser.parse(prototypePath.toString());
				Poco::AutoPtr<Poco::XML::Document> pProjectDoc = domParser.parse(projectTemplatePath.toString());

				Poco::AutoPtr<Poco::Util::MapConfiguration> pProps = new Poco::Util::MapConfiguration;
				pProps->setString("project.guid", projectConfig.getString("vc.project.guid"));
				pProps->setString("project.name", projectConfig.getString("vc.project.name"));
				pProps->setString("project.target", projectConfig.getString("vc.project.target", projectConfig.getString("vc.project.name")));
				pProps->setString("project.outdir", projectConfig.getString("vc.project.outdir", "."));
				pProps->setString("project.pocobase", projectConfig.getString("vc.project.pocobase", ".."));
				pProps->setString("project.platform", pTemplateProps->getString("project.platform", platform));
				pProps->setString("project.targetPlatform", pTemplateProps->getString("project.targetPlatform", "WINDOWS_NT"));
				pProps->setBool("project.vcpkg", projectConfig.getBool("vc.project.vcpkg", false));
				expandAttributes(pProjectDoc->documentElement(), *pProps);

				Poco::XML::Node* pFilesElement = pPrototypeDoc->getNodeByPath("//Files");
				if (!pFilesElement) throw Poco::NotFoundException("No Files element found in prototype document");
				pFilesElement = pProjectDoc->importNode(pFilesElement, true);

				fixFileConfigurations(pFilesElement, projectConfig.getString("vc.project.configurations", ""), archs);
				replaceFiles(pFilesElement, pTemplateProps->getString("project.replaceSourceFiles", ""));

				Poco::XML::Node* pOldFilesElement = pProjectDoc->getNodeByPath("//Files");
				if (!pOldFilesElement) throw Poco::NotFoundException("No Files element found in project document template");

				Poco::XML::Node* pConfigurationsElement = pProjectDoc->getNodeByPath("//Configurations");
				if (!pConfigurationsElement) throw Poco::NotFoundException("No Configurations element found in project document template");

				pProjectDoc->documentElement()->replaceChild(pFilesElement, pOldFilesElement);

				std::set<std::string> configSet;
				Poco::StringTokenizer configs(projectConfig.getString("vc.project.configurations", ""), ",;", Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY);
				for (const auto& arch: archs)
				{
					pProps->setString("project.targetArchitecture", pTemplateProps->getString("project.targetArchitecture." + arch, "IA32"));
					for (const auto& config: configs)
					{
						configSet.insert(config);
						setProperty(*pProps, "configuration.compiler.includes", projectConfig, "vc.project.compiler.include", platform, arch, config);
						setProperty(*pProps, "configuration.compiler.defines", projectConfig, "vc.project.compiler.defines", platform, arch, config);
						setProperty(*pProps, "configuration.compiler.disableWarnings", projectConfig, "vc.project.compiler.disableWarnings", platform, arch, config);
						setProperty(*pProps, "configuration.compiler.additionalOptions", projectConfig, "vc.project.compiler.additionalOptions", platform, arch, config, " ");
						setProperty(*pProps, "configuration.linker.dependencies", projectConfig, "vc.project.linker.dependencies", platform, arch, config, " ");
						setProperty(*pProps, "configuration.linker.libraries", projectConfig, "vc.project.linker.libraries", platform, arch, config);
						setProperty(*pProps, "configuration.linker.entry", projectConfig, "vc.project.linker.entry", platform, arch, config);
						setProperty(*pProps, "configuration.linker.additionalOptions", projectConfig, "vc.project.linker.additionalOptions", platform, arch, config, " ");
						setProperty(*pProps, "configuration.prebuild", projectConfig, "vc.project.prebuild", platform, arch, config);
						setProperty(*pProps, "configuration.postbuild", projectConfig, "vc.project.postbuild", platform, arch, config);
						std::string libSuffix = this->config().getString("progen.libsuffix." + config, "");
						Poco::StringTokenizer rawDependencies(pProps->getString("configuration.linker.dependencies"), " ", Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY);
						std::string expandedDependencies;
						for (Poco::StringTokenizer::Iterator itd = rawDependencies.begin(); itd != rawDependencies.end(); ++itd)
						{
							std::string lib(*itd);
							if (lib.find('.') == std::string::npos)
							{
								lib += libSuffix;
							}
							if (!expandedDependencies.empty()) expandedDependencies += ' ';
							expandedDependencies += lib;
						}
						pProps->setString("configuration.linker.dependencies", expandedDependencies);

						Poco::Path configPath(templatePath);
						configPath.setBaseName(config + "-" + arch);
						configPath.setExtension("template");
						Poco::AutoPtr<Poco::XML::Document> pConfigDoc = domParser.parse(configPath.toString());
						Poco::XML::Element* pConfigElem = pConfigDoc->documentElement();

						std::string prebuild = pProps->getString("configuration.prebuild", "");
						if (!prebuild.empty())
						{
							Poco::XML::Node* pPreBuildNode = pConfigElem->getNodeByPath("Tool[@Name=VCPreBuildEventTool]");
							if (pPreBuildNode)
							{
								static_cast<Poco::XML::Element*>(pPreBuildNode)->setAttribute("CommandLine", prebuild);
							}
						}

						std::string postbuild = pProps->getString("configuration.postbuild", "");
						if (!postbuild.empty())
						{
							Poco::XML::Node* pPostBuildNode = pConfigElem->getNodeByPath("Tool[@Name=VCPostBuildEventTool]");
							if (pPostBuildNode)
							{
								static_cast<Poco::XML::Element*>(pPostBuildNode)->setAttribute("CommandLine", postbuild);
							}
						}

						expandAttributes(pConfigElem, *pProps);
						pConfigElem = static_cast<Poco::XML::Element*>(pProjectDoc->importNode(pConfigElem, true));
						pConfigurationsElement->appendChild(pConfigElem);
					}
				}

				std::string vcprojName = projectConfig.getString("vc.project.name");
				vcprojName.append(pTemplateProps->getString("project.suffix", ""));
				Poco::Path vcprojPath(projectPath);
				vcprojPath.setFileName(vcprojName);

				Poco::File vcprojFile(vcprojPath.toString());
				if (vcprojFile.exists() && config().getBool("progen.backupProjectFile", false))
				{
					Poco::File backupFile(vcprojPath.toString() + ".bak");
					if (backupFile.exists())
					{
						backupFile.setWriteable(true);
						backupFile.remove();
					}
					vcprojFile.renameTo(backupFile.path());
				}
				else if (vcprojFile.exists())
				{
					vcprojFile.setWriteable(true);
				}

				Poco::FileOutputStream vcprojStream(vcprojPath.toString());
				VSXMLWriter writer(vcprojStream, pTemplateProps->getBool("project.convertBoolToUpperCase", false));
				Poco::XML::DOMSerializer serializer;
				serializer.setContentHandler(&writer);
				serializer.serialize(pProjectDoc);
				vcprojStream.close();

				std::string postprocess = pTemplateProps->getString("project.postprocess", "");
				if (!postprocess.empty())
				{
					logger().information("Postprocessing project file: " + vcprojPath.toString());
					std::string tool = config().getString("progen.postprocess." + postprocess + ".tool", "");
					if (!tool.empty())
					{
						Poco::Process::Args args;
						Poco::StringTokenizer argsTokenizer(config().getString("progen.postprocess." + postprocess + ".args", ""), ";,", Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY);
						for (const auto& arg: argsTokenizer)
						{
							if (arg == "%")
								args.push_back("\"" + vcprojPath.toString() + "\"");
							else
								args.push_back(arg);
						}
						Poco::Path vcxprojPath(vcprojPath);
						vcxprojPath.setExtension("vcxproj");
						Poco::File projectFile(vcxprojPath.toString());
						if (projectFile.exists())
						{
							projectFile.remove();
						}
						Poco::File projectUserFile(vcxprojPath.toString() + ".user");
						if (projectUserFile.exists())
						{
							projectUserFile.remove();
						}
						Poco::File projectFiltersFile(vcxprojPath.toString() + ".filters");
						if (projectFiltersFile.exists())
						{
							projectFiltersFile.remove();
						}
						Poco::ProcessHandle ph = Poco::Process::launch(tool, args);
						ph.wait();

						if (config().getBool("progen.postprocess." + postprocess + ".fix2019ProjectFile", false))
						{
							if (projectFile.exists())
							{
								logger().information("Fixing Visual Studio 2019 project file: " + vcxprojPath.toString());
								Poco::AutoPtr<Poco::XML::Document> pProjectDoc = domParser.parse(vcxprojPath.toString());
								fix2019Project(pProjectDoc, configSet, pTemplateProps->getString("project.platform", platform), *pProps, *pTemplateProps);
								writeProject(pProjectDoc, vcxprojPath.toString());
							}
						}
						if (config().getBool("progen.postprocess." + postprocess + ".fix2022ProjectFile", false))
						{
							if (projectFile.exists())
							{
								logger().information("Fixing Visual Studio 2022 project file: " + vcxprojPath.toString());
								Poco::AutoPtr<Poco::XML::Document> pProjectDoc = domParser.parse(vcxprojPath.toString());
								fix2022Project(pProjectDoc, configSet, pTemplateProps->getString("project.platform", platform), *pProps, *pTemplateProps);
								writeProject(pProjectDoc, vcxprojPath.toString());
							}
						}
						if (config().getBool("progen.postprocess." + postprocess + ".deleteOriginalFile", false))
						{
							Poco::File projectFile(vcprojPath.toString());
							projectFile.remove();
						}
						Poco::StringTokenizer filesTokenizer(config().getString("progen.postprocess." + postprocess + ".deleteFiles", ""), ";,", Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY);
						for (const auto& file: filesTokenizer)
						{
							Poco::Path p(vcprojPath);
							p.setFileName(file);
							Poco::File f(p.toString());
							if (f.exists())
							{
								try
								{
									f.remove(true);
								}
								catch (Poco::Exception& exc)
								{
									logger().log(exc);
								}
							}
						}
					}
					else
					{
						logger().warning("No postprocessing tool defined.");
					}
				}
			}
			if (projectConfig.getBool("vc.solution.create", false))
			{
				std::string solutionName = projectConfig.getString("vc.project.name", projectConfig.getString("vc.project.baseName"));
				solutionName.append(pTemplateProps->getString("project.suffix", ""));
				Poco::Path solutionPath(projectPath);
				solutionPath.setFileName(solutionName);
				solutionPath.setExtension("sln");
				logger().information("Creating solution file " + solutionPath.toString());
				generateSolution(solutionPath, projectConfig, *pTemplateProps, platform, tool);
			}
		}
		else
		{
			logger().information("Template directory missing: " + templatePath.toString());
		}
	}

	void process(const std::string& configPath)
	{
		Poco::Path projectPath(configPath);
		Poco::AutoPtr<Poco::Util::PropertyFileConfiguration> pProjectConfig = loadProjectConfig(projectPath);

		Poco::StringTokenizer platforms(pProjectConfig->getString("vc.project.platforms", ""), ",;", Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY);

		Poco::Path templateBasePath(config().getString("progen.templatePath"));
		templateBasePath.makeDirectory();

		Poco::File templateDir(templateBasePath);
		std::vector<std::string> tools;
		templateDir.list(tools);

		for (const auto& tool: tools)
		{
			if (_tools.empty() || _tools.find(tool) != _tools.end())
			{
				for (const auto& platform: platforms)
				{
					if (_platforms.empty() || _platforms.find(platform) != _platforms.end())
					{
						std::string projectType = pProjectConfig->getString("vc.project.type", "executable");
						Poco::Path templatePath(templateBasePath);
						templatePath.pushDirectory(tool);
						templatePath.pushDirectory(platform);
						templatePath.pushDirectory(projectType);
						generateProject(*pProjectConfig, projectPath, templatePath, platform, tool);
					}
				}
			}
		}
	}

	int main(const std::vector<std::string>& args)
	{
		if (_helpRequested || args.empty())
		{
			displayHelp();
			return Application::EXIT_OK;
		}

		for (std::vector<std::string>::const_iterator it = args.begin(); it != args.end(); ++it)
		{
			process(*it);
		}

		return Application::EXIT_OK;
	}

private:
	bool _helpRequested;
	std::string _outputDir;
	std::set<std::string> _platforms;
	std::set<std::string> _architectures;
	std::set<std::string> _tools;
};


POCO_APP_MAIN(ProGenApp)
