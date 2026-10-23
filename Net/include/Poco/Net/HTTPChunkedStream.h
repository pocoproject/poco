//
// HTTPChunkedStream.h
//
// Library: Net
// Package: HTTP
// Module:  HTTPChunkedStream
//
// Definition of the HTTPChunkedStream class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_HTTPChunkedStream_INCLUDED
#define Net_HTTPChunkedStream_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Net/HTTPBasicStreamBuf.h"
#include <istream>
#include <ostream>


namespace Poco::Net {


class HTTPSession;
class MessageHeader;


class Net_API HTTPChunkedStreamBuf: public HTTPBasicStreamBuf
	/// This is the streambuf class used for reading and writing
	/// HTTP message bodies in chunked transfer coding.
	///
	/// Malformed chunked framing makes readFromDevice() throw a
	/// MessageException, so the input stream sets badbit. A body cut
	/// short by the end of the stream ends the input stream after the
	/// data received. In both cases, the cause is stored in the session
	/// (see HTTPSession::networkException()).
	///
	/// A trailer section longer than 65536 bytes is rejected.
	///
	/// After a failed or short write, the output stream writes nothing
	/// more, not even the last chunk. A short write stores a NetException
	/// in the session.
{
public:
	using openmode = HTTPBasicStreamBuf::openmode;

	HTTPChunkedStreamBuf(HTTPSession& session, openmode mode, MessageHeader* pTrailer = nullptr);
	~HTTPChunkedStreamBuf();
	void close();
		/// Writes the last chunk and the trailer once. Throws a MessageException
		/// if the body or the last chunk could not be written or the trailer
		/// is invalid, and stores it in the session unless the session
		/// already holds an exception.

	[[nodiscard]] bool isComplete() const noexcept;
		/// Returns true if the body has been read up to and including
		/// the last chunk and the trailer without an error.

	[[nodiscard]] bool consumeTerminator();
		/// Reads the last chunk and the trailer of an input stream if no
		/// chunk data is left unread. Returns at once if the peer has sent
		/// nothing more. Waits up to the receive timeout for the rest of a
		/// line the peer has begun, for the trailer after the last chunk,
		/// and for data after a TLS record that carries none.
		///
		/// Returns isComplete().

protected:
	std::streamsize readFromDevice(char* buffer, std::streamsize length);
	std::streamsize writeToDevice(const char* buffer, std::streamsize length);

private:
	enum class ReadState
	{
		SIZE,
		DATA,
		DATA_CRLF,
		DONE,
		FAILED
	};

	HTTPSession&    _session;
	openmode        _mode;
	std::streamsize _chunk;
	ReadState       _readState;
	std::string     _chunkBuffer;
	MessageHeader*  _pTrailer;
	bool            _closed;
	bool            _writeFailed;
};


class Net_API HTTPChunkedIOS: public virtual std::ios
	/// The base class for HTTPInputStream.
{
public:
	HTTPChunkedIOS(HTTPSession& session, HTTPChunkedStreamBuf::openmode mode, MessageHeader* pTrailer = nullptr);
	~HTTPChunkedIOS();
	[[nodiscard]] HTTPChunkedStreamBuf* rdbuf();

protected:
	HTTPChunkedStreamBuf _buf;
};


class Net_API HTTPChunkedInputStream: public HTTPChunkedIOS, public std::istream
	/// This class is for internal use by HTTPSession only.
{
public:
	HTTPChunkedInputStream(HTTPSession& session, MessageHeader* pTrailer = nullptr);
	~HTTPChunkedInputStream();
};


class Net_API HTTPChunkedOutputStream: public HTTPChunkedIOS, public std::ostream
	/// This class is for internal use by HTTPSession only.
{
public:
	HTTPChunkedOutputStream(HTTPSession& session, MessageHeader* pTrailer = nullptr);
	~HTTPChunkedOutputStream();
};


} // namespace Poco::Net


#endif // Net_HTTPChunkedStream_INCLUDED
