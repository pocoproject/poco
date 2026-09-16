//
// Context.h
//
// Library: RemotingNG
// Package: Transport
// Module:  Context
//
// Definition of the Context class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_Context_INCLUDED
#define RemotingNG_Context_INCLUDED


#include "Poco/RemotingNG/Identifiable.h"
#include "Poco/RemotingNG/RemoteObject.h"
#include "Poco/RemotingNG/Skeleton.h"
#include "Poco/RemotingNG/Credentials.h"
#include "Poco/RefCountedObject.h"
#include "Poco/AutoPtr.h"
#include "Poco/Any.h"
#include "Poco/ThreadLocal.h"
#include <map>


namespace Poco {
namespace RemotingNG {


class RemotingNG_API Context: public Poco::RefCountedObject
	/// A Context object is used to pass transport-specific
	/// information (such as caller IP address or authentication
	/// information) to a service method in a generic way.
	///
	/// Context objects are created and filled by the transport,
	/// and are accessed via thread-local storage.
{
public:
	using Ptr = Poco::AutoPtr<Context>;
	using Attributes = std::map<std::string, Poco::Any>;

	Context();
		/// Creates an empty Context. 

	~Context();
		/// Destroys the Context.

	static void set(Context::Ptr pContext);
		/// Sets the Context object for the current thread.
		
	static Context::Ptr get();
		/// Returns the Context object for the current thread.
		/// Result may be null if no Context is available.
		
	bool has(const std::string& key) const;
		/// Returns true iff the Context has an attribute with the given value.

	const Poco::Any& get(const std::string& key) const;
		/// Returns the attribute with the given key. 
		///
		/// Throws a Poco::NotFoundException
		/// if the attribute does not exist.
		
	template <typename T>
	T getValue(const std::string& key) const
		/// Convenience function that returns the attribute with the given key,
		/// casted to the desired type.
		///
		/// Throws a Poco::NotFoundException if the attribute does not exist.
		/// Throws a Poco::BadCastException if the cast is invalid.
	{
		const Poco::Any& any = get(key);
		return Poco::AnyCast<T>(any);
	}
	
	template <typename T>
	T getValue(const std::string& key, T deflt) const
		/// Convenience function that returns the attribute with the given key,
		/// casted to the desired type. If the attribute does not exist,
		/// the given default value is returned.
		///
		/// Throws a Poco::BadCastException if the cast is invalid.
	{
		Attributes::const_iterator it = _attrs.find(key);
		if (it != _attrs.end())
			return Poco::AnyCast<T>(it->second);
		else
			return deflt;
	}

	void set(const std::string& key, const Poco::Any& value);
		/// Sets/Overwrites an attribute value.
		
	template <typename T>
	void setValue(const std::string& key, T value)
		/// Sets/Overwrites an attribute value.
	{
		set(key, Poco::Any(value));
	}

	Attributes::const_iterator find(const std::string& key) const;
		/// Searches for an attribute. Returns end() if not found.

	Attributes::const_iterator begin() const;
		/// Returns the begin iterator for attributes.

	Attributes::const_iterator end() const;
		/// Returns the end iterator for attributes.
		
	const Credentials& getCredentials() const;
		/// Returns the Credentials associated with the Context.
		
	void setCredentials(const Credentials& creds);
		/// Sets the Credentials associated with the Context.
		
	void clearCredentials();
		/// Clears the Credentials associated with the Context.

private:
	Attributes _attrs;
	Credentials _creds;
	
	static Poco::ThreadLocal<Ptr> _pContextHolder;
};


class RemotingNG_API ScopedContext
	/// Helper class using RAII for Context setup and teardown.
{
public:
	ScopedContext();
		/// Creates a Context object and registers it as
		/// the current thread's Context.
		
	~ScopedContext();
		/// Destroys the Context object and removes it
		/// from the current thread.
	
	Context::Ptr context() const;
		/// Returns the Context object.

private:
	Context::Ptr _pContext;
};


//
// inlines
//
inline const Credentials& Context::getCredentials() const
{
	return _creds;
}


inline Context::Ptr ScopedContext::context() const
{
	return _pContext;
}


} } // namespace Poco::RemotingNG


#endif // RemotingNG_Context_INCLUDED
