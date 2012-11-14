#include "Poco/Path.h"
#include <iostream>

#include "Poco/Net/IPAddress.h"
#include "Poco/Net/Route.h"
#include "Poco/Net/NetworkInterface.h"

using Poco::Path;
using Poco::Net::IPAddress;
using Poco::Net::Route;
using Poco::Net::NetworkInterface;

void dumpRoutes(const Route::RouteList& routes)
{
	for (Route::RouteList::const_iterator it = routes.begin();
		 it != routes.end(); it++) {
		std::cout << (*it).getDest().toString() << '/' << (*it).getPrefix();
		if ((*it).getType() == Route::ROUTE_INDIRECT)
			std::cout << " via " << (*it).getNextHop().toString();
		std::cout << " dev " << (*it).getNetworkInterface().name();
		if ((*it).validProto())
			std::cout << " proto " << Route::protocolName((*it).getProto());
		if ((*it).validAge())
			std::cout << " age " << (*it).getAge();
		if ((*it).validMTU())
			std::cout << " mtu " << (*it).getMTU();
		std::cout << std::endl;
	}
}

int main(int argc, char** argv)
{
	if (argc != 1) {
		Path p(argv[0]);
		std::cerr << "usage: " << p.getBaseName() << std::endl;
		return 1;
	}

	Route::RouteList routes = Route::list(IPAddress::IPv4);

	dumpRoutes(routes);

#if defined(POCO_HAVE_IPv6)
	routes = Route::list(IPAddress::IPv6);

	dumpRoutes(routes);
#endif

	return 0;
}


// vim:ts=4
