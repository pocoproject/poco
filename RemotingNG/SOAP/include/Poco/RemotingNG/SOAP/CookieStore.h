//
// CookieStore.h
//
// Library: RemotingNG/SOAP
// Package: SOAP
// Module:  CookieStore
//
// Definition of the CookieStore class.
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties,
// copied or duplicated in any form, in whole or in part.
//


#ifndef RemotingNG_SOAP_CookieStore_INCLUDED
#define RemotingNG_SOAP_CookieStore_INCLUDED


#include "Poco/RemotingNG/SOAP/SOAP.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPCookie.h"
#include "Poco/RefCountedObject.h"
#include "Poco/Timestamp.h"
#include "Poco/AutoPtr.h"
#include "Poco/Mutex.h"
#include <map>


namespace Poco {
namespace RemotingNG {
namespace SOAP {


class RemotingNGSOAP_API CookieStore: public Poco::RefCountedObject
	/// A CookieStore object stores cookies set by a SOAP server.
	///
	/// Normally, each Transport (and thus Proxy) instance has its own cookie
	/// store instance. However, CookieStore objects can also be shared
	/// among multiple Transport instances connected to the same server.
	///
	/// A typical use case for this is sharing session cookies among all
	/// Transport/Proxy instances connected to the same server.
	///
	/// Note: The application must take care that CookieStore objects are
	/// only shared among Transport instances connected to the same server.
{
public:
	using Ptr = Poco::AutoPtr<CookieStore>;

	CookieStore();
		/// Creates an empty CookieStore.

	~CookieStore();
		/// Destroys the CookieStore.

	void addCookies(Poco::Net::HTTPRequest& request);
		/// Adds the stored cookies to the given request object.

	void updateCookies(const Poco::Net::HTTPResponse& response);
		/// Updates the stored cookies with the cookies from
		/// the given response object.

private:
	struct CookieInfo
	{
		Poco::Net::HTTPCookie cookie;
		Poco::Timestamp expireTime;
	};
	using CookieMap = std::map<std::string, CookieInfo>;

	CookieMap _cookies;
	Poco::FastMutex _mutex;
};


} } } // namespace Poco::RemotingNG::SOAP


#endif // RemotingNG_SOAP_CookieStore_INCLUDED
