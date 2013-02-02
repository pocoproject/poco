#include <stdio.h>
#include <stdlib.h>

#include <iostream>

#include "Poco/MongoDB/Document.h"
#include "Poco/MongoDB/QueryRequest.h"
#include "Poco/MongoDB/Connection.h"

int main(void)
{
	Poco::MongoDB::Document doc;
	doc.add("test", 10);

	Poco::MongoDB::Connection mongo("localhost", 27017);

	Poco::MongoDB::ResponseMessage response;

	// Count

	Poco::MongoDB::QueryRequest request("test.$cmd");
	request.numberToReturn(1);
	request.query().add("count", std::string("players"));

	mongo.sendRequest(request, response);

	if ( response.documents().size() > 0 )
	{
		Poco::MongoDB::Document::Ptr doc = response.documents()[0];
		std::cout << "n= " << doc->get<double>("n") << std::endl;
	}

	std::cout << "Number of documents returned: " << response.documents().size() << std::endl;

	return 0;
}
