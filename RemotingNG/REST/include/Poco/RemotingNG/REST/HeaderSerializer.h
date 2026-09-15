//
// HeaderSerializer.h
//
// Library: RemotingNG/REST
// Package: REST
// Module:  HeaderSerializer
//
// Definition of the HeaderSerializer class.
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_REST_HeaderSerializer_INCLUDED
#define RemotingNG_REST_HeaderSerializer_INCLUDED


#include "Poco/RemotingNG/REST/REST.h"
#include "Poco/RemotingNG/REST/ScalarSerializer.h"
#include "Poco/Net/HTTPMessage.h"


namespace Poco {
namespace RemotingNG {
namespace REST {


class RemotingNGREST_API HeaderSerializer: public ScalarSerializer
	/// The Header Serializer implementation for the REST
	/// Transport.
{
public:
	HeaderSerializer(Poco::Net::HTTPMessage& message);
		/// Creates the HeaderSerializer. 

	~HeaderSerializer();
		/// Destroys the HeaderSerializer.
	
protected:
	// ScalarSerializer
	void serializeData(const std::string& name, const std::string& val);

private:
	Poco::Net::HTTPMessage* _pMessage;
};


} } } // namespace Poco::RemotingNG::REST


#endif // RemotingNG_REST_HeaderSerializer_INCLUDED
