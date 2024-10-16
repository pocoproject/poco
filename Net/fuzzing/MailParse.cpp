#include "Poco/MemoryStream.h"
#include "Poco/Net/MailMessage.h"
#include "Poco/Net/MailStream.h"
#include "Poco/NullStream.h"

using namespace Poco;
using namespace Poco::Net;

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
	try
	{
		MemoryInputStream stream(reinterpret_cast<const char*>(data), size);
		MailInputStream mis(stream);
		MailMessage mail;
		mail.read(mis);

		MailRecipient recipient(MailRecipient::CC_RECIPIENT, MailMessage::encodeWord(mail.getSender()));
		mail.addRecipient(recipient);

		NullOutputStream null;
		MailOutputStream mos(null);
		mail.write(mos);
	}
	catch (const std::exception&)
	{
	}

	return 0;
}
