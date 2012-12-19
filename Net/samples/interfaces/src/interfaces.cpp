//
// interfaces.cpp
//
// $Id: //poco/1.4/Net/samples/interfaces/src/interfaces.cpp#1 $
//
// This sample demonstrates the NetworkInterface class.
//


#include "Poco/Path.h"
#include "Poco/Exception.h"
#include "Poco/Net/IPAddress.h"
#include "Poco/Net/NetworkInterface.h"
#include <memory>
#include <iostream>

using Poco::Path;
using Poco::Exception;
using Poco::Net::IPAddress;
using Poco::Net::NetworkInterface;


int main(int argc, char** argv)
{
	if (argc != 1)
	{
		Path p(argv[0]);
		std::cerr << "usage: " << p.getBaseName() << std::endl;
		return 1;
	}

	try
	{
		const NetworkInterface::Map map = NetworkInterface::map();
		for ( NetworkInterface::Map::const_iterator it = map.begin();
			it != map.end(); ++it) {
			const NetworkInterface& intf = it->second;
			std::string sep("");

			std::cout << intf.name() << " [" << intf.index() << "]: ";

			std::cout << "<";
			if (intf.isUp()) {
				std::cout << sep << "UP"; sep = ",";
			}
			if (intf.isRunning()) {
				std::cout << sep << "RUNNING"; sep = ",";
			}
			if (intf.isLoopback()) {
				std::cout << sep << "LOOPBACK"; sep = ",";
			}
			if (intf.isPointToPoint()) {
				std::cout << sep << "P2P"; sep = ",";
			}
			if (intf.supportsIPv4()) {
				std::cout << sep << "IPv4"; sep = ",";
			}
			if (intf.supportsIPv6()) {
				std::cout << sep << "IPv6"; sep = ",";
			}
			if (intf.supportsBroadcast()) {
				std::cout << sep << "BCAST"; sep = ",";
			}
			if (intf.supportsMulticast()) {
				std::cout << sep << "MCAST"; sep = ",";
			}

			std::cout << sep << std::dec << intf.mtu(); sep = ",";

			std::cout << ">" << std::endl;

			const NetworkInterface::AddressList& ipList = intf.addressList();

			NetworkInterface::AddressList::const_iterator ipIt = ipList.begin();
			NetworkInterface::AddressList::const_iterator ipEnd = ipList.end();
			for (; ipIt != ipEnd; ++ipIt) {
				std::cout << "  " << ipIt->get<NetworkInterface::IP_ADDRESS>().toString();
				IPAddress addr;
				addr = ipIt->get<NetworkInterface::SUBNET_MASK>();
				if (!addr.isWildcard()) std::cout << '/' << addr.toString() << " (" << addr.prefixLength() << ')';
				addr = ipIt->get<NetworkInterface::BROADCAST_ADDRESS>();
				if (!addr.isWildcard()) std::cout << (intf.isPointToPoint() ? "  dest  " : "  bcast  ") << addr.toString();
				std::cout << std::endl;
			}

			std::cout << std::endl;
		}
	}
	catch (Exception& exc)
	{
		std::cerr << exc.displayText() << std::endl;
		return 1;
	}
		
	return 0;
}
