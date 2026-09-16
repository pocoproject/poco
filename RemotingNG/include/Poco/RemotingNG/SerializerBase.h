//
// SerializerBase.h
//
// Library: RemotingNG
// Package: Serialization
// Module:  SerializerBase
//
// Definition of the SerializerBase class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_SerializerBase_INCLUDED
#define RemotingNG_SerializerBase_INCLUDED


#include "Poco/RemotingNG/RemotingNG.h"
#include <map>
#include <stack>


namespace Poco {
namespace RemotingNG {


class RemotingNG_API SerializerBase
	/// SerializerBase is the common base class for Serializer and Deserializer.
	///
	/// This class provides common definitions and member
	/// functions for Serializer and Deserializer classes,
	/// most importantly properties management.
	///
	/// Properties allow a TypeSerializer or other classes using a Serializer or
	/// Deserializer to send additional (meta-)information to the Serializer (or
	/// Deserializer), for example XML namespace information for XML elements.
	///
	/// Properties are managed in a stacks -- a separate stack is maintained
	/// for every property identified by a name. Users of Serializer and
	/// Deserializer classes can push property values onto a stack before
	/// serializing a certain object or message, and pop properties
	/// from the stack when serialization is completed.
{
public:
	enum MessageType
	{
		MESSAGE_REQUEST,
		MESSAGE_REPLY,
		MESSAGE_EVENT,
		MESSAGE_EVENT_REPLY,
		MESSAGE_FAULT
	};

	static const std::string PROP_NAMESPACE;
	static const std::string PROP_HEADER;
	static const std::string PROP_ACTION;
	static const std::string PROP_REQUEST;
	static const std::string PROP_REPLY;
	static const std::string PROP_FAULT;
	static const std::string PROP_LOCATION;
	static const std::string PROP_FORMAT;
	static const std::string PROP_PATH;
	static const std::string PROP_PRODUCES;
	static const std::string PROP_CONSUMES;
	static const std::string PROP_CONTENT_TYPE;
	static const std::string PROP_LENGTH;
	static const std::string PROP_XSDTYPE;

	static const std::string RETURN_PARAM;
		/// The element name used to serialize a function's return value.

	void pushProperty(const std::string& name, const std::string& value);
		/// Pushes a property value onto the stack for the property with the given name.

	void popProperty(const std::string& name);
		/// Pops a property value from its stack.
		///
		/// Throws an exception if the property stack is empty or does not exist.

	const std::string& getProperty(const std::string& name) const;
		/// Gets the property with the given name from its stack.
		///
		/// Throws an exception if the stack is empty.

	const std::string& getProperty(const std::string& name, const std::string& deflt) const;
		/// Gets the property with the given name from its stack. Returns the default value if the stack is empty.
		///
		/// Warning: Since this method may return a const reference to the default value, the
		/// caller must make sure not to pass a temporary object as default value.

	bool hasProperty(const std::string& name) const;
		/// Returns true if a property with the given name exists.

	void reset();
		/// Resets the Serializer or Deserializer to a clean state.

protected:
	SerializerBase();
		/// Creates a Serializer.

	virtual ~SerializerBase();
		/// Destroys the Serializer.

	virtual void resetImpl() = 0;
		/// Resets the serializer. Must be implemented by subclasses.

	void clearProperties();
		/// Clears all properties.

private:
	SerializerBase(const SerializerBase&);
	SerializerBase& operator = (const SerializerBase&);

	using ValueStack = std::stack<std::string>;
	using Properties = std::map<std::string, ValueStack>;

	Properties _props;
};


//
// inlines
//
inline void SerializerBase::reset()
{
	clearProperties();
	resetImpl();
}


} } // namespace Poco::RemotingNG


#endif // RemotingNG_SerializerBase_INCLUDED
