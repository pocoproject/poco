//
// Serializer.h
//
// Library: RemotingNG/SOAP
// Package: SOAP
// Module:  Serializer
//
// Definition of the Serializer class.
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_SOAP_Serializer_INCLUDED
#define RemotingNG_SOAP_Serializer_INCLUDED


#include "Poco/RemotingNG/SOAP/SOAP.h"
#include "Poco/RemotingNG/SOAP/XMLElement.h"
#include "Poco/RemotingNG/Serializer.h"
#include "Poco/XML/XMLWriter.h"
#include "Poco/SAX/AttributesImpl.h"
#include "Poco/Net/MultipartWriter.h"
#include "Poco/NumberFormatter.h"
#include "Poco/SharedPtr.h"
#include <cstddef>
#include <memory>
#include <map>


namespace Poco::RemotingNG::SOAP {


class RemotingNGSOAP_API Serializer: public Poco::RemotingNG::Serializer
	/// The Serializer implementation for the SOAP
	/// Transport.
	///
	/// The serializer creates an XML stream according
	/// to SOAP 1.1/1.2 Document/Literal message format.
	///
	/// The serializer can also be used to serialize plain
	/// XML. To do so, instead of serializeMessageBegin() and
	/// serializeMessageEnd(), call serializeStructBegin()
	/// and serializeStructEnd(), respectively, at the root level,
	/// after calling setup().
{
public:
	static const std::string SOAPPREFIX;
	static const std::string SOAPENCPREFIX;
	static const std::string SOAP11NAMESPACE;
	static const std::string SOAP12NAMESPACE;
	static const std::string REMOTINGPREFIX;
	static const std::string REMOTINGNAMESPACE;
	static const std::string XSIPREFIX;
	static const std::string XSINAMESPACE;
	static const std::string XSINIL;
	static const std::string XOPNAMESPACE;
	static const std::string XOPINCLUDE;
	static const std::string XOPHREF;
	static const std::string EMPTYSTRING;
	static const XML::AttributesImpl EMPTYATTRIBUTES;
	static const std::string VAL_TRUE;
	static const std::string VAL_FALSE;
	static const std::string ENVELOPE;
	static const std::string FAULT;
	static const std::string ENCODINGSTYLE;
	static const std::string FAULTCODE;
	static const std::string FAULTCODESERVER;
	static const std::string FAULTSTRING;
	static const std::string DETAIL;
	static const std::string CODE;
	static const std::string CODE_VALUE;
	static const std::string REASON;
	static const std::string REASON_TEXT;
	static const std::string DETAIL12;
	static const std::string DETAIL_EXCEPTION;
	static const std::string DETAIL_TYPE;
	static const std::string DETAIL_MESSAGE;
	static const std::string DETAIL_CODE;
	static const std::string LANG;
	static const std::string CODEVALUERECEIVER;
	static const std::string SOAPBODY;
	static const std::string SOAPHEADER;
	static const std::string SOAPENCODINGNAMESPACE;
	static const std::string REPLY;
	static const std::string WSSEPREFIX;
	static const std::string WSSENAMESPACE;
	static const std::string WSUPREFIX;
	static const std::string WSUNAMESPACE;

	enum SOAPVersion
	{
		SOAP_1_1 = 11,
		SOAP_1_2 = 12
	};

	static constexpr std::size_t MIN_MTOM_SIZE = 64;

	enum Options
	{
		OPT_PRETTY_PRINT                = 0x01,
			/// Pretty-print XML messages.

		OPT_DECLARE_SOAPENC_NAMESPACE   = 0x02,
			/// Add a namespace prefix declaration for the
			/// "http://schemas.xmlsoap.org/soap/encoding/" namespace
			/// to the SOAP envelope.

		OPT_NO_ENCODINGSTYLE_ATTRIBUTE  = 0x04,
			/// Do not include the "encodingStyle" attribute in the SOAP envelope.

		OPT_DECLARE_WSSE_NAMESPACE      = 0x08,
			/// Add a namespace prefix declaration for the
			/// "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd" and
			/// "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd"
			/// namespaces to the SOAP envelope.
	};

	class RemotingNGSOAP_API HeaderInterceptor
		/// Subclasses of this class can be registered with the Serializer
		/// (via the Transport) to add additional elements to the SOAP 
		/// envelope header. 
	{
	public:
		using Ptr = Poco::SharedPtr<HeaderInterceptor>;

		virtual void interceptHeader(XMLElement& header) = 0;
			/// During serialization of the SOAP message, this method is
			/// called with the SOAP envelope Header element.
			///
			/// The method can add child elements to the Header element.

		virtual ~HeaderInterceptor() = default;
	};

	Serializer(int options = 0);
		/// Creates the Serializer for SOAP 1.1.

	Serializer(SOAPVersion soapVersion, int options = 0);
		/// Creates the Serializer for the given SOAP version.

	~Serializer() override;
		/// Destroys the Serializer.

	void addHeaderInterceptor(HeaderInterceptor::Ptr pInterceptor);
		/// Adds a header interceptor which can be used to manipulate the SOAP Header element content,
		/// e.g. for adding a WS-Security header.
	
	void removeHeaderInterceptor(HeaderInterceptor::Ptr pInterceptor);
		/// Removes the given header interceptor.

	void setSOAPVersion(SOAPVersion version);
		/// Sets the SOAP version used for serialization.
		///
		/// Note that this method must not be called after
		/// serialization has started.

	[[nodiscard]] SOAPVersion getSOAPVersion() const;
		/// Returns the SOAP version used for serialization.

	void enableMTOM(bool enable);
		/// Enables or disables MTOM.

	[[nodiscard]] bool isMTOMEnabled() const;
		/// Returns true iff MTOM is enabled.

	void setOptions(int options);
		/// Sets the serializer option flags. See the Options enum for supported flags.

	[[nodiscard]] int getOptions() const;
		/// Returns the serializer option flags. See the Options enum for supported flags.

	[[nodiscard]] std::string createMTOMContentType(const std::string& action = EMPTYSTRING);
		/// Creates the Content-Type value for an MTOM message
		/// containing the MIME boundary and start content ID.

	// Poco::RemotingNG::Serializer
	void serializeMessageBegin(const std::string& name, Poco::RemotingNG::SerializerBase::MessageType type) override;
	void serializeMessageEnd(const std::string& name, Poco::RemotingNG::SerializerBase::MessageType type) override;
	void serializeFaultMessage(const std::string& name, const Poco::Exception& exc) override;
	void serializeStructBegin(const std::string& name) override;
	void serializeStructEnd(const std::string& name) override;
	void serializeSequenceBegin(const std::string& name, Poco::UInt32 numElems) override;
	void serializeSequenceEnd(const std::string& name) override;
	void serializeNullableBegin(const std::string& name, bool isNull) override;
	void serializeNullableEnd(const std::string& name) override;
	void serializeOptionalBegin(const std::string& name, bool isOptional) override;
	void serializeOptionalEnd(const std::string& name) override;
	void serialize(const std::string& name, Poco::Int8 val) override;
	void serialize(const std::string& name, Poco::UInt8 val) override;
	void serialize(const std::string& name, Poco::Int16 val) override;
	void serialize(const std::string& name, Poco::UInt16 val) override;
	void serialize(const std::string& name, Poco::Int32 val) override;
	void serialize(const std::string& name, Poco::UInt32 val) override;
	void serialize(const std::string& name, long val) override;
	void serialize(const std::string& name, unsigned long val) override;

#ifndef POCO_INT64_IS_LONG
	void serialize(const std::string& name, Poco::Int64 val) override;
	void serialize(const std::string& name, Poco::UInt64 val) override;
#endif

	void serialize(const std::string& name, float val) override;
	void serialize(const std::string& name, double val) override;
	void serialize(const std::string& name, bool val) override;
	void serialize(const std::string& name, char val) override;
	void serialize(const std::string& name, const std::string& val) override;
	void serialize(const std::string& name, const std::vector<char>& val) override;
	void serialize(const std::string& name, const Poco::DateTime& val) override;
	void serialize(const std::string& name, const Poco::LocalDateTime& val) override;

	void pushAttribute(const std::string& ns, const std::string& attrName) override;
	void registerNamespace(const std::string& ns) override;

protected:
	void resetImpl() override;
	void setupImpl(std::ostream& out) override;

private:
	struct RemotingNGSOAP_API DelayedElement
	{
		std::string name;
		std::string nameSpace;
		std::string value;
		bool simpleElem;
		XML::AttributesImpl attr;
		std::map<std::string, std::string> openAttr; /// Attrs still missing for this element
		std::vector<DelayedElement> children;
		int curChild;

		DelayedElement();
		DelayedElement(const std::string& val);
		DelayedElement(const std::string& n, const std::string& ns);
		DelayedElement(const std::string& n, const std::string& ns, const std::string& val, bool simpleElem);
		DelayedElement(const std::string& n, const std::string& ns, const std::string& val, bool simpleElem, const std::map<std::string, std::string>& openAttr);
		[[nodiscard]] bool valid() const;
		void clear();
	};

	Serializer(const Serializer&);
	Serializer& operator = (const Serializer&);

	void serializeMethodStart(const std::string& methodName);
	void serializeMethodEnd(const std::string& methodName);
	void serializeData(const std::string& name, const std::string& val);
	void handleStartElementImpl(const std::string& name, const std::string& value);
	void handleStartElementXml(const std::string& name, const std::string& value);
	void handleStartElementComplexImpl(const std::string& name);
	void handleEndElementComplexImpl(const std::string& name);
	void handleStartElementComplexXml(const std::string& name);
	void handleEndElementComplexXml(const std::string& name);
	void handleStartElementNilImpl(const std::string& name);
	void handleStartElementNilXml(const std::string& name);
	void handleStartElementIncludeImpl(const std::string& name, const std::string& cid);
	void handleStartElementIncludeXml(const std::string& name, const std::string& cid);
	void checkHeadersEnd(const std::string& name);
	bool checkHeadersStart(const std::string& name);
	void writeRoot();
	void createEnvelope();
	[[nodiscard]] std::string createContentID();
	void writeParts();
	void interceptHeaders(DelayedElement& delayedHeader);
	[[nodiscard]] static DelayedElement delayElement(const XMLElement& element);

private:
	using StartElementFunct = void (Serializer::*)(const std::string&, const std::string&);
	using StartElementComplexFunct = void (Serializer::*)(const std::string&);
	using EndElementComplexFunct = void (Serializer::*)(const std::string&);
	using StartElementNilFunct = void (Serializer::*)(const std::string&);
	using StartElementIncludeFunct = void (Serializer::*)(const std::string&, const std::string&);

	void write(const Serializer::DelayedElement& elem, bool closeXMLTags);
	void closeXML();

	std::vector<HeaderInterceptor::Ptr> _headerInterceptors;
	SOAPVersion _soapVersion;
	bool _mtom;
	int _options;
	const std::string* _pSOAPNamespace;
	std::unique_ptr<XML::XMLWriter> _pXMLWriter;
	std::map<std::string, std::string> _attr; /// Collects attr info for elements, maps attr names to namespaces.
	std::set<std::string> _headerElements;
	Poco::SharedPtr<DelayedElement> _pRoot;
	std::stack<DelayedElement*> _curParent;
	int _depth;
	Poco::SharedPtr<DelayedElement> _pDelayed;
	StartElementFunct handleStartElement;
	StartElementComplexFunct handleStartElementComplex;
	EndElementComplexFunct handleEndElementComplex;
	StartElementNilFunct handleStartElementNil;
	StartElementIncludeFunct handleStartElementInclude;
	bool _documentStarted;
	std::ostream* _pStream;
	Poco::Net::MultipartWriter* _pMultipartWriter;
	std::string _boundary;
	std::string _startContentID;
	std::map<std::string, const std::vector<char>*> _parts;
	bool _declareEnvelopeNamespace;
};


//
// inlines
//
inline Serializer::SOAPVersion Serializer::getSOAPVersion() const
{
	return _soapVersion;
}


inline int Serializer::getOptions() const
{
	return _options;
}


} // namespace Poco::RemotingNG::SOAP


#endif // RemotingNG_SOAP_Serializer_INCLUDED
