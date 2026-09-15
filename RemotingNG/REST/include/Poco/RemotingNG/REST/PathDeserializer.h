//
// PathDeserializer.h
//
// Library: RemotingNG/REST
// Package: REST
// Module:  PathDeserializer
//
// Definition of the PathDeserializer class.
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef RemotingNG_REST_PathDeserializer_INCLUDED
#define RemotingNG_REST_PathDeserializer_INCLUDED


#include "Poco/RemotingNG/REST/REST.h"
#include "Poco/RemotingNG/REST/ScalarDeserializer.h"
#include "Poco/Net/HTTPRequest.h"
#include <map>


namespace Poco {
namespace RemotingNG {
namespace REST {


class RemotingNGREST_API PathDeserializer: public ScalarDeserializer
	/// The Path Deserializer implementation for the REST Transport.
	///
	/// Extracts parameter values from URI path segments based on a path template.
	/// The path template can contain placeholders in two formats:
	///   - Simple placeholder: {name} - matches any path segment
	///   - Alternation placeholder: {name:(option1|option2|...)} - matches only specified options
	///
	/// Examples:
	///   - Template "/api/user/{id}" matches "/api/user/123" and extracts id="123"
	///   - Template "/api/{endpoint:(query|config)}" matches "/api/query" and extracts endpoint="query"
	///
	/// For more information, please see the Deserializer documentation.
{
public:
	PathDeserializer(const std::string& pathTemplate, const Poco::Net::HTTPRequest& request);
		/// Creates the PathDeserializer using the given path template and HTTP request.
		/// The path template defines which segments contain parameters to extract.

	~PathDeserializer();
		/// Destroys the PathDeserializer.

	// Poco::RemotingNG::Deserializer
	bool deserializeNullableBegin(const std::string& name, bool isMandatory, bool& isNull);

protected:
	void parsePath(const std::string& pathTemplate, const Poco::Net::HTTPRequest& request);

	// ScalarDeserializer
	const std::string& deserializeData(const std::string& name, bool isMandatory, bool& found);

private:
	std::map<std::string, std::string> _values;
};


} } } // namespace Poco::RemotingNG::REST


#endif // RemotingNG_REST_PathDeserializer_INCLUDED
