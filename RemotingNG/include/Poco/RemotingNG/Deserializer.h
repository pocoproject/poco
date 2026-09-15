//
// Deserializer.h
//
// Library: RemotingNG
// Package: Serialization
// Module:  Deserializer
//
// Definition of the Deserializer class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_Deserializer_INCLUDED
#define RemotingNG_Deserializer_INCLUDED


#include "Poco/RemotingNG/RemotingNG.h"
#include "Poco/RemotingNG/SerializerBase.h"
#include "Poco/DateTime.h"
#include "Poco/LocalDateTime.h"
#include "Poco/Timestamp.h"
#include "Poco/Timespan.h"
#include "Poco/UUID.h"
#include "Poco/ULID.h"
#include "Poco/URI.h"
#include <vector>


namespace Poco {
namespace RemotingNG {


class RemotingNG_API Deserializer: public SerializerBase
	/// The Deserializer interface for transports.
	///
	/// Implementations of the Deserializer interface must handle
	/// deserialization of various message types (request, response,
	/// events), as well as basic types. Deserialization of complex
	/// types is implemented by providing specializations of the
	/// TypeDeserializer class template.
	///
	/// Note that deserialize*() methods will generally return a bool
	/// and have a bool parameter isMandatory. These methods will return
	/// true if the respective element or value has been found and
	/// deserialized. If the element or value has not been found and
	/// isMandatory is true, an exception will be thrown. If isMandatory
	/// is false, false will be returned and the value will not be changed.
{
public:
	Deserializer();
		/// Creates a Deserializer.

	virtual ~Deserializer();
		/// Destroys the Deserializer.

	void setup(std::istream& istr);
		/// Set up the Deserializer for reading from the given input stream.
		///
		/// This must be called before any of the deserialize*() methods.

	virtual SerializerBase::MessageType findMessage(std::string& name) = 0;
		/// Reads from the stream until the message has been found.
		/// Returns the type of the message and the name of the found message.
		///
		/// Used on the skeleton side to deserialize incoming requests.
		/// Will not be called by a Proxy when deserializing a reply.
		///
		/// The name returned by this method must be passed to deserializeMessageBegin().
		///
		/// This method must be implemented in such a way that it can be called
		/// multiple times in succession, until deserializeMessageBegin() is called.

	virtual void deserializeMessageBegin(const std::string& name, SerializerBase::MessageType type) = 0;
		/// Begin deserialization of a message.
		///
		/// If the message is a fault, it will be deserialized an an appropriate exception will be thrown.

	virtual void deserializeMessageEnd(const std::string& name, SerializerBase::MessageType type) = 0;
		/// End deserialization of a message.

	virtual bool deserializeStructBegin(const std::string& name, bool isMandatory) = 0;
		/// Begin deserialization of a complex (structured) object.
		///
		/// Returns true if the element with the given name was found.
		/// If the element was not found and isMandatory is true, an exception will be thrown.
		///
		/// Note that if this method returns false, deserializeComplexTypeEnd() will not be called.

	virtual void deserializeStructEnd(const std::string& name) = 0;
		/// End deserialization of a complex (structured) object.

	virtual bool deserializeSequenceBegin(const std::string& name, bool isMandatory, Poco::UInt32& lengthHint) = 0;
		/// Begin deserialization of a vector or other sequence.
		///
		/// Returns true if the element with the given name was found.
		/// If the element was not found and isMandatory is true, an exception may be thrown
		/// (depending on whether the serialization format can distinguish between an empty
		/// and a missing sequence).
		///
		/// Note that if this method returns false, deserializeVectorBegin() will not be called.
		/// If sizeHint is greater than 0, it will be used to reserve space in the result vector.

	virtual void deserializeSequenceEnd(const std::string& name) = 0;
		/// End deserialization of a vector or other sequence.

	virtual bool deserializeNullableBegin(const std::string& name, bool isMandatory, bool& isNull) = 0;
		/// Begin deserialization of a Nullable or pointer which may be NULL.
		///
		/// Returns true if the element with the given name was found.
		/// If the element was not found and isMandatory is true, an exception may be thrown
		/// (depending on whether the serialization format can distinguish between a NULL
		/// and a missing value). IsNull will be set accordingly.
		///
		/// Note that if this method returns false or sets isNull to true,
		/// deserializeNullableEnd() will not be called.

	virtual void deserializeNullableEnd(const std::string& name) = 0;
		/// End deserialization of a Nullable or pointer which may be NULL.

	virtual bool deserializeOptionalBegin(const std::string& name, bool isMandatory, bool& isSpecified);
		/// Begin deserialization of an Optional.
		///
		/// Returns true if the element with the given name was found.
		/// If the element was not found and isMandatory is true, an exception may be thrown
		/// (depending on whether the serialization format can distinguish between a NULL
		/// and a missing value). IsSpecified will be set accordingly.
		///
		/// Note that if this method returns false or sets isSpecified to false,
		/// deserializeOptionalEnd() will not be called.
		///
		/// The default implementation calls deserializeNullableBegin().

	virtual void deserializeOptionalEnd(const std::string& name);
		/// End deserialization of an Optional.
		///
		/// The default implementation calls deserializeNullableEnd().

	virtual bool deserialize(const std::string& name, bool isMandatory, Poco::Int8& value) = 0;
		/// Deserialize an Int8.

	virtual bool deserialize(const std::string& name, bool isMandatory, Poco::UInt8& value) = 0;
		/// Deserialize an UInt8.

	virtual bool deserialize(const std::string& name, bool isMandatory, Poco::Int16& value) = 0;
		/// Deserialize an Int16.

	virtual bool deserialize(const std::string& name, bool isMandatory, Poco::UInt16& value) = 0;
		/// Deserialize an UInt16.

	virtual bool deserialize(const std::string& name, bool isMandatory, Poco::Int32& value) = 0;
		/// Deserialize an Int32.

	virtual bool deserialize(const std::string& name, bool isMandatory, Poco::UInt32& value) = 0;
		/// Deserialize an UInt32.

	virtual bool deserialize(const std::string& name, bool isMandatory, long& value) = 0;
		/// Deserialize a long.

	virtual bool deserialize(const std::string& name, bool isMandatory, unsigned long& value) = 0;
		/// Deserialize an unsigned long.

#ifndef POCO_INT64_IS_LONG
	virtual bool deserialize(const std::string& name, bool isMandatory, Poco::Int64& value) = 0;
		/// Deserialize an Int64.

	virtual bool deserialize(const std::string& name, bool isMandatory, Poco::UInt64& value) = 0;
		/// Deserialize an UInt64.
#endif

	virtual bool deserialize(const std::string& name, bool isMandatory, float& value) = 0;
		/// Deserialize a float.

	virtual bool deserialize(const std::string& name, bool isMandatory, double& value) = 0;
		/// Deserialize a double.

	virtual bool deserialize(const std::string& name, bool isMandatory, bool& value) = 0;
		/// Deserialize a boolean.

	virtual bool deserialize(const std::string& name, bool isMandatory, char& value) = 0;
		/// Deserialize a single character.

	virtual bool deserialize(const std::string& name, bool isMandatory, std::string& value) = 0;
		/// Deserialize a std::string.

	virtual bool deserialize(const std::string& name, bool isMandatory, std::vector<char>& value) = 0;
		/// Deserializes raw binary data.

	virtual bool deserialize(const std::string& name, bool isMandatory, Poco::DateTime& value);
		/// Deserializes a Poco::DateTime.
		///
		/// The default implementation expects an ISO8601-formatted string.

	virtual bool deserialize(const std::string& name, bool isMandatory, Poco::LocalDateTime& value);
		/// Deserializes a Poco::LocalDateTime.
		///
		/// The default implementation expects an ISO8601-formatted string.

	virtual bool deserialize(const std::string& name, bool isMandatory, Poco::Timestamp& value);
		/// Deserializes a Poco::Timestamp.
		///
		/// The default implementation expects an ISO8601-formatted string.

	virtual bool deserialize(const std::string& name, bool isMandatory, Poco::Timespan& value);
		/// Deserializes a Poco::Timespan.
		///
		/// The default implementation expects a 64-bit integer containing the time
		/// difference in microseconds.

	virtual bool deserialize(const std::string& name, bool isMandatory, Poco::URI& value);
		/// Deserializes a Poco::URI.
		///
		/// The default implementation expects a string representation.

	virtual bool deserialize(const std::string& name, bool isMandatory, Poco::UUID& value);
		/// Deserializes a Poco::UUID.
		///
		/// The default implementation expects a string representation.

	virtual bool deserialize(const std::string& name, bool isMandatory, Poco::ULID& value);
		/// Deserializes a Poco::ULID.
		///
		/// The default implementation expects a string representation.

	virtual void pushAttribute(const std::string& attrNamespace, const std::string& attrName, bool isMandatory);
		/// For XML-based transports, this method allows for deserialization of data either as
		/// element content (default) or attribute value.
		///
		/// The rule that deserializers follow is to first call pushAttribute() for every variable to
		/// be deserialized as attribute, then call deserializeMessageBegin() or deserializeStructStart()
		/// for the element containing the attributes.
		///
		/// Example: The following code:
		///
		///     ser.pushAttribute("", "attr1", true);
		///     ser.pushAttribute("", "attr2", true);
		///     ser.deserializeStructBegin("complexType", true);
		///     ser.deserialize("attr1", attr1, true);
		///     ser.deserialize("attr2", attr2, true);
		///     ser.deserialize("variable", variable, true);
		///     ser.deserializeStructEnd("complexType");
		///
		/// will read this XML fragment:
 		///     <complexType attr1="foo" attr2="bar"><variable>42</variable>...</complexType>
 		///
 		/// Should be overridden by subclasses supporting attributes. The default implementation
 		/// does nothing.

protected:
	virtual void setupImpl(std::istream& inStream) = 0;
		/// Set up the Deserializer for reading
		/// from the given input stream.
		///
		/// It is guaranteed that reset was called prior to calling this method
		/// so the Serializer is in a clean state.
};


//
// inlines
//
inline void Deserializer::setup(std::istream& istr)
{
	reset();
	setupImpl(istr);
}


} } // namespace Poco::RemotingNG


#endif // RemotingNG_Deserializer_INCLUDED
