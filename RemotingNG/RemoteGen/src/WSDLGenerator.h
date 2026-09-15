//
// WSDLGenerator.h
//
// Definition of the WSDLGenerator class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#ifndef RemoteGen_WSDLGenerator_INCLUDED
#define RemoteGen_WSDLGenerator_INCLUDED


#include "AbstractGenerator.h"
#include "Poco/SAX/ContentHandler.h"
#include <map>


class WSDLGenerator: public AbstractGenerator
	/// WSDLGenerator generates a wsdl file for a given class definition.
{
public:
	static const Poco::XML::XMLString DEFINITIONS;
	static const Poco::XML::XMLString SOAP_NS;
	static const Poco::XML::XMLString SOAP12_NS;
	static const Poco::XML::XMLString SOAPHTTP_NS;
	static const Poco::XML::XMLString SOAPENC_NS;
	static const Poco::XML::XMLString WSDL_NS;
	static const Poco::XML::XMLString SOAP_PREFIX;
	static const Poco::XML::XMLString SOAP12_PREFIX;
	static const Poco::XML::XMLString SOAPENC_PREFIX;
	static const Poco::XML::XMLString WSDL_PREFIX;
	static const Poco::XML::XMLString TYPE_PREFIX;
	static const Poco::XML::XMLString WSDLTYPE_PREFIX;
	static const Poco::XML::XMLString MESSAGE;
	static const Poco::XML::XMLString PART;
	static const Poco::XML::XMLString PARTS;
	static const Poco::XML::XMLString PORTTYPE;
	static const Poco::XML::XMLString PORT;
	static const Poco::XML::XMLString INPUT;
	static const Poco::XML::XMLString OUTPUT;
	static const Poco::XML::XMLString FAULT;
	static const Poco::XML::XMLString OPERATION;
	static const Poco::XML::XMLString BINDING;
	static const Poco::XML::XMLString STYLE;
	static const Poco::XML::XMLString TRANSPORT;
	static const Poco::XML::XMLString BODY;
	static const Poco::XML::XMLString PARAMETERS;
	static const Poco::XML::XMLString SOAPACTION;
	static const Poco::XML::XMLString SERVICE;
	static const Poco::XML::XMLString ADDRESS;
	static const Poco::XML::XMLString LOCATION;
	static const Poco::XML::XMLString TYPES;
	static const Poco::XML::XMLString MSG;
	static const Poco::XML::XMLString SOAPHEADER;
	static const Poco::XML::XMLString REMOTING_NS;
	static const Poco::XML::XMLString REMOTING_PREFIX;

	struct HeaderInfo
	{
		std::string message;
		std::string part;
		std::string bodyUse;
		HeaderInfo(const std::string& msg, const std::string& prt, const std::string& use): 
			message(msg),
			part(prt),
			bodyUse(use)
		{
		}
	};

	struct PortType
	{
		std::string operationName;
		std::string input;
		std::string output;
		std::multimap<Poco::UInt32, HeaderInfo> inputHeaders;
		std::multimap<Poco::UInt32, HeaderInfo> outputHeaders;
		PortType(const std::string& opName, const std::string& in, const std::string& out, const std::multimap<Poco::UInt32, HeaderInfo>& inHdr, const std::multimap<Poco::UInt32, HeaderInfo>& outHdr): 
			operationName(opName),
			input(in),
			output(out),
			inputHeaders(inHdr),
			outputHeaders(outHdr)
		{
		}
	};

	struct BindingInfo
	{
		std::string soapAction;
		PortType operationInfo;
		std::string bindingStyle;
		std::string bodyUse;

		BindingInfo(const std::string& action, const PortType& op, const std::string& bindStyle, const std::string& use):
			soapAction(action), 
			operationInfo(op), 
			bindingStyle(bindStyle), 
			bodyUse(use)
		{
		}
	};

	WSDLGenerator(Poco::CodeGeneration::CppGenerator& cppGen, const std::string& targetNamespace, Poco::XML::ContentHandler& wsdlOut);
		/// Creates the WSDLGenerator.

	~WSDLGenerator();
		/// Destroys the WSDLGenerator.

	static std::string generateFileName(const Poco::CppParser::Struct* pStruct);

	static std::string generateFileName(const std::string& structName);

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

	static bool hasSoapHeader(const Poco::CppParser::Function* pFunc, bool request);
		/// Checks whether this method has a soapHeader set. Either for request or for response

	static std::multimap<Poco::UInt32, const Poco::CppParser::Parameter*> getSoapHeader(const Poco::CppParser::Function* pFunc, bool request);
		/// Returns soapHeader sorted by order

protected:
	void writePortType(const PortType& pt);
	void writeBindingInfo(const std::string& ns, const BindingInfo& bi);
	void writeServiceLocations(const std::string& ns, const std::string& methodName, const std::string& serviceSuffix, const std::string& bindingSuffix);
	void writeSoapHeaders(const std::multimap<Poco::UInt32, HeaderInfo>& headers);
	void writeBinding(const std::string& ns, const std::string& bindingSuffix);
	void writeRemotingSchema();
	void writeExceptionMessage();

private:
	typedef std::vector<PortType> PortTypeVec;
	typedef std::vector<BindingInfo> BindingInfoVec;

	Poco::XML::ContentHandler& _wsdlOut;
	std::string _targetNamespace;
	PortTypeVec _portTypes;
	BindingInfoVec _bindingInfos;
	std::set<std::string> _functions;
};


//
// inlines
//
inline std::string WSDLGenerator::newClassName(const Poco::CppParser::Struct* pStruct)
{
	return pStruct->name();
}


#endif // RemoteGen_WSDLGenerator_INCLUDED
