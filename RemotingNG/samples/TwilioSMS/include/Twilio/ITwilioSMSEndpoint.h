//
// ITwilioSMSEndpoint.h
//
// Package: Generated
// Module:  ITwilioSMSEndpoint
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef ITwilioSMSEndpoint_INCLUDED
#define ITwilioSMSEndpoint_INCLUDED


#include "Poco/AutoPtr.h"
#include "Poco/RefCountedObject.h"
#include "Poco/RemotingNG/Identifiable.h"
#include "Twilio/TwilioSMSEndpoint.h"


namespace Twilio {


class ITwilioSMSEndpoint: public virtual Poco::RefCountedObject
{
public:
	using Ptr = Poco::AutoPtr<ITwilioSMSEndpoint>;

	ITwilioSMSEndpoint();
		/// Creates a ITwilioSMSEndpoint.

	virtual ~ITwilioSMSEndpoint();
		/// Destroys the ITwilioSMSEndpoint.

	virtual Twilio::TwilioSMSResponse post(const std::string& accountSID, const std::string& from, const std::string& to, const std::string& body) = 0;

	static const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId();
		/// Returns the TypeId of the class.

};


} // namespace Twilio


#endif // ITwilioSMSEndpoint_INCLUDED

