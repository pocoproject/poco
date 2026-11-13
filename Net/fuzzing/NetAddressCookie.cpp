//
// NetAddressCookie.cpp
//
// Fuzzer for the Net address/cookie/media-type parsing surface.
//
// Threat model:
// - Untrusted input: IP address strings, host:port endpoint strings, cookie
//   attribute bags and Content-Type headers - exactly what an attacker
//   controls in request URLs, X-Forwarded-For headers, Set-Cookie response
//   headers and proxy configuration.
// - Trust boundary: IPAddress::parse()/tryParse(), the SocketAddress string
//   forms, HTTPCookie's NameValueCollection parser and MediaType::parse().
// - As of the 2026-11-10 coverage report these regions sit at 0% project-wide:
//   Net/src/IPAddress.cpp (413 lines), IPAddressImpl.cpp (412),
//   SocketAddress.cpp (277), SocketAddressImpl.cpp (77), HTTPCookie.cpp (278),
//   plus dark parts of HTTPResponse.cpp and MediaType.cpp. No existing
//   harness reaches them (verified via the reports-by-target union).
// - DNS note: SocketAddress's string constructors fall back to DNS::hostByName
//   for non-numeric hosts - a network path that must not run under fuzzing.
//   The harness therefore only invokes the string forms after
//   IPAddress::tryParse() has accepted the host part, so every SocketAddress
//   string call is guaranteed DNS-free while the host:port and bracket
//   splitting logic still gets fuzzed with mutated numeric hosts.
// - What counts as a bug: memory-safety violations on parse or binary
//   address construction (ASan/UBSan findings), cookie/attribute handling
//   crashes, media-type parameter parsing crashes.
//

#include "Poco/Net/HTTPCookie.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/IPAddress.h"
#include "Poco/Net/MediaType.h"
#include "Poco/Net/NameValueCollection.h"
#include "Poco/Net/SocketAddress.h"

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

using namespace Poco;
using namespace Poco::Net;


namespace
{

template <typename F>
void ignore(const F& func)
{
try
{
func();
}
catch (const std::exception&)
{
}
}


void fuzzIPAddress(const std::string& addr)
{
ignore([&] { [[maybe_unused]] IPAddress ip = IPAddress::parse(addr); });

IPAddress parsed;
if (!IPAddress::tryParse(addr, parsed))
return;

ignore([&] { [[maybe_unused]] std::string s = parsed.toString(); });
ignore([&] { [[maybe_unused]] IPAddress::Family f = parsed.family(); });
ignore([&] { [[maybe_unused]] bool v = parsed.isLoopback(); });
ignore([&] { [[maybe_unused]] bool v = parsed.isLinkLocal(); });
ignore([&] { [[maybe_unused]] bool v = parsed.isMulticast(); });
ignore([&] { [[maybe_unused]] bool v = parsed.isSiteLocal(); });
ignore([&] { [[maybe_unused]] bool v = parsed.isGlobalMC(); });
ignore([&] { [[maybe_unused]] bool v = parsed.isWildcard(); });
ignore([&] { [[maybe_unused]] bool v = parsed.isBroadcast(); });
ignore([&] { [[maybe_unused]] unsigned v = parsed.prefixLength(); });
ignore([&] { [[maybe_unused]] unsigned v = parsed.scope(); });

// Round-trip: re-parse the serialized form.
ignore([&]
{
IPAddress reparsed = IPAddress::parse(parsed.toString());
[[maybe_unused]] bool same = reparsed == parsed;
});
}


void fuzzSocketAddress(const std::string& addr, const std::string& addr2, std::uint16_t port)
{
IPAddress host;
if (!IPAddress::tryParse(addr, host))
return; // never let SocketAddress's string forms reach DNS

ignore([&]
{
SocketAddress sa(host, port);
[[maybe_unused]] std::string s = sa.toString();
[[maybe_unused]] IPAddress h = sa.host();
[[maybe_unused]] std::uint16_t p = sa.port();
[[maybe_unused]] SocketAddress::Family f = sa.family();
[[maybe_unused]] const struct sockaddr* a = sa.addr();
});

// host:port and [host]:port splitting with a numeric host stays DNS-free.
const std::string hostPort = addr + ":" + std::to_string(port);
ignore([&]
{
SocketAddress sa(hostPort);
[[maybe_unused]] std::uint16_t p = sa.port();
});

const std::string bracket = "[" + addr + "]:" + std::to_string(port);
ignore([&]
{
SocketAddress sa(bracket);
[[maybe_unused]] std::uint16_t p = sa.port();
});

IPAddress host2;
if (IPAddress::tryParse(addr2, host2))
{
ignore([&]
{
SocketAddress a(host, port);
SocketAddress b(host2, port);
[[maybe_unused]] bool lt = a < b;
[[maybe_unused]] bool eq = a == b;
});
}
}


void fuzzCookies(const std::string& value)
{
// Feed the value string as every interesting cookie attribute at once;
// HTTPCookie's parser decides which apply.
NameValueCollection nvc;
nvc.add("Version", value);
nvc.add("Comment", value);
nvc.add("Domain", value);
nvc.add("Path", value);
nvc.add("MaxAge", value);
nvc.add("Expires", value);
nvc.add("Secure", "secure");
nvc.add("HttpOnly", "HttpOnly");
nvc.add("SameSite", value);
nvc.add("Priority", value);

ignore([&]
{
HTTPCookie cookie(nvc);
{
[[maybe_unused]] int v = cookie.getVersion();
}
{
[[maybe_unused]] int v = cookie.getMaxAge();
}
{
[[maybe_unused]] bool v = cookie.getSecure();
}
{
[[maybe_unused]] bool v = cookie.getHttpOnly();
}
{
[[maybe_unused]] HTTPCookie::SameSite v = cookie.getSameSite();
}
{
[[maybe_unused]] const std::string& v = cookie.getDomain();
}
{
[[maybe_unused]] const std::string& v = cookie.getPath();
}
{
[[maybe_unused]] const std::string& v = cookie.getComment();
}
{
[[maybe_unused]] const std::string& v = cookie.getPriority();
}
{
[[maybe_unused]] std::string s = cookie.toString();
}
});
}


void fuzzMediaType(const std::string& mt)
{
ignore([&]
{
MediaType mediaType(mt);
{
[[maybe_unused]] std::string s = mediaType.toString();
}
{
[[maybe_unused]] const std::string& t = mediaType.getType();
}
{
[[maybe_unused]] const std::string& st = mediaType.getSubType();
}
{
[[maybe_unused]] const std::string& v = mediaType.getParameter("charset");
}
{
[[maybe_unused]] bool v = mediaType.hasParameter("boundary");
}
const NameValueCollection& params = mediaType.parameters();
for (const auto& param : params)
{
[[maybe_unused]] const std::string& v = param.second;
}
});
}


} // namespace


extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
// Input layout: [op:1][port:2][str1]NUL[str2] - str1 is the address-ish
// string, str2 the cookie/media-type payload. Both are raw attacker bytes.
if (size < 4) return 0;

const std::uint8_t op = data[0];
const std::uint16_t port = static_cast<std::uint16_t>((data[1] << 8) | data[2]);

const uint8_t* sep = static_cast<const uint8_t*>(memchr(data + 3, '\0', size - 3));
std::string str1(reinterpret_cast<const char*>(data) + 3, sep ? sep - (data + 3) : size - 3);
std::string str2;
if (sep && sep + 1 < data + size)
str2.assign(reinterpret_cast<const char*>(sep + 1), size - static_cast<std::size_t>(sep + 1 - data));

const bool runAll = op == 0;

if (runAll || op & 0x01)
fuzzIPAddress(str1);

if (runAll || op & 0x02)
fuzzSocketAddress(str1, str2, port);

if (runAll || op & 0x04)
fuzzCookies(str2);

if (runAll || op & 0x08)
{
fuzzMediaType(str2);
fuzzMediaType(str1);
}

if (runAll || op & 0x10)
{
// Set-Cookie header parsing through HTTPResponse.
ignore([&]
{
HTTPResponse response;
response.add("Set-Cookie", str2);
std::vector<HTTPCookie> cookies;
response.getCookies(cookies);
for (const auto& cookie : cookies)
{
[[maybe_unused]] std::string s = cookie.toString();
}
});
}

return 0;
}
