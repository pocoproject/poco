//
// Headers.h
//
// Library: AI/MCP/HTTP
// Package: HTTP
// Module:  Headers
//
// The values of the MCP headers of the Streamable HTTP transport, which the
// client and the server transport share. Not installed.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AIMCPHTTP_Headers_INCLUDED
#define AIMCPHTTP_Headers_INCLUDED


#include "Poco/AI/MCP/HTTP/HTTP.h"
#include "Poco/JSON/Object.h"
#include "Poco/Dynamic/Var.h"
#include <string>
#include <vector>


namespace Poco::AI::MCP::HTTP {


[[nodiscard]] bool isToken(const std::string& name);
	/// Returns true if name is an HTTP field name token (RFC 9110).


[[nodiscard]] bool isPlain(const std::string& value);
	/// Returns true if value can be sent as it is: printable ASCII without
	/// leading or trailing spaces, and without "=?", which starts the base64
	/// form and an RFC 2047 encoded word.


[[nodiscard]] std::string headerValue(const std::string& value);
	/// The value as a header carries it: as it is, or in the base64 form
	/// =?base64?<base64>?=.


[[nodiscard]] bool decodeHeaderValue(const std::string& received, std::string& value);
	/// Sets value to what a header received carries: the decoded text of
	/// the base64 form, else the header as it is. Returns false for a base64
	/// form whose payload is not canonical base64 (padded, without line
	/// breaks or other characters) or does not decode to UTF-8.
	///
	/// Poco::Net::MessageHeader decodes RFC 2047 encoded words in every
	/// header it reads, and drops the "=?" of a value that only starts like
	/// one, as the base64 form does: a value that starts with "base64?" and
	/// ends with "?=" is taken as the base64 form without its "=?".


struct Annotation
	/// An x-mcp-header annotation of an input schema: the header name, the
	/// property path from the root of the arguments to the parameter, and
	/// the type of the parameter: string, integer or boolean.
{
	std::string name;
	std::vector<std::string> path;
	std::string type;
};


void collectAnnotations(const Poco::JSON::Object::Ptr& pSchema, std::vector<std::string>& path, std::vector<Annotation>& annotations);
	/// Collects the x-mcp-header annotations of a schema object reached from
	/// the root through properties keys only.
	///
	/// Throws Poco::DataFormatException for an invalid annotation (at the
	/// root, not a header name, on a parameter that is not a string, an
	/// integer or a boolean, a name used twice), or for one anywhere else.


[[nodiscard]] bool renderParam(const Poco::Dynamic::Var& value, std::string& text);
	/// Renders a parameter value as its Mcp-Param header carries it: a
	/// string as it is, a boolean as true or false, an integer within the
	/// safe range of JavaScript, a number without a fraction as such an
	/// integer. Returns false for a value that has no header.


[[nodiscard]] Poco::Dynamic::Var valueAt(const Poco::JSON::Object::Ptr& pArguments, const std::vector<std::string>& path);
	/// The value at the property path of the arguments, or an empty Var.


} // namespace Poco::AI::MCP::HTTP


#endif // AIMCPHTTP_Headers_INCLUDED
