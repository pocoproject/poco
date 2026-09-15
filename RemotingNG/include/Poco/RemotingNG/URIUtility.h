//
// URIUtility.h
//
// Library: RemotingNG
// Package: RemotingNG
// Module:  URIUtility
//
// Definition of the URIUtility class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#ifndef RemotingNG_URIUtility_INCLUDED
#define RemotingNG_URIUtility_INCLUDED


#include "Poco/RemotingNG/Identifiable.h"


namespace Poco {
namespace RemotingNG {


class RemotingNG_API URIUtility
	/// This class provides utility methods for parsing and creating RemotingNG URIs.
{
public:
	static std::string createURIPath(const Identifiable::ObjectId& oid, const Identifiable::TypeId& tid, const std::string& protocol);
		/// Creates an URI path from the given input parameters.
		
	static std::string createURIAuthority(const std::string& host, Poco::UInt16 port);
		/// Creates an URI authority (e.g. host:port) from the given parameters.

	static void parseURIPath(const std::string& uri, Identifiable::ObjectId& oid, Identifiable::TypeId& tid, std::string& protocol);
		/// Parses an URI containing a path created by createURIPath().
		///
		/// Throws an exception if the URI format is not recognized.

	static bool matchPath(const std::string& path, const std::string& pathTemplate);
		/// Matches the given path against the path template. A path template can contain
		/// placeholders for certain path segments. The format of a place holder is {<identifier>},
		/// an identifier enclosed in curly brackets. For example, the path "/object/12345"
		/// will match the path template "/object/{id}".
		///
		/// The path template can also contain alternation patterns using parentheses and
		/// pipe characters. Two syntaxes are supported:
		///   - Standalone: (option1|option2|option3)
		///   - Named (for parameter extraction): {name:(option1|option2|option3)}
		///
		/// For example, the path "/api/query" will match the path template
		/// "/api/(query|config|status)" or "/api/{endpoint:(query|config|status)}".
		/// The named syntax allows the matched value to be extracted as a parameter.
		///
		/// Returns true if the path matches the pathTemplate, otherwise false.

private:
	static bool matchAlternatives(const std::string& segment, std::string::const_iterator& itTempl, std::string::const_iterator endTempl);
		/// Matches segment against pipe-separated alternatives in template.
		/// Iterator must point to first alternative (after opening '(').
		/// On return, iterator points past closing ')'.
};


} } // namespace Poco::RemotingNG


#endif // RemotingNG_URIUtility_INCLUDED
