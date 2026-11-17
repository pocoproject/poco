#include "Poco/AutoPtr.h"
#include "Poco/DOM/DOMParser.h"
#include "Poco/DOM/Document.h"
#include "Poco/SAX/DefaultHandler.h"
#include "Poco/SAX/EntityResolver.h"
#include "Poco/SAX/InputSource.h"
#include "Poco/SAX/SAXParser.h"
#include "Poco/SAX/XMLReader.h"
#include "Poco/XML/XMLStreamParser.h"
#include "Poco/XML/XMLString.h"
#include <cstddef>
#include <cstdint>
#include <exception>
#include <memory>
#include <sstream>
#include <string>


using namespace Poco;
using namespace Poco::XML;


namespace
{
	class EmptyEntityResolver: public EntityResolver
		/// Resolves every external entity, including the external DTD subset
		/// and external parameter entities, to an empty in-memory stream and
		/// never returns null, so that a parser that uses it does not fall back
		/// to EntityResolverImpl, which opens the file or URI that the document
		/// names.
	{
	public:
		InputSource* resolveEntity(const XMLString* publicId, const XMLString& systemId) override
		{
			auto pStream = std::make_unique<std::istringstream>();
			auto pSource = std::make_unique<InputSource>(systemId);
			if (publicId != nullptr) pSource->setPublicId(*publicId);
			pSource->setByteStream(*pStream.release());
			return pSource.release();
		}

		void releaseInputSource(InputSource* pSource) override
		{
			delete pSource->getByteStream();
			delete pSource;
		}
	};
}


extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
	std::string xml(reinterpret_cast<const char*>(data), size);

	EmptyEntityResolver resolver;

	// SAX Parser

	SAXParser saxParser;
	std::uint8_t saxFeatures = size > 0 ? data[size - 1] : 0;

	DefaultHandler defHandler;
	saxParser.setContentHandler(&defHandler);
	saxParser.setDTDHandler(&defHandler);
	saxParser.setErrorHandler(&defHandler);
	saxParser.setEntityResolver(&resolver);

	for (const auto& feature : {
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
	domParser.setEntityResolver(&resolver);

	for (const auto& feature : {
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
			// Reading the attribute map marks the attributes as handled;
			// otherwise the parser throws at the first end tag of an element
			// that has attributes.
			if (e == XMLStreamParser::EV_START_ELEMENT) (void) streamParser.attributeMap();
			(void) streamParser.getQName().toString();
		}
	}
	catch (const std::exception&)
	{
	}

	return 0;
}
