//
// InflatingStream.h
//
// Library: Foundation
// Package: Streams
// Module:  ZLibStream
//
// Definition of the InflatingInputStream and InflatingOutputStream classes.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_InflatingStream_INCLUDED
#define Foundation_InflatingStream_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/BufferedStreamBuf.h"
#include <istream>
#include <ostream>

struct z_stream_s;
struct gz_header_s;


namespace Poco {


class Foundation_API InflatingStreamBuf: public BufferedStreamBuf
	/// This is the streambuf class used by InflatingInputStream and InflatingOutputStream.
	/// The actual work is delegated to zlib (see http://zlib.net).
	/// Both zlib (deflate) streams and gzip streams are supported.
	/// Output streams should always call close() to ensure
	/// proper completion of decompression.
{
public:
	enum StreamType
	{
		STREAM_ZLIB, /// Expect a zlib header, use Adler-32 checksum.
		STREAM_GZIP, /// Expect a gzip header, use CRC-32 checksum.
		STREAM_ZIP   /// STREAM_ZIP is handled as STREAM_ZLIB, except that we do not check the ADLER32 value (must be checked by caller)
	};

	InflatingStreamBuf(std::istream& istr, StreamType type);
		/// Creates an InflatingStreamBuf for expanding the compressed data read from
		/// the give input stream.

	InflatingStreamBuf(std::istream& istr, int windowBits);
		/// Creates an InflatingStreamBuf for expanding the compressed data read from
		/// the given input stream.
		///
		/// Please refer to the zlib documentation of inflateInit2() for a description
		/// of the windowBits parameter.

	InflatingStreamBuf(std::ostream& ostr, StreamType type);
		/// Creates an InflatingStreamBuf for expanding the compressed data passed through
		/// and forwarding it to the given output stream.

	InflatingStreamBuf(std::ostream& ostr, int windowBits);
		/// Creates an InflatingStreamBuf for expanding the compressed data passed through
		/// and forwarding it to the given output stream.
		///
		/// Please refer to the zlib documentation of inflateInit2() for a description
		/// of the windowBits parameter.

	~InflatingStreamBuf() override;
	/// Destroys the InflatingStreamBuf.

	int close();
		/// Finishes up the stream.
		///
		/// Must be called when inflating to an output stream.

	void reset();
		/// Resets the stream buffer so that another stream can be read
		/// from the same underlying input stream.
		///
		/// Decompressed data that has not been read yet is discarded.
		/// If the stream has been read in part, the rest of it is
		/// decompressed and discarded. If nothing of the stream has been
		/// read from the underlying input stream yet, it is not skipped.
		///
		/// If a read has reached the end of the underlying input stream
		/// before the end of the stream, nothing is skipped, and the next
		/// stream is read from the current position of the underlying
		/// input stream.
		///
		/// In all other cases without an error, the next stream starts
		/// with the data that follows the current stream. Buffered input
		/// is kept, so the underlying stream must not be repositioned.
		///
		/// After a decompression error, reads keep failing without
		/// consuming more input until reset() is called. If the error was a checksum or length mismatch in
		/// the stream trailer, the end of the failed stream is known and
		/// buffered input that follows it is kept, as after a normal end.
		/// For any other error, the end of the failed stream cannot be
		/// determined, so buffered input is discarded and the next stream
		/// is read from the current position of the underlying input
		/// stream. Only in this case and after an early end of the
		/// underlying input stream, the caller may need to position the
		/// underlying stream at the start of the next stream and call
		/// clear() on it.
		///
		/// When inflating to an output stream, written data that has not
		/// been inflated yet is inflated and written to the output stream
		/// first. If this fails, an IOException is thrown and the stream
		/// buffer is not reset. After a failed write, reset() discards
		/// the pending data.

protected:
	std::streamsize readFromDevice(char* buffer, std::streamsize length) override;
	std::streamsize writeToDevice(const char* buffer, std::streamsize length) override;
	int sync() override;

private:
	enum
	{
		STREAM_BUFFER_SIZE  = 1024,
		INFLATE_BUFFER_SIZE = 32768
	};

	std::istream*	_pIstr;
	std::ostream*	_pOstr;
	char*			_buffer;
	z_stream_s*		_pZstr;
	gz_header_s*	_pHeader;
	bool			_eof;
	bool			_check;
	bool			_failed;
	bool			_inputEnded; // the underlying input stream ended before the current stream
};


class Foundation_API InflatingIOS: public virtual std::ios
	/// The base class for InflatingOutputStream and InflatingInputStream.
	///
	/// This class is needed to ensure the correct initialization
	/// order of the stream buffer and base classes.
{
public:
	InflatingIOS(std::ostream& ostr, InflatingStreamBuf::StreamType type = InflatingStreamBuf::STREAM_ZLIB);
		/// Creates an InflatingIOS for expanding the compressed data passed through
		/// and forwarding it to the given output stream.

	InflatingIOS(std::ostream& ostr, int windowBits);
		/// Creates an InflatingIOS for expanding the compressed data passed through
		/// and forwarding it to the given output stream.
		///
		/// Please refer to the zlib documentation of inflateInit2() for a description
		/// of the windowBits parameter.

	InflatingIOS(std::istream& istr, InflatingStreamBuf::StreamType type = InflatingStreamBuf::STREAM_ZLIB);
		/// Creates an InflatingIOS for expanding the compressed data read from
		/// the given input stream.

	InflatingIOS(std::istream& istr, int windowBits);
		/// Creates an InflatingIOS for expanding the compressed data read from
		/// the given input stream.
		///
		/// Please refer to the zlib documentation of inflateInit2() for a description
		/// of the windowBits parameter.

	~InflatingIOS() override;
	/// Destroys the InflatingIOS.

	[[nodiscard]] InflatingStreamBuf* rdbuf();
		/// Returns a pointer to the underlying stream buffer.

protected:
	InflatingStreamBuf _buf;
};


class Foundation_API InflatingOutputStream: public std::ostream, public InflatingIOS
	/// This stream decompresses all data passing through it
	/// using zlib's inflate algorithm.
	///
	/// After all data has been written to the stream, close()
	/// must be called to ensure completion of decompression.
{
public:
	InflatingOutputStream(std::ostream& ostr, InflatingStreamBuf::StreamType type = InflatingStreamBuf::STREAM_ZLIB);
		/// Creates an InflatingOutputStream for expanding the compressed data passed through
		/// and forwarding it to the given output stream.

	InflatingOutputStream(std::ostream& ostr, int windowBits);
		/// Creates an InflatingOutputStream for expanding the compressed data passed through
		/// and forwarding it to the given output stream.
		///
		/// Please refer to the zlib documentation of inflateInit2() for a description
		/// of the windowBits parameter.

	~InflatingOutputStream() override;
	/// Destroys the InflatingOutputStream.

	int close();
		/// Finishes up the stream.
		///
		/// Must be called to ensure all data is properly written to
		/// the target output stream.
};


class Foundation_API InflatingInputStream: public std::istream, public InflatingIOS
	/// This stream decompresses all data passing through it
	/// using zlib's inflate algorithm.
	/// Example:
	///     std::ifstream istr("data.gz", std::ios::binary);
	///     InflatingInputStream inflater(istr, InflatingStreamBuf::STREAM_GZIP);
	///     std::string data;
	///     inflater >> data;
	///
	/// The underlying input stream can contain more than one gzip/deflate stream.
	/// After a gzip/deflate stream has been processed, reset() can be called
	/// to inflate the next stream.
{
public:
	InflatingInputStream(std::istream& istr, InflatingStreamBuf::StreamType type = InflatingStreamBuf::STREAM_ZLIB);
		/// Creates an InflatingInputStream for expanding the compressed data read from
		/// the given input stream.

	InflatingInputStream(std::istream& istr, int windowBits);
		/// Creates an InflatingInputStream for expanding the compressed data read from
		/// the given input stream.
		///
		/// Please refer to the zlib documentation of inflateInit2() for a description
		/// of the windowBits parameter.

	~InflatingInputStream() override;
	/// Destroys the InflatingInputStream.

	void reset();
		/// Resets the zlib machinery so that another zlib stream can be read from
		/// the same underlying input stream, and clears the stream state.
		///
		/// See InflatingStreamBuf::reset() for how buffered input is
		/// handled after a decompression error.
};


} // namespace Poco


#endif // Foundation_InflatingStream_INCLUDED
