//
// AttributedObject.h
//
// Library: RemotingNG
// Package: ORB
// Module:  AttributedObject
//
// Definition of the AttributedObject class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_AttributedObject_INCLUDED
#define RemotingNG_AttributedObject_INCLUDED


#include "Poco/RemotingNG/RemotingNG.h"
#include "Poco/AutoPtr.h"
#include "Poco/Mutex.h"
#include <cstdlib>
#include <vector>
#include <map>


namespace Poco {
namespace RemotingNG {


class RemotingNG_API AttributedObject
	/// A AttributedObject can hold any number of attributes in 
	/// the form of name-value pairs.
{
public:
	using NameValueMap = std::map<std::string, std::string>;

	AttributedObject();
		/// Creates an AttributedObject.

	AttributedObject(const AttributedObject& other);
		/// Creates an AttributedObject by copying the attributes from another one.

	AttributedObject(AttributedObject&& other) noexcept;
		/// Creates an AttributedObject by moving the attributes from another one.
		
	AttributedObject& operator = (const AttributedObject& other);
		/// Assigns the attributes from another AttributedObject.

	AttributedObject& operator = (AttributedObject&& other) noexcept;
		/// Moves the attributes from another AttributedObject.

	virtual ~AttributedObject();
		/// Destroys the AttributedObject.
		
	void setAttribute(const std::string& name, const std::string& value);
		/// Adds or modifies the attribute with the given name.
		
	const std::string& getAttribute(const std::string& name) const;
		/// Returns the value of the attribute with the given name.
		/// Throws a Poco::NotFoundException if no such attribute
		/// exists.
		
	const std::string& getAttribute(const std::string& name, const std::string& deflt) const;
		/// Returns the value of the attribute with the given name,
		/// or the given default value if no such attribute
		/// exists.
	
	bool hasAttribute(const std::string& name) const;
		/// Returns true iff an attribute with the given name exists.
		
	void removeAttribute(const std::string& name);
		/// Removes the attribute with the given name.

		/// Does nothing if no attribute with the given name exists.
		
	std::size_t countAttributes() const;
		/// Returns the number of attributes.
		
	std::vector<std::string> enumerateAttributes() const;
		/// Returns a vector containing all attribute names.
		
	void clearAttributes();
		/// Removes all attributes.
	
private:
	NameValueMap _attributes;
	mutable Poco::FastMutex _mutex;
};


//
// inlines
//
inline std::size_t AttributedObject::countAttributes() const
{
	return _attributes.size();
}


} } // namespace Poco::RemotingNG


#endif // RemotingNG_AttributedObject_INCLUDED
