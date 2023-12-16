//
// SAXParser.h
//
// Library: XML
// Package: SAX
// Module:  SAX
//
// Implementation of the XMLReader interface.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SAX_SAXParser_INCLUDED
#define SAX_SAXParser_INCLUDED


#include "Poco/XML/XML.h"
#include "Poco/SAX/XMLReader.h"
#include "Poco/XML/ParserEngine.h"


namespace Poco {
namespace XML {


class XML_API SAXParser: public XMLReader
	/// This class provides a SAX2 (Simple API for XML) interface to expat,
	/// the XML parser toolkit.
	/// The following SAX2 features and properties are supported:
	///   * http://xml.org/sax/features/external-general-entities
	///   * http://xml.org/sax/features/external-parameter-entities
	///   * http://xml.org/sax/features/namespaces
	///   * http://xml.org/sax/features/namespace-prefixes
	///   * http://xml.org/sax/properties/lexical-handler
	///   * http://xml.org/sax/properties/declaration-handler
	///
	/// The following proprietary extensions are supported:
	///   * http://www.appinf.com/features/enable-partial-reads --
	///     see ParserEngine::setEnablePartialReads()
	///   * http://www.appinf.com/properties/bla-maximum-amplification
	///     see ParserEngine::setBillionLaughsAttackProtectionMaximumAmplification();
	///     argument must be a float >= 1.0 formatted as string;
	///     property is set-only.
	///   * http://www.appinf.com/properties/bla-activation-threshold
	///     see ParserEngine::setBillionLaughsAttackProtectionActivationThreshold();
	///     argument must be a 64-bit unsigned integer formatted as string;
	///     property is set-only.
{
public:
	SAXParser();
		/// Creates an SAXParser.

	SAXParser(const XMLString& encoding);
		/// Creates an SAXParser with the given encoding.

	~SAXParser() override;
		/// Destroys the SAXParser.

	void setEncoding(const XMLString& encoding);
		/// Sets the encoding used by the parser if no
		/// encoding is specified in the XML document.

	const XMLString& getEncoding() const;
		/// Returns the name of the encoding used by
		/// the parser if no encoding is specified in
		/// the XML document.

	void addEncoding(const XMLString& name, Poco::TextEncoding* pEncoding);
		/// Adds an encoding to the parser. Does not take ownership of the pointer!

	/// XMLReader
	void setEntityResolver(EntityResolver* pResolver) override;
	EntityResolver* getEntityResolver() const override;
	void setDTDHandler(DTDHandler* pDTDHandler) override;
	DTDHandler* getDTDHandler() const override;
	void setContentHandler(ContentHandler* pContentHandler) override;
	ContentHandler* getContentHandler() const override;
	void setErrorHandler(ErrorHandler* pErrorHandler) override;
	ErrorHandler* getErrorHandler() const override;
	void setFeature(const XMLString& featureId, bool state) override;
	bool getFeature(const XMLString& featureId) const override;
	void setProperty(const XMLString& propertyId, const XMLString& value) override;
	void setProperty(const XMLString& propertyId, void* value) override;
	void* getProperty(const XMLString& propertyId) const override;
	void parse(InputSource* pSource) override;
	void parse(const XMLString& systemId) override;
	void parseMemoryNP(const char* xml, std::size_t size) override;

	/// Extensions
	void parseString(const std::string& xml);

	static const XMLString FEATURE_PARTIAL_READS;
	static const XMLString PROPERTY_BLA_MAXIMUM_AMPLIFICATION;
	static const XMLString PROPERTY_BLA_ACTIVATION_THRESHOLD;

protected:
	void setupParse();

private:
	ParserEngine _engine;
	bool _namespaces;
	bool _namespacePrefixes;
};


} } // namespace Poco::XML


#endif // SAX_SAXParser_INCLUDED
