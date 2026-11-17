//
// URI.cpp
//
// Library: Foundation
// Package: URI
// Module:  URI
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/URI.h"
#include "Poco/NumberFormatter.h"
#include "Poco/Exception.h"
#include "Poco/String.h"
#include "Poco/NumberParser.h"
#include "Poco/NumericString.h"
#include "Poco/Path.h"
#include "Poco/Ascii.h"


namespace Poco {


const std::string URI::RESERVED_PATH        = "?#";
const std::string URI::RESERVED_QUERY       = "?#/:;+@";
const std::string URI::RESERVED_QUERY_PARAM = "?#/:;+@&=";
const std::string URI::RESERVED_FRAGMENT    = "";
const std::string URI::ILLEGAL              = "%<>{}|\\\"^`!*'()$,[]";


namespace {


constexpr std::string::size_type ESCAPE_LENGTH = 3;
	/// The length of a percent-encoded character: '%' and two hex digits.


constexpr char ASCII_SPACE = 0x20;
	/// The space character; encode() percent-encodes it and the control characters below it.


constexpr char ASCII_DEL = 0x7F;
	/// The delete control character, which encode() percent-encodes.


[[nodiscard]] constexpr bool isUnreserved(char c)
	/// Returns true if c is an unreserved character of RFC 3986, section 2.3:
	/// ALPHA, DIGIT, '-', '.', '_' or '~'. The letters and digits are checked as
	/// character ranges, so that non-ASCII bytes are not unreserved.
{
	return (c >= 'a' && c <= 'z')
		|| (c >= 'A' && c <= 'Z')
		|| (c >= '0' && c <= '9')
		|| c == '-' || c == '.' || c == '_' || c == '~';
}


[[nodiscard]] std::string decodeDotSegments(const std::string& rawPath)
	/// Returns rawPath with the segments that decode to "." or ".." in literal form.
{
	std::string result;
	std::string::size_type start = 0;
	while (true)
	{
		const std::string::size_type slash = rawPath.find('/', start);
		const std::string segment = rawPath.substr(start, slash - start);
		std::string decodedSegment;
		URI::decode(segment, decodedSegment);
		if (decodedSegment == "." || decodedSegment == "..")
			result += decodedSegment;
		else
			result += segment;
		if (slash == std::string::npos) break;
		result += '/';
		start = slash + 1;
	}
	return result;
}


[[nodiscard]] std::string normalizePercentEncoding(const std::string& rawPath)
	/// Decodes the percent-encoded unreserved characters of rawPath and upper-cases
	/// the hex digits of the other percent-encoded characters (RFC 3986, section 6.2.2).
	/// Every '%' in rawPath must start a well-formed percent-encoded character, which
	/// getRawPath() guarantees, as decode() checks every stored raw path.
{
	std::string result;
	std::string::size_type pos = 0;
	while (pos < rawPath.size())
	{
		if (rawPath[pos] == '%')
		{
			const char* const hexBegin = rawPath.data() + pos + 1;
			const char* const hexEnd = rawPath.data() + pos + ESCAPE_LENGTH;
			unsigned char c = 0;
			if (strToInt(hexBegin, hexEnd, c, NumberParser::NUM_BASE_HEX) && isUnreserved(static_cast<char>(c)))
			{
				result += static_cast<char>(c);
			}
			else
			{
				result += '%';
				for (const char* it = hexBegin; it != hexEnd; ++it)
					result += static_cast<char>(Ascii::toUpper(*it));
			}
			pos += ESCAPE_LENGTH;
		}
		else result += rawPath[pos++];
	}
	return result;
}


} // namespace


URI::URI():
	_port(0)
{
}


URI::URI(const std::string& uri):
	_port(0)
{
	parse(uri);
}


URI::URI(const char* uri):
	_port(0)
{
	parse(std::string(uri));
}


URI::URI(const std::string& scheme, const std::string& pathEtc):
	_scheme(scheme),
	_port(0)
{
	toLowerInPlace(_scheme);
	std::string::const_iterator beg = pathEtc.begin();
	std::string::const_iterator end = pathEtc.end();
	parsePathEtc(beg, end);
}


URI::URI(const std::string& scheme, const std::string& authority, const std::string& pathEtc):
	_scheme(scheme),
	_port(0)
{
	toLowerInPlace(_scheme);
	std::string::const_iterator beg = authority.begin();
	std::string::const_iterator end = authority.end();
	parseAuthority(beg, end);
	beg = pathEtc.begin();
	end = pathEtc.end();
	parsePathEtc(beg, end);
}


URI::URI(const std::string& scheme, const std::string& authority, const std::string& path, const std::string& query):
	_scheme(scheme),
	_port(0),
	_path(path),
	_query(query)
{
	checkControlChars(_query);
	toLowerInPlace(_scheme);
	std::string::const_iterator beg = authority.begin();
	std::string::const_iterator end = authority.end();
	parseAuthority(beg, end);
}


URI::URI(const std::string& scheme, const std::string& authority, const std::string& path, const std::string& query, const std::string& fragment):
	_scheme(scheme),
	_port(0),
	_path(path),
	_query(query),
	_fragment(fragment)
{
	checkControlChars(_query);
	checkControlChars(_fragment);
	toLowerInPlace(_scheme);
	std::string::const_iterator beg = authority.begin();
	std::string::const_iterator end = authority.end();
	parseAuthority(beg, end);
}


URI::URI(const URI& uri):
	_scheme(uri._scheme),
	_userInfo(uri._userInfo),
	_host(uri._host),
	_port(uri._port),
	_path(uri._path),
	_rawPath(uri._rawPath),
	_query(uri._query),
	_fragment(uri._fragment)
{
}


URI::URI(URI&& uri) noexcept:
	_scheme(std::move(uri._scheme)),
	_userInfo(std::move(uri._userInfo)),
	_host(std::move(uri._host)),
	_port(std::move(uri._port)),
	_path(std::move(uri._path)),
	_rawPath(std::move(uri._rawPath)),
	_query(std::move(uri._query)),
	_fragment(std::move(uri._fragment))
{
}


URI::URI(const URI& baseURI, const std::string& relativeURI):
	_scheme(baseURI._scheme),
	_userInfo(baseURI._userInfo),
	_host(baseURI._host),
	_port(baseURI._port),
	_path(baseURI._path),
	_rawPath(baseURI._rawPath),
	_query(baseURI._query),
	_fragment(baseURI._fragment)
{
	resolve(relativeURI);
}


URI::URI(const Path& path):
	_scheme("file"),
	_port(0)
{
	Path absolutePath(path);
	absolutePath.makeAbsolute();
	_path = absolutePath.toString(Path::PATH_UNIX);
}


URI::~URI()
{
}


URI& URI::operator = (const URI& uri)
{
	if (&uri != this)
	{
		_scheme   = uri._scheme;
		_userInfo = uri._userInfo;
		_host     = uri._host;
		_port     = uri._port;
		copyPath(uri);
		_query    = uri._query;
		_fragment = uri._fragment;
	}
	return *this;
}


URI& URI::operator = (URI&& uri) noexcept
{
	_scheme   = std::move(uri._scheme);
	_userInfo = std::move(uri._userInfo);
	_host     = std::move(uri._host);
	_port     = std::move(uri._port);
	_path     = std::move(uri._path);
	_rawPath  = std::move(uri._rawPath);
	_query    = std::move(uri._query);
	_fragment = std::move(uri._fragment);

	return *this;
}


URI& URI::operator = (const std::string& uri)
{
	clear();
	parse(uri);
	return *this;
}


URI& URI::operator = (const char* uri)
{
	clear();
	parse(std::string(uri));
	return *this;
}


void URI::swap(URI& uri) noexcept
{
	std::swap(_scheme, uri._scheme);
	std::swap(_userInfo, uri._userInfo);
	std::swap(_host, uri._host);
	std::swap(_port, uri._port);
	std::swap(_path, uri._path);
	std::swap(_rawPath, uri._rawPath);
	std::swap(_query, uri._query);
	std::swap(_fragment, uri._fragment);
}


void URI::clear()
{
	_scheme.clear();
	_userInfo.clear();
	_host.clear();
	_port = 0;
	_path.clear();
	_rawPath.clear();
	_query.clear();
	_fragment.clear();
}


std::string URI::toString() const
{
	std::string uri;
	if (isRelative())
	{
		uri = getRawPath();
	}
	else
	{
		uri = _scheme;
		uri += ':';
		std::string auth = getAuthority();
		if (!auth.empty() || _scheme == "file")
		{
			uri.append("//");
			uri.append(auth);
		}
		if (!_path.empty())
		{
			if (!auth.empty() && _path[0] != '/')
				uri += '/';
			uri += getRawPath();
		}
		else if (!_query.empty() || !_fragment.empty())
		{
			uri += '/';
		}
	}
	if (!_query.empty())
	{
		uri += '?';
		uri.append(_query);
	}
	if (!_fragment.empty())
	{
		uri += '#';
		uri.append(_fragment);
	}
	return uri;
}


void URI::setScheme(const std::string& scheme)
{
	_scheme = scheme;
	toLowerInPlace(_scheme);
}


void URI::setUserInfo(const std::string& userInfo)
{
	std::string decodedUserInfo;
	decode(userInfo, decodedUserInfo);
	checkControlChars(decodedUserInfo);
	_userInfo = std::move(decodedUserInfo);
}


void URI::setHost(const std::string& host)
{
	checkControlChars(host);
	_host = host;
}


unsigned short URI::getPort() const
{
	if (_port == 0)
		return getWellKnownPort();
	else
		return _port;
}


void URI::setPort(unsigned short port)
{
	_port = port;
}


std::string URI::getAuthority() const
{
	std::string auth;
	if (!_userInfo.empty())
	{
		auth.append(_userInfo);
		auth += '@';
	}
	if (_host.find(':') != std::string::npos)
	{
		auth += '[';
		auth += _host;
		auth += ']';
	}
	else auth.append(_host);
	if (_port && !isWellKnownPort())
	{
		auth += ':';
		NumberFormatter::append(auth, _port);
	}
	return auth;
}


void URI::setAuthority(const std::string& authority)
{
	_userInfo.clear();
	_host.clear();
	_port = 0;
	std::string::const_iterator beg = authority.begin();
	std::string::const_iterator end = authority.end();
	parseAuthority(beg, end);
}


void URI::setPath(const std::string& path)
{
	// path may refer to _path, and a malformed escape must leave the URI unchanged.
	std::string decodedPath;
	decode(path, decodedPath);
	_path = std::move(decodedPath);
	_rawPath.clear();
}


std::string URI::getRawPath() const
{
	if (!_rawPath.empty() && isRawPathUsable(_rawPath))
		return _rawPath;
	std::string rawPath;
	encode(_path, RESERVED_PATH, rawPath);
	return rawPath;
}


void URI::setRawPath(const std::string& path)
{
	checkControlChars(path);
	if (path.find_first_of("?# ") != std::string::npos)
		throw URISyntaxException("URI path contains a character that must be percent-encoded", path);
	std::string decodedPath;
	decode(path, decodedPath);
	// Without '%', the raw path equals the percent-encoded _path that getRawPath() returns.
	std::string rawPath;
	if (path.find('%') != std::string::npos)
		rawPath = encodeKeepingEscapes(path, RESERVED_PATH);
	// Moving a std::string does not throw, so an exception above leaves both members unchanged.
	_path    = std::move(decodedPath);
	_rawPath = std::move(rawPath);
}


void URI::setRawQuery(const std::string& query)
{
	checkControlChars(query);
	_query = query;
}


void URI::setQuery(const std::string& query)
{
	_query.clear();
	encode(query, RESERVED_QUERY, _query);
}


void URI::addQueryParameter(const std::string& param, const std::string& val)
{
	if (!_query.empty()) _query += '&';
	encode(param, RESERVED_QUERY_PARAM, _query);
	_query += '=';
	encode(val, RESERVED_QUERY_PARAM, _query);
}


std::string URI::getQuery() const
{
	std::string query;
	decode(_query, query);
	return query;
}


URI::QueryParameters URI::getQueryParameters(bool plusIsSpace) const
{
	QueryParameters result;
	std::string::const_iterator it(_query.begin());
	std::string::const_iterator end(_query.end());
	while (it != end)
	{
		std::string name;
		std::string value;
		while (it != end && *it != '=' && *it != '&')
		{
			if (plusIsSpace && (*it == '+'))
				name += ' ';
			else
				name += *it;
			++it;
		}
		if (it != end && *it == '=')
		{
			++it;
			while (it != end && *it != '&')
			{
				if (plusIsSpace && (*it == '+'))
					value += ' ';
				else
					value += *it;
				++it;
			}
		}
		std::string decodedName;
		std::string decodedValue;
		URI::decode(name, decodedName);
		URI::decode(value, decodedValue);
		result.push_back(std::make_pair(decodedName, decodedValue));
		if (it != end && *it == '&') ++it;
	}
	return result;
}


void URI::setQueryParameters(const QueryParameters& params)
{
	_query.clear();
	for (const auto& p: params)
	{
		addQueryParameter(p.first, p.second);
	}
}


std::string URI::getFragment() const
{
	std::string fragment;
	decode(_fragment, fragment);
	return fragment;
}


void URI::setFragment(const std::string& fragment)
{
	_fragment.clear();
	encode(fragment, RESERVED_FRAGMENT, _fragment);
}


void URI::setRawFragment(const std::string& fragment)
{
	checkControlChars(fragment);
	_fragment = fragment;
}


void URI::setPathEtc(const std::string& pathEtc)
{
	_path.clear();
	_rawPath.clear();
	_query.clear();
	_fragment.clear();
	std::string::const_iterator beg = pathEtc.begin();
	std::string::const_iterator end = pathEtc.end();
	parsePathEtc(beg, end);
}


std::string URI::getPathEtc() const
{
	std::string pathEtc = getRawPath();
	if (!_query.empty())
	{
		pathEtc += '?';
		pathEtc += _query;
	}
	if (!_fragment.empty())
	{
		pathEtc += '#';
		pathEtc += _fragment;
	}
	return pathEtc;
}


std::string URI::getPathAndQuery() const
{
	std::string pathAndQuery = getRawPath();
	if (!_query.empty())
	{
		pathAndQuery += '?';
		pathAndQuery += _query;
	}
	return pathAndQuery;
}


void URI::resolve(const std::string& relativeURI)
{
	URI parsedURI(relativeURI);
	resolve(parsedURI);
}


void URI::resolve(const URI& relativeURI)
{
	if (!relativeURI._scheme.empty())
	{
		_scheme   = relativeURI._scheme;
		_userInfo = relativeURI._userInfo;
		_host     = relativeURI._host;
		_port     = relativeURI._port;
		copyPath(relativeURI);
		_query    = relativeURI._query;
		removeDotSegments();
	}
	else
	{
		if (!relativeURI._host.empty())
		{
			_userInfo = relativeURI._userInfo;
			_host     = relativeURI._host;
			_port     = relativeURI._port;
			copyPath(relativeURI);
			_query    = relativeURI._query;
			removeDotSegments();
		}
		else
		{
			if (relativeURI._path.empty())
			{
				if (!relativeURI._query.empty())
					_query = relativeURI._query;
			}
			else
			{
				if (relativeURI._path[0] == '/')
				{
					copyPath(relativeURI);
					removeDotSegments();
				}
				else if (_rawPath.empty() && relativeURI._rawPath.empty())
				{
					mergePath(relativeURI._path);
				}
				else
				{
					URI rawURI = rawPathURI();
					rawURI.mergePath(decodeDotSegments(relativeURI.getRawPath()));
					mergePath(relativeURI._path);
					setRawPathIfEquivalent(rawURI._path);
				}
				_query = relativeURI._query;
			}
		}
	}
	_fragment = relativeURI._fragment;
}


bool URI::isRelative() const
{
	return _scheme.empty();
}


bool URI::empty() const
{
	return _scheme.empty() && _host.empty() && _path.empty() && _query.empty() && _fragment.empty();
}


bool URI::operator == (const URI& uri) const
{
	return equals(uri);
}


bool URI::operator == (const std::string& uri) const
{
	URI parsedURI(uri);
	return equals(parsedURI);
}


bool URI::operator != (const URI& uri) const
{
	return !equals(uri);
}


bool URI::operator != (const std::string& uri) const
{
	URI parsedURI(uri);
	return !equals(parsedURI);
}


bool URI::equals(const URI& uri) const
{
	// A normalized raw path decodes to the same string as the raw path, so different
	// decoded paths imply different normalized raw paths; normalizing runs last.
	// Without stored raw paths, equal decoded paths have equal encoded forms.
	return _scheme   == uri._scheme
		&& _userInfo == uri._userInfo
		&& _host     == uri._host
		&& getPort() == uri.getPort()
		&& _path     == uri._path
		&& _query    == uri._query
		&& _fragment == uri._fragment
		&& ((_rawPath.empty() && uri._rawPath.empty())
			|| normalizePercentEncoding(getRawPath()) == normalizePercentEncoding(uri.getRawPath()));
}


void URI::normalize()
{
	removeDotSegments(!isRelative());
}


void URI::removeDotSegments(bool removeLeading)
{
	if (_path.empty()) return;

	std::string rawPath;
	if (!_rawPath.empty())
	{
		URI rawURI = rawPathURI();
		rawURI.removeDotSegments(removeLeading);
		rawPath = std::move(rawURI._path);
	}
	bool leadingSlash  = *(_path.begin()) == '/';
	bool trailingSlash = *(_path.rbegin()) == '/';
	std::vector<std::string> segments;
	std::vector<std::string> normalizedSegments;
	getPathSegments(segments);
	for (const auto& s: segments)
	{
		if (s == "..")
		{
			if (!normalizedSegments.empty())
			{
				if (normalizedSegments.back() == "..")
					normalizedSegments.push_back(s);
				else
					normalizedSegments.pop_back();
			}
			else if (!removeLeading)
			{
				normalizedSegments.push_back(s);
			}
		}
		else if (s != ".")
		{
			normalizedSegments.push_back(s);
		}
	}
	buildPath(normalizedSegments, leadingSlash, trailingSlash);
	setRawPathIfEquivalent(rawPath);
}


void URI::getPathSegments(std::vector<std::string>& segments) const
{
	getPathSegments(_path, segments);
}


void URI::getPathSegments(const std::string& path, std::vector<std::string>& segments)
{
	std::string::const_iterator it  = path.begin();
	std::string::const_iterator end = path.end();
	std::string seg;
	while (it != end)
	{
		if (*it == '/')
		{
			if (!seg.empty())
			{
				segments.push_back(seg);
				seg.clear();
			}
		}
		else seg += *it;
		++it;
	}
	if (!seg.empty())
		segments.push_back(seg);
}


void URI::encode(const std::string& str, const std::string& reserved, std::string& encodedStr)
{
	for (auto c: str)
		encodeChar(c, reserved, encodedStr);
}


void URI::encodeChar(char c, const std::string& reserved, std::string& encodedStr)
{
	if (isUnreserved(c))
	{
		encodedStr += c;
	}
	else if (c <= ASCII_SPACE || c >= ASCII_DEL || ILLEGAL.find(c) != std::string::npos || reserved.find(c) != std::string::npos)
	{
		encodedStr += '%';
		encodedStr += NumberFormatter::formatHex(static_cast<unsigned>(static_cast<unsigned char>(c)), 2);
	}
	else encodedStr += c;
}


std::string URI::encodeKeepingEscapes(const std::string& str, const std::string& reserved)
{
	std::string encodedStr;
	encodedStr.reserve(str.size());
	std::string::size_type pos = 0;
	while (pos < str.size())
	{
		if (str[pos] == '%')
		{
			encodedStr.append(str, pos, ESCAPE_LENGTH);
			pos += ESCAPE_LENGTH;
		}
		else encodeChar(str[pos++], reserved, encodedStr);
	}
	return encodedStr;
}


void URI::decode(const std::string& str, std::string& decodedStr, bool plusAsSpace)
{
	bool inQuery = false;
	std::string::const_iterator it  = str.begin();
	std::string::const_iterator end = str.end();
	while (it != end)
	{
		char c = *it++;
		if (c == '?') inQuery = true;
		// spaces may be encoded as plus signs in the query
		if (inQuery && plusAsSpace && c == '+') c = ' ';
		else if (c == '%')
		{
			if (it == end) throw URISyntaxException("URI encoding: no hex digit following percent sign", str);
			char hi = *it++;
			if (it == end) throw URISyntaxException("URI encoding: two hex digits must follow percent sign", str);
			char lo = *it++;
			if (hi >= '0' && hi <= '9')
				c = hi - '0';
			else if (hi >= 'A' && hi <= 'F')
				c = hi - 'A' + 10;
			else if (hi >= 'a' && hi <= 'f')
				c = hi - 'a' + 10;
			else throw URISyntaxException("URI encoding: not a hex digit");
			c *= 16;
			if (lo >= '0' && lo <= '9')
				c += lo - '0';
			else if (lo >= 'A' && lo <= 'F')
				c += lo - 'A' + 10;
			else if (lo >= 'a' && lo <= 'f')
				c += lo - 'a' + 10;
			else throw URISyntaxException("URI encoding: not a hex digit");
		}
		decodedStr += c;
	}
}


bool URI::isWellKnownPort() const
{
	return _port == getWellKnownPort();
}


unsigned short URI::getWellKnownPort() const
{
	if (_scheme == "ftp")
		return 21;
	else if (_scheme == "ssh")
		return 22;
	else if (_scheme == "telnet")
		return 23;
	else if (_scheme == "smtp")
		return 25;
	else if (_scheme == "dns")
		return 53;
	else if (_scheme == "http" || _scheme == "ws")
		return 80;
	else if (_scheme == "nntp")
		return 119;
	else if (_scheme == "imap")
		return 143;
	else if (_scheme == "ldap")
		return 389;
	else if (_scheme == "https" || _scheme == "wss")
		return 443;
	else if (_scheme == "smtps")
		return 465;
	else if (_scheme == "rtsp")
		return 554;
	else if (_scheme == "ldaps")
		return 636;
	else if (_scheme == "dnss")
		return 853;
	else if (_scheme == "imaps")
		return 993;
	else if (_scheme == "sip")
		return 5060;
	else if (_scheme == "sips")
		return 5061;
	else if (_scheme == "xmpp")
		return 5222;
	else
		return 0;
}


void URI::parse(const std::string& uri)
{
	std::string::const_iterator it  = uri.begin();
	std::string::const_iterator end = uri.end();
	if (it == end) return;
	if (*it != '/' && *it != '.' && *it != '?' && *it != '#')
	{
		std::string scheme;
		while (it != end && *it != ':' && *it != '?' && *it != '#' && *it != '/') scheme += *it++;
		if (it != end && *it == ':')
		{
			++it;
			if (it == end) throw URISyntaxException("URI scheme must be followed by authority or path", uri);
			setScheme(scheme);
			if (*it == '/')
			{
				++it;
				if (it != end && *it == '/')
				{
					++it;
					parseAuthority(it, end);
				}
				else --it;
			}
			parsePathEtc(it, end);
		}
		else
		{
			it = uri.begin();
			parsePathEtc(it, end);
		}
	}
	else parsePathEtc(it, end);
}


void URI::parseAuthority(std::string::const_iterator& it, const std::string::const_iterator& end)
{
	std::string userInfo;
	std::string part;
	while (it != end && *it != '/' && *it != '?' && *it != '#')
	{
		if (*it == '@')
		{
			userInfo = part;
			part.clear();
		}
		else part += *it;
		++it;
	}
	checkControlChars(userInfo);
	std::string::const_iterator pbeg = part.begin();
	std::string::const_iterator pend = part.end();
	parseHostAndPort(pbeg, pend);
	_userInfo = userInfo;
}


void URI::parseHostAndPort(std::string::const_iterator& it, const std::string::const_iterator& end)
{
	if (it == end) return;
	std::string host;
	if (*it == '[')
	{
		// IPv6 address
		++it;
		while (it != end && *it != ']') host += *it++;
		if (it == end) throw URISyntaxException("unterminated IPv6 address");
		++it;
	}
	else
	{
		while (it != end && *it != ':') host += *it++;
	}
	if (it != end && *it == ':')
	{
		++it;
		std::string port;
		while (it != end) port += *it++;
		if (!port.empty())
		{
			int nport = 0;
			if (NumberParser::tryParse(port, nport) && nport > 0 && nport < 65536)
				_port = (unsigned short) nport;
			else
				throw URISyntaxException("bad or invalid port number", port);
		}
		else _port = 0;
	}
	else _port = 0;
	checkControlChars(host);
	_host = host;
	if (_host.size() && _host[0] != '%')
		toLowerInPlace(_host);
}


void URI::parsePath(std::string::const_iterator& it, const std::string::const_iterator& end)
{
	std::string path;
	while (it != end && *it != '?' && *it != '#') path += *it++;
	decode(path, _path);
	if (path.find('%') != std::string::npos)
		_rawPath = encodeKeepingEscapes(path, RESERVED_PATH);
}


void URI::parsePathEtc(std::string::const_iterator& it, const std::string::const_iterator& end)
{
	if (it == end) return;
	if (*it != '?' && *it != '#')
		parsePath(it, end);
	if (it != end && *it == '?')
	{
		++it;
		parseQuery(it, end);
	}
	if (it != end && *it == '#')
	{
		++it;
		parseFragment(it, end);
	}
}


void URI::parseQuery(std::string::const_iterator& it, const std::string::const_iterator& end)
{
	std::string query;
	while (it != end && *it != '#') query += *it++;
	checkControlChars(query);
	_query = std::move(query);
}


void URI::parseFragment(std::string::const_iterator& it, const std::string::const_iterator& end)
{
	std::string fragment;
	while (it != end) fragment += *it++;
	checkControlChars(fragment);
	_fragment = std::move(fragment);
}


void URI::mergePath(const std::string& path)
{
	std::vector<std::string> segments;
	std::vector<std::string> normalizedSegments;
	bool addLeadingSlash = false;
	if (!_path.empty())
	{
		getPathSegments(segments);
		bool endsWithSlash = *(_path.rbegin()) == '/';
		if (!endsWithSlash && !segments.empty())
			segments.pop_back();
		addLeadingSlash = _path[0] == '/';
	}
	getPathSegments(path, segments);
	addLeadingSlash = addLeadingSlash || (!path.empty() && path[0] == '/');
	bool hasTrailingSlash = (!path.empty() && *(path.rbegin()) == '/');
	bool addTrailingSlash = false;
	for (const auto& s: segments)
	{
		if (s == "..")
		{
			addTrailingSlash = true;
			if (!normalizedSegments.empty())
				normalizedSegments.pop_back();
		}
		else if (s != ".")
		{
			addTrailingSlash = false;
			normalizedSegments.push_back(s);
		}
		else addTrailingSlash = true;
	}
	buildPath(normalizedSegments, addLeadingSlash, hasTrailingSlash || addTrailingSlash);
}


void URI::buildPath(const std::vector<std::string>& segments, bool leadingSlash, bool trailingSlash)
{
	_path.clear();
	_rawPath.clear();
	bool first = true;
	for (const auto& s: segments)
	{
		if (first)
		{
			first = false;
			if (leadingSlash)
				_path += '/';
			else if (_scheme.empty() && s.find(':') != std::string::npos)
				_path.append("./");
		}
		else _path += '/';
		_path.append(s);
	}
	if (trailingSlash)
		_path += '/';
}


void URI::checkControlChars(const std::string& str)
{
	for (auto c: str)
	{
		if (Ascii::hasProperties(c, Ascii::ACP_CONTROL))
			throw URISyntaxException("URI contains control characters", str);
	}
}


URI URI::rawPathURI() const
{
	// The path algorithms split at '/' and compare segments with "." and "..";
	// on the raw path "%2F" then stays inside its segment.
	URI rawURI;
	rawURI._scheme = _scheme;
	rawURI._path = decodeDotSegments(getRawPath());
	return rawURI;
}


bool URI::isRawPathUsable(const std::string& rawPath) const
{
	const bool rawLeadingSlash = !rawPath.empty() && rawPath[0] == '/';
	const bool leadingSlash = !_path.empty() && _path[0] == '/';
	return rawLeadingSlash == leadingSlash || (getAuthority().empty() && _scheme != "file");
}


void URI::setRawPathIfEquivalent(const std::string& rawPath)
{
	std::string decodedPath;
	decode(rawPath, decodedPath);
	if (rawPath.find('%') != std::string::npos && decodedPath == _path && isRawPathUsable(rawPath))
		_rawPath = rawPath;
	else
		_rawPath.clear();
}


void URI::copyPath(const URI& uri)
{
	std::string rawPath = uri._rawPath;
	_path    = uri._path;
	_rawPath = std::move(rawPath);
}


} // namespace Poco
