//
// XSDGenerator.h
//
// Definition of the XSDGenerator class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#ifndef RemoteGen_XSDGenerator_INCLUDED
#define RemoteGen_XSDGenerator_INCLUDED


#include "AbstractGenerator.h"
#include "Poco/SAX/ContentHandler.h"
#include "Poco/XML/XMLString.h"
#include <map>


class XSDGenerator: public AbstractGenerator
	/// XSDGenerator generates a xsd file for a given class definition.
{
public:
	static const Poco::XML::XMLString SCHEMA_NS;
	static const Poco::XML::XMLString SCHEMALOCATION;
	static const Poco::XML::XMLString TARGETNAMESPACE;
	static const Poco::XML::XMLString SCHEMA;
	static const Poco::XML::XMLString XMLNS;
	static const Poco::XML::XMLString EMPTYSTRING;
	static const Poco::XML::XMLString COMPLEXTYPE;
	static const Poco::XML::XMLString ELEMENT;
	static const Poco::XML::XMLString SEQUENCE;
	static const Poco::XML::XMLString NAME;
	static const Poco::XML::XMLString TYPE;
	static const Poco::XML::XMLString NILLABLE;
	static const Poco::XML::XMLString MINOCCURS;
	static const Poco::XML::XMLString MAXOCCURS;
	static const Poco::XML::XMLString UNBOUNDED;
	static const Poco::XML::XMLString COMPLEXCONTENT;
	static const Poco::XML::XMLString BASE;
	static const Poco::XML::XMLString EXTENSION;
	static const Poco::XML::XMLString SCHEMA_PREFIX;
	static const Poco::XML::XMLString USE;
	static const Poco::XML::XMLString REQUIRED;
	static const Poco::XML::XMLString ATTRIBUTE;
	static const Poco::XML::XMLString ELEMENTFORMDEFAULT;
	static const Poco::XML::XMLString REQUEST;
	static const Poco::XML::XMLString HEADER;

	XSDGenerator(Poco::CodeGeneration::CppGenerator& cppGen, const std::string& targetNamespace, Poco::XML::ContentHandler& xsdOut, bool documentStarted = false);
	/// Creates the XSDGenerator. Set documentStarted to true, if the XSD file is written as a sub document of another files (ie WSDL file)

	~XSDGenerator();
		/// Destroys the XSDGenerator.

	static std::string generateFileName(const Poco::CppParser::Struct* pStruct);

	static std::string generateFileName(const std::string& structName);

	static std::string generateComplexTypeName(const Poco::CppParser::Struct* pStruct);

	void structStart(const Poco::CppParser::Struct* pStruct, const CodeGenerator::Properties& properties);
		/// Struct Start

	void structEnd();
		// Struct End

	void methodStart(const Poco::CppParser::Function* pFunc, const CodeGenerator::Properties& properties);
		/// Method Start

	void methodEnd(const Poco::CppParser::Function* pFunc, const CodeGenerator::Properties& properties);
		/// Method End

	void registerCallbacks(Poco::CodeGeneration::GeneratorEngine& e);

	std::string newClassName(const Poco::CppParser::Struct* pStruct);
		/// generates the new class name based on the old one

	std::vector<std::string> newBaseClasses(const Poco::CppParser::Struct* pStruct);

	static const std::string& mapToSchemaType(const std::string& type);

	void detectProperties(const Poco::CppParser::Variable* pVar, bool& isVector, bool& isMandatory, bool& isNullable, std::string& name, std::string& resolvedType);

	static bool isOutParameter(const Poco::CppParser::Parameter* pParam);

protected:
	static std::map<std::string, std::string> initTypeMapping();

	std::string generateComplexType(const Poco::CppParser::Struct* pType);
		/// Returns the name of the generated type

	void detectTypes(const Poco::CppParser::Function* pFuncOld, std::vector<const Poco::CppParser::Struct*>& detectedTypes);

	static void detectOutParams(const Poco::CppParser::Function* pFuncOld, std::vector<const Poco::CppParser::Parameter*>& outParams);

	void generateTypes(const std::vector<const Poco::CppParser::Struct*>& detectedTypes);

	Poco::CppParser::Struct* convertToStruct(const Poco::CppParser::Function* pFuncOld, bool isResponse);
		/// Creates for parameters of the function a struct with get/set methods. If isResponse is set to true only outParams+return parameter is included.

	Poco::CppParser::Parameter* createParameter(const Poco::CppParser::Parameter* pCopy, Poco::CppParser::Function* pConstructorFull);
		/// Creates a parameter form pCopy, adds it to the constructor.

	std::string createParameterTypeDecl(const std::string& typeDecl);
		/// if typeDecl contains a primitive type, we return the typeDecl as value type, otherwise as const& type

	Poco::CppParser::Variable* createVariable(const Poco::CppParser::Parameter* pParam, Poco::CppParser::Struct* pStruct);

	Poco::CppParser::Function* createGetFct(const Poco::CppParser::Parameter* pParam, Poco::CppParser::Struct* pStruct);

	Poco::CppParser::Function* createSetFct(const Poco::CppParser::Parameter* pParam, Poco::CppParser::Struct* pStruct);

	void addVarDocu(Poco::CppParser::Variable* pVar, const Poco::CppParser::Function* pFunc, const std::string& paramName, Poco::UInt32 order);

	void exportElements();

	void deepCopyMembers(const Poco::CppParser::Struct* pIn, Poco::CppParser::Struct* pOut);

private:
	typedef std::set<std::string> HandledTypes;
	typedef std::map<std::string, std::string> ExportedTypes;
	typedef std::map<std::string, std::string> SoapHeaderTypes;
		// maps element names to type names

	Poco::CppParser::NameSpace _ns;
	Poco::XML::ContentHandler& _xsdOut;
	std::string          _targetNamespace;
	bool                 _documentStarted;
	HandledTypes         _handledTypes;
	ExportedTypes        _exportElementTypes;
	SoapHeaderTypes      _exportSoapHeaderTypes;
};


//
// inlines
//
inline std::string XSDGenerator::newClassName(const Poco::CppParser::Struct* pStruct)
{
	return pStruct->name();
}


#endif // RemoteGen_XSDGenerator_INCLUDED
