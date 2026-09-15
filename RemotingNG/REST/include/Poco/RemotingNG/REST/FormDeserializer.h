//
// FormDeserializer.h
//
// Library: RemotingNG/REST
// Package: REST
// Module:  FormDeserializer
//
// Definition of the FormDeserializer class.
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_REST_FormDeserializer_INCLUDED
#define RemotingNG_REST_FormDeserializer_INCLUDED


#include "Poco/RemotingNG/REST/REST.h"
#include "Poco/RemotingNG/REST/ScalarDeserializer.h"
#include "Poco/RemotingNG/Deserializer.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTMLForm.h"
#include <istream>


namespace Poco {
namespace RemotingNG {
namespace REST {


class RemotingNGREST_API FormDeserializer: public ScalarDeserializer
	/// The Form Deserializer implementation for the REST Transport.
	///
	/// For more information, please see the Deserializer documentation.
{
public:
	FormDeserializer(Poco::Net::HTTPRequest& request);
		/// Creates the FormDeserializer.

	FormDeserializer(Poco::Net::HTTPRequest& request, std::istream& stream);
		/// Creates the FormDeserializer.

	FormDeserializer(Poco::Net::HTTPResponse& response, std::istream& stream);
		/// Creates the FormDeserializer.

	~FormDeserializer();
		/// Destroys the FormDeserializer.

	// Poco::RemotingNG::Deserializer
	bool deserializeNullableBegin(const std::string& name, bool isMandatory, bool& isNull);

protected:
	void handlePart(const std::string& name, std::istream& stream);

	// ScalarDeserializer
	const std::string& deserializeData(const std::string& name, bool isMandatory, bool& found);
	bool deserializeBuffer(const std::string& name, bool isMandatory, std::vector<char>& val);

private:
	using PartMap = std::map<std::string, std::vector<char>>;

	Poco::Net::HTMLForm _form;
	PartMap _parts;

	static const std::string EMPTY;

	friend class PartHandler;
};


} } } // namespace Poco::RemotingNG::REST


#endif // RemotingNG_REST_FormDeserializer_INCLUDED
