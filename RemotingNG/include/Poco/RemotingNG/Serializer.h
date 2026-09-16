//
// Serializer.h
//
// Library: RemotingNG
// Package: Serialization
// Module:  Serializer
//
// Definition of the Serializer class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_Serializer_INCLUDED
#define RemotingNG_Serializer_INCLUDED


#include "Poco/RemotingNG/RemotingNG.h"
#include "Poco/RemotingNG/SerializerBase.h"
#include "Poco/DateTime.h"
#include "Poco/LocalDateTime.h"
#include "Poco/Timestamp.h"
#include "Poco/Timespan.h"
#include "Poco/UUID.h"
#include "Poco/ULID.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"
#include <vector>


namespace Poco {
namespace RemotingNG {


class RemotingNG_API Serializer: public SerializerBase
	/// The Serializer interface for transports.
	///
	/// Implementations of the Serializer interface must handle
	/// serialization of various message types (request, response,
	/// events), as well as basic types. Serialization of complex
	/// types is implemented by providing specializations of the
	/// TypeSerializer class template.
{
public:
	Serializer();
		/// Creates a Serializer.

	virtual ~Serializer();
		/// Destroys the Serializer.

	void setup(std::ostream& ostr);
		/// Set up the Serializer for writing to the given output stream.
		///
		/// This must be called before any of the serialize*() methods.

	virtual void serializeMessageBegin(const std::string& name, SerializerBase::MessageType type) = 0;
		/// Begin serialization of a message.

	virtual void serializeMessageEnd(const std::string& name, SerializerBase::MessageType type) = 0;
		/// End serialization of a message.

	virtual void serializeFaultMessage(const std::string& name, const Poco::Exception& exc) = 0;
		/// Serialize an exception message.

	virtual void serializeStructBegin(const std::string& name) = 0;
		/// Begin serialization of a complex (structured) object.

	virtual void serializeStructEnd(const std::string& name) = 0;
		/// End serialization of a complex (structured) object.

	virtual void serializeSequenceBegin(const std::string& name, Poco::UInt32 length) = 0;
		/// Begin serialization of a vector or other sequence.
		///
		/// Length contains the number of elements in the sequence.

	virtual void serializeSequenceEnd(const std::string& name) = 0;
		/// End serialization of a vector.

	virtual void serializeNullableBegin(const std::string& name, bool isNull) = 0;
		/// Begin serialization of a Nullable or pointer which may be NULL.
		///
		/// If isNull is true, the call to serializeNullableBegin() must be immediately
		/// followed by a call to serializeNullableEnd().

	virtual void serializeNullableEnd(const std::string& name) = 0;
		/// End serialization of a Nullable or pointer which may be NULL.

	virtual void serializeOptionalBegin(const std::string& name, bool isSpecified);
		/// Begin serialization of an Optional.
		///
		/// If isSpecified is false, the call to serializeOptionalBegin() must be immediately
		/// followed by a call to serializeOptionalEnd().
		///
		/// The default implementation calls serializeNullableBegin().

	virtual void serializeOptionalEnd(const std::string& name);
		/// End serialization of an Optional.
		///
		/// The default implementation calls serializeNullableEnd().

	virtual void serialize(const std::string& name, Poco::Int8 value) = 0;
		/// Serialize an Int8.

	virtual void serialize(const std::string& name, Poco::UInt8 value) = 0;
		/// Serialize an UInt8.

	virtual void serialize(const std::string& name, Poco::Int16 value) = 0;
		/// Serialize an Int16.

	virtual void serialize(const std::string& name, Poco::UInt16 value) = 0;
		/// Serialize an UInt16.

	virtual void serialize(const std::string& name, Poco::Int32 value) = 0;
		/// Serialize an Int32.

	virtual void serialize(const std::string& name, Poco::UInt32 value) = 0;
		/// Serialize an UInt32.

	virtual void serialize(const std::string& name, long value) = 0;
		/// Serialize a long.

	virtual void serialize(const std::string& name, unsigned long value) = 0;
		/// Serialize an unsigned long.

#ifndef POCO_INT64_IS_LONG
	virtual void serialize(const std::string& name, Poco::Int64 value) = 0;
		/// Serialize an Int64.

	virtual void serialize(const std::string& name, Poco::UInt64 value) = 0;
		/// Serialize an UInt64.
#endif

	virtual void serialize(const std::string& name, bool value) = 0;
		/// Serialize a boolean.

	virtual void serialize(const std::string& name, float value) = 0;
		/// Serialize a float.

	virtual void serialize(const std::string& name, double value) = 0;
		/// Serialize a double.

	virtual void serialize(const std::string& name, char value) = 0;
		/// Serialize a single character.

	virtual void serialize(const std::string& name, const std::string& value) = 0;
		/// Serialize a std::string.

	virtual void serialize(const std::string& name, const std::vector<char>& value) = 0;
		/// Serialize binary data.

	virtual void serialize(const std::string& name, const Poco::DateTime& value);
		/// Serialize a Poco::DateTime.
		///
		/// The default implementation serializes to an ISO8601 string.

	virtual void serialize(const std::string& name, const Poco::LocalDateTime& value);
		/// Serialize a Poco::LocalDateTime.
		///
		/// The default implementation serializes to an ISO8601 string.

	virtual void serialize(const std::string& name, const Poco::Timestamp& value);
		/// Serialize a Poco::Timestamp.
		///
		/// The default implementation serializes to an ISO8601 string.

	virtual void serialize(const std::string& name, const Poco::Timespan& value);
		/// Serialize a Poco::Timespan.
		///
		/// The default implementation serializes to a 64-bit integer containing
		/// the timespan in microseconds.

	virtual void serialize(const std::string& name, const Poco::URI& value);
		/// Serialize a Poco::URI.
		///
		/// The default implementation serializes to string.

	virtual void serialize(const std::string& name, const Poco::UUID& value);
		/// Serialize a Poco::UUID.
		///
		/// The default implementation serializes to string.

	virtual void serialize(const std::string& name, const Poco::ULID& value);
		/// Serialize a Poco::ULID.
		///
		/// The default implementation serializes to string.

	virtual void pushAttribute(const std::string& attrNamespace, const std::string& attrName);
		/// For XML-based transports, this method allows for serialization of data either as
		/// element content (default) or attribute value.
		///
		/// The rule that serializers follow is to first call pushAttribute() for every variable to
		/// be serialized as attribute, then call serializeMessageBegin() or serializeStructBegin()
		/// for the element containing the attributes.
		///
		/// Example: The following code:
		///
		///     ser.pushAttribute("", "attr1");
		///     ser.pushAttribute("", "attr2");
		///     ser.serializeStructBegin("complexType");
		///     ser.serialize("attr1", "foo");
		///     ser.serialize("attr2", "bar");
		///     ser.serialize("variable", 42);
		///     ser.serializeStructEnd("complexType");
		///
		/// will produce this XML fragment:
 		///     <complexType attr1="foo" attr2="bar"><variable>42</variable>...</complexType>
 		///
 		/// Should be overridden by subclasses supporting attributes. The default implementation
 		/// does nothing.

	virtual void registerNamespace(const std::string& namespc);
		/// For XML-based transports, this method allows for registration of a namespace
		/// which will be used by multiple following elements. This allows the Serializer
		/// to generate a namespace prefix declaration on an outer element, preventing
		/// the Serializer from generating a prefix declaration on any element in a sequence.

protected:
	virtual void setupImpl(std::ostream& ostr) = 0;
		/// Set up the Serializer for writing
		/// to the given output stream.
		///
		/// It is guaranteed that reset was called prior to calling this method
		/// so the Serializer is in a clean state.
};


//
// inlines
//
inline void Serializer::setup(std::ostream& ostr)
{
	reset();
	setupImpl(ostr);
}


} } // namespace Poco::RemotingNG


#endif // RemotingNG_Serializer_INCLUDED
