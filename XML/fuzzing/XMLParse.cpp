#include "Poco/AutoPtr.h"
#include "Poco/DOM/DOMParser.h"
#include "Poco/DOM/Document.h"
#include "Poco/SAX/DefaultHandler.h"
#include "Poco/SAX/SAXParser.h"
#include "Poco/XML/XMLStreamParser.h"

using namespace Poco;
using namespace Poco::XML;

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
	std::string xml(reinterpret_cast<const char*>(data), size);

	// SAX Parser

	SAXParser saxParser;
	std::uint8_t saxFeatures = size > 0 ? data[size - 1] : 0;

	DefaultHandler defHandler;
	saxParser.setContentHandler(&defHandler);
	saxParser.setDTDHandler(&defHandler);
	saxParser.setErrorHandler(&defHandler);
	saxParser.setEntityResolver(&defHandler);

	for (const auto feature : {
			 XMLReader::FEATURE_EXTERNAL_GENERAL_ENTITIES,
			 XMLReader::FEATURE_EXTERNAL_PARAMETER_ENTITIES,
			 XMLReader::FEATURE_NAMESPACES,
			 XMLReader::FEATURE_NAMESPACE_PREFIXES,
			 SAXParser::FEATURE_PARTIAL_READS,
		 })
	{
		saxParser.setFeature(feature, saxFeatures & 0x01);
		saxFeatures >>= 1;
	}

	try
	{
		saxParser.parseString(xml);
	}
	catch (const std::exception&)
	{
	}

	// DOM Parser

	DOMParser domParser;
	std::uint8_t domFeatures = size > 0 ? data[size - 1] : 0;

	for (const auto feature : {
			 XMLReader::FEATURE_EXTERNAL_GENERAL_ENTITIES,
			 XMLReader::FEATURE_EXTERNAL_PARAMETER_ENTITIES,
			 XMLReader::FEATURE_NAMESPACES,
			 XMLReader::FEATURE_NAMESPACE_PREFIXES,
			 DOMParser::FEATURE_FILTER_WHITESPACE,
		 })
	{
		domParser.setFeature(feature, domFeatures & 0x01);
		domFeatures >>= 1;
	}

	try
	{
		Poco::AutoPtr<Document> doc = domParser.parseString(xml);
	}
	catch (const std::exception&)
	{
	}

	// Stream Parser

	std::istringstream stream(xml);

	try
	{
		XMLStreamParser streamParser(stream, "fuzz");
		for (XMLStreamParser::EventType e : streamParser)
		{
			streamParser.getQName().toString();
		}
	}
	catch (const std::exception&)
	{
	}

	return 0;
}
