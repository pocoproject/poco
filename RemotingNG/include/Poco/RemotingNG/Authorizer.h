//
// Authorizer.h
//
// Library: RemotingNG
// Package: Transport
// Module:  Authorizer
//
// Definition of the Authorizer class.
//
// Copyright (c) 2017, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#ifndef RemotingNG_Authorizer_INCLUDED
#define RemotingNG_Authorizer_INCLUDED


#include "Poco/RemotingNG/RemotingNG.h"
#include "Poco/RefCountedObject.h"
#include "Poco/AutoPtr.h"


namespace Poco {
namespace RemotingNG {


class RemotingNG_API Authorizer: public Poco::RefCountedObject
	/// The Authorizer interface is used for authorizing incoming requests. 
	///
	/// Authorizers are specific to a transport and external authentication/ 
	/// authorization mechanism and must be implemented specifically for an application.
	///
	/// An implementation of Authorizer is responsible for the following:
	///   - Ensure that the current session, connection or request is properly 
	///     authenticated (e.g., via username and password, using HTTP Basic
	///     Authentication or another transport-specific authentication mechanism).
	///     For transports that support an Authenticator, this is usually taken
	///     care of by the Authenticator.
	///   - Check whether the user account associated with the request has
	///     the required permission for a specific method call.
{
public:
	using Ptr = Poco::AutoPtr<Authorizer>;

	Authorizer();
		/// Creates the Authorizer.

	virtual ~Authorizer();
		/// Destroys the Authorizer.
		
	virtual bool authorize(const std::string& method, const std::string& permission) = 0;
		/// Verifies that the request is properly authenticated (using a 
		/// transport-specific authentication mechanism, e.g. HTTP Basic
		/// Authentication) and checks whether the user account associated
		/// with the request has the given permission.
		///
		/// The given permission will be the the one specified with the @permission 
		/// attribute for the respective remote method. If no @permission attribute
		/// has been specified for a remote method, this method will not be called.
		///
		/// An implementation of this method will usually obtain the Context
		/// object associated with the request to retrieve authentication
		/// data (e.g., username and password, or any other transport-specific
		/// information).
		///
		/// Returns true if the request is authorized, otherwise false.
};


} } // namespace Poco::RemotingNG


#endif // RemotingNG_Authorizer_INCLUDED
