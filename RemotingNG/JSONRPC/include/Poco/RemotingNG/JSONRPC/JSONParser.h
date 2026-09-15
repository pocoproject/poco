//
// JSONParser.h
//
// Library: RemotingNG/JSONRPC
// Package: JSONParser
// Module:  JSONParser
//
// Definition of the JSONParser class.
//
// Copyright (c) 2010-2013, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_JSONRPC_JSONParser_INCLUDED
#define RemotingNG_JSONRPC_JSONParser_INCLUDED


#include "Poco/RemotingNG/JSONRPC/JSONRPC.h"
#include "Poco/RemotingNG/JSONRPC/JSValue.h"
#include <istream>


namespace Poco {
namespace RemotingNG {
namespace JSONRPC {


class RemotingNGJSONRPC_API JSONParser
	/// A simple parser for JSON.
{
public:
	JSONParser(std::istream& istr);
		/// Creates a JSONParser for parsing the given stream.
	
	~JSONParser();
		/// Destroys the JSONParser.
		
	JSValue::Ptr parse();
		/// Reads a JSValue from the stream.

	void setMaxDepth(std::size_t maxDepth);
		/// Sets the maximum allowed depth of the JSON document.

	std::size_t getMaxDepth() const;
		/// Returns the maximum allowed depth of the JSON document.

	constexpr static std::size_t MAX_DEPTH = 50;

protected:
	int parseValue(int ch, JSValue::Ptr& pValue);
	int parseObject(int ch, JSValue::Ptr& pObject);
	int parseArray(int ch, JSValue::Ptr& pArray);
	int parseScalar(int ch, JSValue::Ptr& pScalar);
	int parseString(int ch, std::string& str);
	int parseUnicode(int ch, std::string& str);
	int skipWhitespace(int ch);
	static bool isWhitespace(int ch);
	static bool isEOF(int ch);

private:
	JSONParser();
	JSONParser(const JSONParser&);
	JSONParser& operator = (const JSONParser&);
	
	std::streambuf& _streambuf;
	std::size_t _depth = 0;
	std::size_t _maxDepth = MAX_DEPTH;
};


//
// inlines
//
inline void JSONParser::setMaxDepth(std::size_t maxDepth)
{
	_maxDepth = maxDepth;
}


inline std::size_t JSONParser::getMaxDepth() const
{
	return _maxDepth;
}


inline bool JSONParser::isWhitespace(int ch)
{
	return ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n' || ch == '\f';
}


inline bool JSONParser::isEOF(int ch)
{
	return ch == -1;
}


} } } // namespace Poco::RemotingNG::JSONRPC


#endif // RemotingNG_JSONRPC_JSONParser_INCLUDED
