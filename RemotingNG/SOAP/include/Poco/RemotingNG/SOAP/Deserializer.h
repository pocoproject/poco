//
// Deserializer.h
//
// Library: RemotingNG/SOAP
// Package: SOAP
// Module:  Deserializer
//
// Definition of the Deserializer class.
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_SOAP_Deserializer_INCLUDED
#define RemotingNG_SOAP_Deserializer_INCLUDED


#include "Poco/RemotingNG/SOAP/SOAP.h"
#include "Poco/RemotingNG/SOAP/XMLElement.h"
#include "Poco/RemotingNG/SOAP/SOAPContentHandler.h"
#include "Poco/RemotingNG/SOAP/Serializer.h"
#include "Poco/RemotingNG/Deserializer.h"
#include "Poco/SAX/SAXParser.h"
#include "Poco/SAX/WhitespaceFilter.h"
#include "Poco/SharedPtr.h"
#include <set>
#include <stack>


namespace Poco {
namespace RemotingNG {
namespace SOAP {


class RemotingNGSOAP_API Deserializer: public Poco::RemotingNG::Deserializer
	/// The Deserializer implementation for the SOAP Transport.
	///
	/// For more information, please see the Serializer documentation.
	///
	/// The deserializer can also be used to deserialize plain
	/// XML. To do so, instead of deserializeMessageBegin() and
	/// deserializeMessageEnd(), call deserializeStructBegin()
	/// and deserializeStructEnd(), respectively, at the root level,
	/// after calling setup().
{
public:
	class RemotingNGSOAP_API HeaderInterceptor
		/// Subclasses of this class can be registered with the Deserializer
		/// (via the Transport or Listener) to process the SOAP envelope
		/// header element and its children.
	{
	public:
		using Ptr = Poco::SharedPtr<HeaderInterceptor>;

		virtual void interceptHeader(const XMLElement& header) = 0;
			/// During deserialization of the SOAP message, this method is
			/// called with the SOAP envelope Header element.
			///
			/// The method can inspect the Header element and its children.
	
		virtual ~HeaderInterceptor() = default;
	};

	Deserializer();
		/// Creates the Deserializer.

	~Deserializer();
		/// Destroys the Deserializer.

	void addHeaderInterceptor(HeaderInterceptor::Ptr pInterceptor);
		/// Adds a header interceptor which can be used to handle the SOAP Header element content,
		/// e.g. for adding a WS-Security header.

	void removeHeaderInterceptor(HeaderInterceptor::Ptr pInterceptor);
		/// Removes the given header interceptor.

	Serializer::SOAPVersion soapVersion() const;
		/// Returns the SOAP version (1.1 or 1.2) of the
		/// most recently processed message.

	void enableMTOM(bool enable);
		/// Enable MTOM for the next messages.

	bool isMTOMEnabled() const;
		/// Returns true iff MTOM is enabled.

	void parseContentType(const std::string& contentType);
		/// If MTOM is enabled, parses the Content-Type for MIME boundary
		/// and content IDs.

	// Poco::RemotingNG::Deserializer
	Poco::RemotingNG::SerializerBase::MessageType findMessage(std::string& name);
	void deserializeMessageBegin(const std::string& name, Poco::RemotingNG::SerializerBase::MessageType type);
	void deserializeMessageEnd(const std::string& name, Poco::RemotingNG::SerializerBase::MessageType type);
	bool deserializeStructBegin(const std::string& name, bool isMandatory);
	void deserializeStructEnd(const std::string& name);
	bool deserializeSequenceBegin(const std::string& name, bool isMandatory, Poco::UInt32& sizeHint);
	void deserializeSequenceEnd(const std::string& name);
	bool deserializeNullableBegin(const std::string& name, bool isMandatory, bool& isNull);
	void deserializeNullableEnd(const std::string& name);
	bool deserializeOptionalBegin(const std::string& name, bool isMandatory, bool& isSpecified);
	void deserializeOptionalEnd(const std::string& name);

	bool deserialize(const std::string& name, bool isMandatory, Poco::Int8& value);
	bool deserialize(const std::string& name, bool isMandatory, Poco::UInt8& value);
	bool deserialize(const std::string& name, bool isMandatory, Poco::Int16& value);
	bool deserialize(const std::string& name, bool isMandatory, Poco::UInt16& value);
	bool deserialize(const std::string& name, bool isMandatory, Poco::Int32& value);
	bool deserialize(const std::string& name, bool isMandatory, Poco::UInt32& value);
	bool deserialize(const std::string& name, bool isMandatory, long& value);
	bool deserialize(const std::string& name, bool isMandatory, unsigned long& value);
#ifndef POCO_INT64_IS_LONG
	bool deserialize(const std::string& name, bool isMandatory, Poco::Int64& value);
	bool deserialize(const std::string& name, bool isMandatory, Poco::UInt64& value);
#endif
	bool deserialize(const std::string& name, bool isMandatory, float& value);
	bool deserialize(const std::string& name, bool isMandatory, double& value);
	bool deserialize(const std::string& name, bool isMandatory, bool& value);
	bool deserialize(const std::string& name, bool isMandatory, char& value);
	bool deserialize(const std::string& name, bool isMandatory, std::string& value);
	bool deserialize(const std::string& name, bool isMandatory, std::vector<char>& value);
	bool deserialize(const std::string& name, bool isMandatory, Poco::DateTime& value);
	bool deserialize(const std::string& name, bool isMandatory, Poco::LocalDateTime& value);

	void pushAttribute(const std::string& ns, const std::string& attrName, bool isMandatory);

protected:
	void resetImpl();
	void setupImpl(std::istream& istr);

private:
	Deserializer(const Deserializer&);
	Deserializer& operator = (const Deserializer&);

	void deserializeMethodStart(const std::string& methodName);
		/// Throws an exception if the methodName doesn't match.

	const std::string& detectMethodName();
		/// Deserializes until the method name is found, then returns the name.
		/// Used on the skeleton side to deserialize incoming requests. You still must call deserializeMethodStart
		/// when using this method.

	void deserializeMethodEnd(const std::string& mi);

	void handleConditionalError(const std::string& name, bool isMandatory);
		/// Throws an exception if isMandatory is set to true.

	const std::string& getData(const std::string& name, bool isMandatory, bool& found);
		/// Returns the value as string, throws an exception if the entry is not found and isMandatory is true.
		/// Advances the childposition in the parent

	void checkAttributes();
		/// Checks that the mandatory attributes are all present

	void checkHeaderStart(const std::string& name);
	void checkHeaderEnd(const std::string& name);

	static std::string unbracketContentID(const std::string& cid);
	static void copyToVector(std::istream& istr, std::vector<char>& vec);

private:
	using EntryPair = std::pair<const SOAPContentHandler::Node*, std::size_t>;
		/// A pair containing the entry and the currently active child position
	using AttrInfo = std::pair<std::string, bool>;
		/// Stores the namespace and the mandatory info
	using AttrMap = std::map<std::string, AttrInfo>;

	std::vector<HeaderInterceptor::Ptr> _headerInterceptors;
	Serializer::SOAPVersion _soapVersion;
	bool _mtom;
	const SOAPContentHandler::Node* _pRoot;
	XML::SAXParser _internalReader;
	XML::WhitespaceFilter _reader;
	SOAPContentHandler _contentHandler;
	std::stack<EntryPair> _entries;
	AttrMap _attr;
	std::string _tmp;
	std::string _tmpReplyName;
	std::set<std::string> _headerElements;
	std::map<std::string, std::vector<char>> _parts;
	std::string _boundary;
	std::string _startContentID;
};


//
// inlines
//
inline Serializer::SOAPVersion Deserializer::soapVersion() const
{
	return _soapVersion;
}


} } } // namespace Poco::RemotingNG::SOAP


#endif // RemotingNG_SOAP_Deserializer_INCLUDED
