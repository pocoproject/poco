//
// HTTPURI.h
//
// Library: Foundation
// Package: URI
// Module:  URI
//
// Definition of the HTTPURI class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_HTTPURI_INCLUDED
#define Foundation_HTTPURI_INCLUDED


#include "Poco/URI.h"


namespace Poco {


class Path;


class Foundation_API HTTPURI : public URI
	/// An extended URI which allow to provide HTTP Headers
{
public:
	typedef std::vector<std::pair<std::string, std::string> > Headers;

	HTTPURI();
		/// Creates an empty URI.

	explicit HTTPURI(const std::string& uri);
		/// Parses an URI from the given string. Throws a
		/// SyntaxException if the uri is not valid.
		
	explicit HTTPURI(const char* uri);
		/// Parses an URI from the given string. Throws a
		/// SyntaxException if the uri is not valid.
		
	HTTPURI(const std::string& scheme, const std::string& pathEtc);
		/// Creates an URI from its parts.
		
	HTTPURI(const std::string& scheme, const std::string& authority, const std::string& pathEtc);
		/// Creates an URI from its parts.

	HTTPURI(const std::string& scheme, const std::string& authority, const std::string& path, const std::string& query);
		/// Creates an URI from its parts.

	HTTPURI(const std::string& scheme, const std::string& authority, const std::string& path, const std::string& query, const std::string& fragment);
		/// Creates an URI from its parts.

	HTTPURI(const HTTPURI& uri);
		/// Copy constructor. Creates an URI from another one.
		
	HTTPURI(const HTTPURI& baseURI, const std::string& relativeURI);
		/// Creates an URI from a base URI and a relative URI, according to
		/// the algorithm in section 5.2 of RFC 3986.

	explicit HTTPURI(const Path& path);
		/// Creates a URI from a path.
		///
		/// The path will be made absolute, and a file:// URI
		/// will be built from it.

	virtual ~HTTPURI();
		/// Destroys the URI.
	
	HTTPURI& operator = (const HTTPURI& uri);
		/// Assignment operator.
		
	HTTPURI& operator = (const std::string& uri);
		/// Parses and assigns an URI from the given string. Throws a
		/// SyntaxException if the uri is not valid.

	HTTPURI& operator = (const char* uri);
		/// Parses and assigns an URI from the given string. Throws a
		/// SyntaxException if the uri is not valid.
		
	void swap(HTTPURI& uri);
		/// Swaps the URI with another one.	
		
	void clear();
		/// Clears all parts of the URI.
		
	void addHeader(const std::string& headerName, const std::string& headerValue);
	
	void removeHeader(const std::string& headerName);
	
	const Headers & getHeaders();
private:
	Headers	_headers;
};

	
} // namespace Poco


#endif // Foundation_URI_INCLUDED
