//
// HeaderDeserializer.h
//
// Library: RemotingNG/REST
// Package: REST
// Module:  HeaderDeserializer
//
// Definition of the HeaderDeserializer class.
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef RemotingNG_REST_HeaderDeserializer_INCLUDED
#define RemotingNG_REST_HeaderDeserializer_INCLUDED


#include "Poco/RemotingNG/REST/REST.h"
#include "Poco/RemotingNG/REST/ScalarDeserializer.h"
#include "Poco/Net/HTTPMessage.h"


namespace Poco {
namespace RemotingNG {
namespace REST {


class RemotingNGREST_API HeaderDeserializer: public ScalarDeserializer
	/// The Header Deserializer implementation for the REST Transport.
	///
	/// For more information, please see the Deserializer documentation.
{
public:
	HeaderDeserializer(const Poco::Net::HTTPMessage& message);
		/// Creates the HeaderDeserializer.

	~HeaderDeserializer();
		/// Destroys the HeaderDeserializer.

	// Poco::RemotingNG::Deserializer
	bool deserializeNullableBegin(const std::string& name, bool isMandatory, bool& isNull);

protected:
	// ScalarDeserializer
	const std::string& deserializeData(const std::string& name, bool isMandatory, bool& found);

private:
	const Poco::Net::HTTPMessage& _message;

	static const std::string EMPTY;
};


} } } // namespace Poco::RemotingNG::REST


#endif // RemotingNG_REST_HeaderDeserializer_INCLUDED
