//
// PathSerializer.h
//
// Library: RemotingNG/REST
// Package: REST
// Module:  PathSerializer
//
// Definition of the PathSerializer class.
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef RemotingNG_REST_PathSerializer_INCLUDED
#define RemotingNG_REST_PathSerializer_INCLUDED


#include "Poco/RemotingNG/REST/REST.h"
#include "Poco/RemotingNG/REST/ScalarSerializer.h"
#include "Poco/Net/HTTPRequest.h"
#include <map>


namespace Poco {
namespace RemotingNG {
namespace REST {


class RemotingNGREST_API PathSerializer: public ScalarSerializer
	/// The Path Serializer implementation for the REST
	/// Transport.
{
public:
	PathSerializer(const std::string& pathTemplate, Poco::Net::HTTPRequest& request);
		/// Creates the PathSerializer. 

	~PathSerializer();
		/// Destroys the PathSerializer.
		
	void close();
		/// Writes the complete path to the request.
	
protected:
	const std::string& replaceItem(const std::string& item);
		/// Replaces the path item with a parameter if it's a placeholder.
		
	// ScalarSerializer
	void serializeData(const std::string& name, const std::string& val);

private:
	std::string _pathTemplate;
	Poco::Net::HTTPRequest& _request;
	std::map<std::string, std::string> _values;
	
	static const std::string EMPTY;
	
	friend class PathDeserializer;
};


} } } // namespace Poco::RemotingNG::REST


#endif // RemotingNG_REST_PathSerializer_INCLUDED
