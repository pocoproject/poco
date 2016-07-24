#ifndef TimeHandler_INCLUDED
#define TimeHandler_INCLUDED


#include "Poco/Net/HTTPRequestHandler.h"


class TimeHandler: public Poco::Net::HTTPRequestHandler
{
public:
	void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
};


#endif // TimeHandler_INCLUDED
