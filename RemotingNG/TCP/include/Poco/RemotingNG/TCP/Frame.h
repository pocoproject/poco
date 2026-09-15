//
// Frame.h
//
// Library: RemotingNG/TCP
// Package: TCP
// Module:  Frame
//
// Definition of the Frame class.
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#ifndef RemotingNG_TCP_Frame_INCLUDED
#define RemotingNG_TCP_Frame_INCLUDED


#include "Poco/RemotingNG/TCP/TCP.h"
#include "Poco/RefCountedObject.h"
#include "Poco/AutoPtr.h"
#include "Poco/Buffer.h"
#include "Poco/ByteOrder.h"


namespace Poco {
namespace RemotingNG {
namespace TCP {


class RemotingNGTCP_API Frame: public Poco::RefCountedObject
	/// This class represents a network frame used by the
	/// TCP transport connection.
	///
	/// A frame consists of a header, followed by payload.
	/// Header format is as follows:
	///   - Frame Type:   32 bit unsigned integer.
	///   - Channel:      32 bit unsigned integer.
	///   - Flags:        16 bit unsigned integer.
	///   - Payload Size: 16 bit unsigned integer.
	///
	/// All integers are in network byte order.
	/// Payload sizes are 16 bit, although practically
	/// only payloads up to a size of 1012 bytes are used.
{
public:
	using Ptr = Poco::AutoPtr<Frame>;

	enum Types
	{
		FRAME_TYPE_HELO = 0x48454C4F,
			/// "HELO" - first frame to be sent over channel 0 after connection
			/// has been established. Sent by both parties. Payload is
			/// protocol major version (8 bit unsigned), followed by
			/// protocol minor version (8 bit unsigned), followed by
			/// flags (8 bit unsigned, currently unused), followed by
			/// number of capabilities (8 bit unsigned), followed by
			/// list of capabilities as 32 bit integers.

		FRAME_TYPE_BYE  = 0x42594521,
			/// "BYE!" - last frame to be sent over channel.
			/// The endpoint wishing to close the connection
			/// sends a BYE! frame, and the other side acknowledges
			/// by responding also with a BYE! frame. After acknowledgement,
			/// the underlying TCP connection is closed.
			///
			/// The frame has no payload.

		FRAME_TYPE_AUTH = 0x41555448,
			/// "AUTH" - A Remoting NG authentication request message.
			///
			/// Payload:
			///   - mechanism: string
			///   - payload: serialized credentials (key-value pairs)

		FRAME_TYPE_AUTC = 0x41555443,
			/// "AUTC" - A Remoting NG authentication continuation message.
			///
			/// Payload:
			///   - conversation ID: UInt32
			///   - payload: serialized credentials (key-value pairs)

		FRAME_TYPE_AUTR = 0x41555452,
			/// "AUTR" - A Remoting NG authentication response message.
			///
			/// Payload is:
			///   - state: UInt8 (0x01 = Done, 0x02 = Continue, 0x80 = Failed)
			///   - conversation ID: UInt32
			///   - payload: serialized credentials (key-value pairs)
			///   - authToken (only if state = Done)

		FRAME_TYPE_REQU = 0x52455155,
			/// "REQU" - A Remoting NG request message.
			///
			/// Payload is serialized request (see Poco::RemotingNG::BinarySerializer).

		FRAME_TYPE_REPL = 0x5245504C,
			/// "RESP" - A Remoting NG reply message.
			///
			/// Payload is serialized reply (see Poco::RemotingNG::BinarySerializer).

		FRAME_TYPE_EVNT = 0x45564E54,
			/// "EVNT" - A Remoting NG event message.
			///
			/// Payload is serialized event (see Poco::RemotingNG::BinarySerializer).

		FRAME_TYPE_EVNR = 0x45564E52,
			/// "EVNR" - A Remoting NG event reply message.
			///
			/// Payload is serialized event reply (see Poco::RemotingNG::BinarySerializer).

		FRAME_TYPE_EVSU = 0x45565355,
			/// "EVSU" - A Remoting NG event subscribe/renewal message.

		FRAME_TYPE_EVUN = 0x4556554E,
			/// "EVUN" - A Remoting NG event unsubscribe message.
	};

	enum Flags
	{
		FRAME_FLAG_EOM     = 0x0001,
			/// End Of Message - the last frame of a message.

		FRAME_FLAG_CONT    = 0x0002,
			/// Continuation frame - set on all frames except the
			/// first one in a multi-frame message.

		FRAME_FLAG_ONEWAY  = 0x0004,
			/// One Way Message - no reply expected.

		FRAME_FLAG_DEFLATE = 0x0008,
			/// Frame/message payload is compressed using zlib deflate
			/// algorithm.

		FRAME_FLAG_AUTH    = 0x0010,
			/// Frame contains authentication token.

		FRAME_FLAG_EXTHDR  = 0x8000
			/// Extended header - reserved for future use.
	};

	enum Sizes
	{
		FRAME_HEADER_SIZE = 12,
			/// Size of frame header.

		FRAME_MAX_SIZE = 1024,
			/// Maximum frame size (header + payload) currently used by protocol.

		FRAME_MAX_PAYLOAD_SIZE = FRAME_MAX_SIZE - FRAME_HEADER_SIZE
			/// Maximum frame payload size currently used by protocol.
	};

	enum Version
	{
		PROTO_MAJOR_VERSION = 1,
		PROTO_MINOR_VERSION = 1
	};

	enum Capabilities
	{
		CAPA_REMOTING_PROTOCOL_1_0 = 0x524D0100,
			/// The endpoint understands the Remoting NG binary protocol, version 1.0

		CAPA_REMOTING_PROTOCOL_1_1 = 0x524D0101,
			/// The endpoint understands the Remoting NG binary protocol, version 1.1
			/// (including authentication)
	};

	Frame(Poco::UInt32 type, Poco::UInt32 channel, Poco::UInt16 flags, Poco::UInt16 bufferSize);
		/// Creates a frame with the given type, flags and buffer size.
		///
		/// Buffer size must be sufficient for both header and payload.
		/// Minimum buffer size is the size of the header, which is 12 bytes.
		///
		/// The initial payload size is 0.

	~Frame();
		/// Destroys the frame.

	Poco::UInt32 type() const;
		/// Returns the frame type.

	Poco::UInt32 channel() const;
		/// Returns the channel.

	Poco::UInt16 flags() const;
		/// Returns the flags.

	void updateFlags(Poco::UInt16 flags);
		/// Updates the flags.

	Poco::UInt16 getPayloadSize() const;
		/// Returns the payload size.

	void setPayloadSize(Poco::UInt16 size);
		/// Sets the payload size, which must not exceed the
		/// buffer size minus header size.

	Poco::UInt16 bufferSize() const;
		/// Returns the buffer size.

	Poco::UInt16 maxPayloadSize() const;
		/// Returns the maximum payload size,
		/// which is buffer size minus header size.

	Poco::UInt16 frameSize() const;
		/// Returns the frame size, which is header size plus payload size.

	char* payloadBegin();
		/// Returns a pointer to the begin of the payload in the buffer.

	const char* payloadBegin() const;
		/// Returns a pointer to the begin of the payload in the buffer.

	char* payloadEnd();
		/// Returns a pointer to the end (one past last byte) of the payload in the buffer.

	const char* payloadEnd() const;
		/// Returns a pointer to the end (one past last byte) of the payload in the buffer.

	char* bufferBegin();
		/// Returns a pointer to the begin of the buffer.

	const char* bufferBegin() const;
		/// Returns a pointer to the begin of the buffer.

	char* bufferEnd();
		/// Returns a pointer to the end (one past last byte) of the buffer.

	const char* bufferEnd() const;
		/// Returns a pointer to the end (one past last byte) of the buffer.

private:
	enum
	{
		FRAME_OFFSET_TYPE    = 0,
		FRAME_OFFSET_CHANNEL = 4,
		FRAME_OFFSET_FLAGS   = 8,
		FRAME_OFFSET_SIZE    = 10,
		FRAME_OFFSET_PAYLOAD = 12
	};

	Poco::Buffer<char> _buffer;
};


//
// inlines
//
inline Poco::UInt32 Frame::type() const
{
	return Poco::ByteOrder::fromNetwork(*reinterpret_cast<const Poco::UInt32*>(_buffer.begin() + FRAME_OFFSET_TYPE));
}


inline Poco::UInt32 Frame::channel() const
{
	return Poco::ByteOrder::fromNetwork(*reinterpret_cast<const Poco::UInt32*>(_buffer.begin() + FRAME_OFFSET_CHANNEL));
}


inline Poco::UInt16 Frame::flags() const
{
	return Poco::ByteOrder::fromNetwork(*reinterpret_cast<const Poco::UInt16*>(_buffer.begin() + FRAME_OFFSET_FLAGS));
}


inline void Frame::updateFlags(Poco::UInt16 flags)
{
	*reinterpret_cast<Poco::UInt16*>(_buffer.begin() + FRAME_OFFSET_FLAGS) = Poco::ByteOrder::toNetwork(flags);
}


inline Poco::UInt16 Frame::getPayloadSize() const
{
	return Poco::ByteOrder::fromNetwork(*reinterpret_cast<const Poco::UInt16*>(_buffer.begin() + FRAME_OFFSET_SIZE));
}


inline void Frame::setPayloadSize(Poco::UInt16 size)
{
	poco_assert (size <= maxPayloadSize());

	*reinterpret_cast<Poco::UInt16*>(_buffer.begin() + FRAME_OFFSET_SIZE) = Poco::ByteOrder::toNetwork(size);
}


inline Poco::UInt16 Frame::bufferSize() const
{
	return static_cast<Poco::UInt16>(_buffer.size());
}


inline Poco::UInt16 Frame::maxPayloadSize() const
{
	return static_cast<Poco::UInt16>(_buffer.size() - FRAME_OFFSET_PAYLOAD);
}


inline Poco::UInt16 Frame::frameSize() const
{
	return static_cast<Poco::UInt16>(getPayloadSize() + FRAME_OFFSET_PAYLOAD);
}


inline char* Frame::payloadBegin()
{
	return _buffer.begin() + FRAME_OFFSET_PAYLOAD;
}


inline const char* Frame::payloadBegin() const
{
	return _buffer.begin() + FRAME_OFFSET_PAYLOAD;
}


inline char* Frame::payloadEnd()
{
	return _buffer.begin() + FRAME_OFFSET_PAYLOAD + getPayloadSize();
}


inline const char* Frame::payloadEnd() const
{
	return _buffer.begin() + FRAME_OFFSET_PAYLOAD + getPayloadSize();
}


inline char* Frame::bufferBegin()
{
	return _buffer.begin();
}


inline const char* Frame::bufferBegin() const
{
	return _buffer.begin();
}


inline char* Frame::bufferEnd()
{
	return _buffer.end();
}


inline const char* Frame::bufferEnd() const
{
	return _buffer.end();
}


} } } // namespace Poco::RemotingNG::TCP


#endif // RemotingNG_TCP_Frame_INCLUDED
