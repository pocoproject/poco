#include "Poco/JWT/Token.h"
#include "Poco/JWT/Signer.h"

using namespace Poco;
using namespace Poco::JWT;

extern "C" int LLVMFuzzerInitialize(int* argc, char*** argv)
{
	Crypto::initializeCrypto();
	return 0;
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
	const std::string input(reinterpret_cast<const char*>(data), size);

	Signer signer;
	signer.addAllAlgorithms();
	signer.setHMACKey("secret");

	try
	{
		// verify untrusted input
		Token token;
		token = signer.verify(input);
	}
	catch (const Exception&)
	{
	}

	for (const auto& algorithm : signer.getAlgorithms())
	{
		try
		{
			// sign and verify again
			Token token(input);
			token.setAudience(token.getAudience());
			signer.sign(token, algorithm);
			token = signer.verify(token.toString());
		}
		catch (const Exception&)
		{
		}
	}

	return 0;
}
