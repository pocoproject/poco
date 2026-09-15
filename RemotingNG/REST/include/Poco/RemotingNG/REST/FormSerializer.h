//
// FormSerializer.h
//
// Library: RemotingNG/REST
// Package: REST
// Module:  FormSerializer
//
// Definition of the FormSerializer class.
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_REST_FormSerializer_INCLUDED
#define RemotingNG_REST_FormSerializer_INCLUDED


#include "Poco/RemotingNG/REST/REST.h"
#include "Poco/RemotingNG/REST/ScalarSerializer.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTMLForm.h"
#include <ostream>


namespace Poco {
namespace RemotingNG {
namespace REST {


class RemotingNGREST_API FormSerializer: public ScalarSerializer
	/// The Form Serializer implementation for the REST
	/// Transport.
{
public:
	FormSerializer(Poco::Net::HTTPRequest& request);
		/// Creates the FormSerializer.

	FormSerializer(Poco::Net::HTTPRequest& request, std::ostream& stream);
		/// Creates the FormSerializer.

	FormSerializer(Poco::Net::HTTPResponse& response, std::ostream& stream);
		/// Creates the FormSerializer.

	~FormSerializer();
		/// Destroys the FormSerializer.

	void close();
		/// Finalizes serialization.

protected:
	// ScalarSerializer
	void serializeData(const std::string& name, const std::string& val);
	void serializeBuffer(const std::string& name, const std::vector<char>& val);

	static const std::string APPLICATION_OCTET_STREAM;

private:
	Poco::Net::HTTPRequest* _pRequest;
	Poco::Net::HTTPResponse* _pResponse;
	std::ostream* _pStream;
	Poco::Net::HTMLForm _form;

	friend class FormDeserializer;
};


} } } // namespace Poco::RemotingNG::REST


#endif // RemotingNG_REST_FormSerializer_INCLUDED
