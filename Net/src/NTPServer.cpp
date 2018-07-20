#include "Poco/Net/NTPServer.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Timespan.h"
#include "Poco/ByteOrder.h"
#include "Poco/Net/NTPPacket.h"

using Poco::Net::Socket;
using Poco::Net::DatagramSocket;
using Poco::Net::SocketAddress;
using Poco::Net::IPAddress;
using Poco::Net::NTPPacket;

namespace Poco {
namespace Net {

static int ntp_reply(DatagramSocket& socket, SocketAddress& sender, NTPPacket& recv_pack, Poco::UInt64 iRecvTime)
{
	//RFC 5905 - IETF	

	if ((recv_pack.mode() != 3) || (recv_pack.version() > 4) || (recv_pack.version() < 1) || (recv_pack.stratum() >= 16)) 
		return 1;

	/*
	LI   = 0
	VN   = Version Nummer same as Client
	Mode = 4
	*/
	NTPPacket send_pack;
	send_pack.setLeapIndicator(0);
	send_pack.setVersion(recv_pack.version());
	send_pack.setMode(4);

	/* Stratum = 1 (primary reference)
	Reference ID = 'LOCL"
	*/
	/* Stratum */
	send_pack.setStratum(0x01);

	/* Reference ID = "LOCL" */
	send_pack.setReferenceId(0x4C4F434C);	

	/* Copy Poll */
	send_pack.setPool(recv_pack.pool());

	/* Precision in Microsecond */	
	send_pack.setPrecision((signed char)(-6)); /* 2^(-6) sec */
	
	/* Root Delay = 0, Root Dispersion = 0 */
	send_pack.setRootDelay(0);
	send_pack.setRootDispersion(0);

	/* 1 minute earlier to prevent client verification */
	send_pack.setReferenceTimestamp(NTPPacket::getNTPTime(-60));

	/* Originate Time = Transmit Time @ Client */
	send_pack.setOriginateTimestamp(recv_pack.transmitTimestamp());

	/* Receive Time @ Server */
	send_pack.setReceiveTimestamp(iRecvTime);

	/* Transmit Time*/
	send_pack.setTransmitTimestamp(NTPPacket::getNTPTime());

	unsigned char send_buf[48] = { 0 };
	send_pack.packet(send_buf);

	if (socket.sendTo(send_buf, sizeof(send_buf), sender) < sizeof(send_buf))
		return 1;
	return 0;
}

NTPServer::NTPServer():
	_thread("NTPServer"),
	_stop(false)
{
	_socket.bind(SocketAddress("0.0.0.0", 123), false);
	_thread.start(*this);
}


NTPServer::NTPServer(const SocketAddress& sa):
	_thread("NTPServer"),
	_stop(false)
{
	_socket.bind(sa, false);
	_thread.start(*this);
}


NTPServer::~NTPServer()
{
	stop();
}

void NTPServer::stop() 
{
	if (_stop)
		return;
	_stop = true;
	try 
	{
		_socket.close();
	}
	catch (...) 
	{
	}
	try 
	{
		_thread.join();
	}
	catch (...) 
	{
	}
}

void NTPServer::run()
{
	Poco::Timespan span(250000);

	unsigned char buffer[48 + 1] = { 0 };
	while (!_stop)
	{
		if (!_socket.poll(span, Socket::SELECT_READ))
			continue;
		try
		{
			SocketAddress sender;
			int n = _socket.receiveFrom(buffer, 48, sender);
			if (n == 0)
				break;
			if (n < 48)
				continue;

			Poco::UInt64 uiReceiveTime = NTPPacket::getNTPTime();
			NTPPacket recv_pack(buffer);
			ntp_reply(_socket, sender, recv_pack, uiReceiveTime);
		}
		catch (Poco::Exception& exc)
		{			
		}
	}
}

} } // namespace Poco::Net
