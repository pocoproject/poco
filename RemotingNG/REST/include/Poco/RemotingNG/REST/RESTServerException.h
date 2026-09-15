//
// RESTServerException.h
//
// Library: RemotingNG/REST
// Package: REST
// Module:  RESTServerException
//
// Definition of the RESTServerException class.
//
// Copyright (c) 2024, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_REST_RESTServerException_INCLUDED
#define RemotingNG_REST_RESTServerException_INCLUDED


#include "Poco/RemotingNG/RemotingException.h"
#include "Poco/RemotingNG/REST/JSValue.h"


namespace Poco {
namespace RemotingNG {
namespace REST {


class RemotingNGREST_API RESTServerException: public Poco::RemotingNG::RemoteException
	/// This exception class contains detailed information from a REST 
	/// error received from the server.
{
public:
	RESTServerException(const std::string& reason, const std::string& message, int status, JSValue::Ptr pContent);
		/// Creates a RESTServerException with the given reason, message, HTTP status code and detail.

	RESTServerException(const std::string& reason, int status, JSValue::Ptr pDetail);
		/// Creates a RESTServerException with the given reason, HTTP status code and detail.

	RESTServerException(const RESTServerException& exc);
		/// Creates a new RESTServerException from another one.

	~RESTServerException() noexcept;
		/// Destroy the exception.

	RESTServerException& operator = (const RESTServerException& exc);
		/// Assignment operator.

	const char* name() const noexcept;
		/// Returns a static string describing the exception.

	const char* className() const noexcept;
		/// Returns the name of the exception class.

	Poco::Exception* clone() const;
		/// Creates an exact copy of the exception.

	void rethrow() const;
		/// (Re)Throws the exception.

	const std::string& reason() const;
		/// Returns the HTTP reason text of the server response.

	int status() const;
		/// Returns the HTTP status code of the server response.

	JSValue::Ptr content() const;
		/// Returns the JSON content of the server response.

	std::string error() const;
		/// Returns the value of the content's "error" property
		/// if it exists, otherwise an empty string.

	std::string detail() const;
		/// Returns the value of the content's "detail" property
		/// if it exists, otherwise an empty string.

private:
	std::string _reason;
	JSValue::Ptr _pContent;
};


//
// inlines
//


inline const std::string& RESTServerException::reason() const
{
	return _reason;
}

	
inline int RESTServerException::status() const
{
	return code();
}
	

inline JSValue::Ptr RESTServerException::content() const
{
	return _pContent;
}


} } } // namespace Poco::RemotingNG::REST


#endif // RemotingNG_REST_RESTServerException_INCLUDED
