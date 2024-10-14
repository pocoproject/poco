#include "Poco/MemoryStream.h"
#include "Poco/Net/EscapeHTMLStream.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/Net/HTTPCredentials.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/OAuth10Credentials.h"
#include "Poco/Net/OAuth20Credentials.h"
#include "Poco/Net/DNS.h"
#include "Poco/NullStream.h"

using namespace Poco;
using namespace Poco::Net;

template <class F>
void catchExceptions(const F& func)
{
	try
	{
		func();
	}
	catch (const std::exception&)
	{
	}
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
	NullOutputStream null;

	// HTTPRequest parsing
	catchExceptions(
		[&]
		{
			MemoryInputStream stream(reinterpret_cast<const char*>(data), size);
			HTTPRequest request;
			request.read(stream);
			request.write(null);

			HTTPCredentials creds;
			creds.fromURI(URI(request.getURI()));
			creds.updateAuthInfo(request);
			creds.updateProxyAuthInfo(request);
		});

	// HTTPResponse parsing
	catchExceptions(
		[&]
		{
			MemoryInputStream stream(reinterpret_cast<const char*>(data), size);
			HTTPResponse response;
			response.read(stream);
			response.write(null);

			HTTPRequest request(HTTPRequest::HTTP_GET, "/");
			request.setHost(DNS::encodeIDN(DNS::decodeIDN(response.get(HTTPRequest::HOST))));

			HTTPCredentials creds;
			creds.authenticate(request, response);
			creds.proxyAuthenticate(request, response);
		});

	// OAuth10Credentials
	catchExceptions(
		[&]
		{
			MemoryInputStream stream(reinterpret_cast<const char*>(data), size);
			HTTPRequest request;
			request.read(stream);

			EscapeHTMLOutputStream htmlStream(null);
			HTMLForm form(request, stream);
			form.prepareSubmit(request);
			form.write(htmlStream);

			OAuth10Credentials oauth10(request);
			oauth10.verify(request, URI(request.getURI()), form);
			oauth10.authenticate(request, URI(request.getURI()), form,
								 request.hasToken("X-Method", "Plain") ? OAuth10Credentials::SIGN_PLAINTEXT
																	   : OAuth10Credentials::SIGN_HMAC_SHA1);
		});

	// OAuth20Credentials
	catchExceptions(
		[&]
		{
			MemoryInputStream stream(reinterpret_cast<const char*>(data), size);
			HTTPRequest request;
			request.read(stream);

			OAuth20Credentials oauth20(request);
			oauth20.authenticate(request);
		});

	return 0;
}
