//
// Twitter.h
//
// $Id: //poco/1.4/Net/samples/TwitterClient/src/Twitter.h#2 $
//
// A C++ implementation of a Twitter client based on the POCO Net library.
//
// Copyright (c) 2009-2013, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Twitter_INCLUDED
#define Twitter_INCLUDED


#include "Poco/Poco.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/Util/AbstractConfiguration.h"
#include "Poco/AutoPtr.h"


class Twitter
	/// A simple implementation of a Twitter API client
	/// (see <http://dev.twitter.com> for more information).
	/// 
	/// Currently, only the update message is supported.
{
public:
	Twitter();
		/// Creates the Twitter object, using
		/// the default Twitter API URI (<http://api.twitter.com/1.1/statuses/>).
		
	Twitter(const std::string& twitterURI);
		/// Creates the Twitter object using the given Twitter API URI.
		
	~Twitter();
		/// Destroys the Twitter object.
		
	void login(const std::string& consumerKey, const std::string& consumerSecret, const std::string& token, const std::string& tokenSecret);
		/// Specifies the OAuth authentication information used in all API calls.
		
	Poco::Int64 update(const std::string& status);
		/// Updates the user's status.
		///
		/// Returns the ID of the newly created status.

	Poco::AutoPtr<Poco::Util::AbstractConfiguration> invoke(const std::string& httpMethod, const std::string& twitterMethod, Poco::Net::HTMLForm& params);
		/// Invokes the given method of the Twitter API, using the parameters
		/// given in the Poco::Net::HTMLForm object. httpMethod must be GET or POST,
		/// according to the Twitter API documentation.
		///
		/// Returns a Poco::Util::JSONConfiguration with the server's response if the
		/// server's HTTP response status code is 200 (OK).
		/// Otherwise, throws a Poco::ApplicationException.
	
	static const std::string TWITTER_URI;

protected:
	void sign(Poco::Net::HTTPRequest& request, const Poco::Net::HTMLForm& params, const std::string& uri) const;
		/// Signs the given HTTP request according to OAuth 1.0a as used by the Twitter API.
		///
		/// See <https://dev.twitter.com/docs/auth/authorizing-request> and
		/// <https://dev.twitter.com/docs/auth/creating-signature> for more information.
	
	std::string createNonce() const;
		/// Creates a nonce, which is basically a Base64-encoded 32 character random
		/// string, with non-alphanumeric characters removed.

	std::string createSignature(Poco::Net::HTTPRequest& request, const Poco::Net::HTMLForm& params, const std::string& uri, const std::string& nonce, const std::string& timestamp) const;
		/// Creates a OAuth signature for the given request and its parameters, according
		/// to <https://dev.twitter.com/docs/auth/creating-signature>.
		
	static std::string percentEncode(const std::string& str);
		/// Percent-encodes the given string according to Twitter API's rules,
		/// given in <https://dev.twitter.com/docs/auth/percent-encoding-parameters>.

private:
	Twitter(const Twitter&);
	Twitter& operator = (const Twitter&);
	
	std::string _uri;
	std::string _consumerKey;
	std::string _consumerSecret;
	std::string _token;
	std::string _tokenSecret;
};


#endif // Twitter_INCLUDED
