//
// MultipartReader.h
//
// Library: Net
// Package: Messages
// Module:  MultipartReader
//
// Definition of the MultipartReader class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_MultipartReader_INCLUDED
#define Net_MultipartReader_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/BufferedStreamBuf.h"
#include <memory>
#include <istream>
#include <string>


namespace Poco::Net {


class MessageHeader;
class ReadWindow;


class Net_API MultipartStreamBuf: public Poco::BufferedStreamBuf
	/// This is the streambuf class used for reading from a multipart message stream.
{
public:
	MultipartStreamBuf(std::istream& istr, const std::string& boundary);
		/// Creates the MultipartStreamBuf and connects it
		/// to the given input stream.

	MultipartStreamBuf(std::istream& istr, const std::string& boundary, std::streamsize contentLength);
		/// Creates the MultipartStreamBuf with a known Content-Length
		/// for the current part, enabling bulk-read optimization.

	~MultipartStreamBuf();
	[[nodiscard]]
	bool lastPart() const;

protected:
	std::streamsize readFromDevice(char* buffer, std::streamsize length);

private:
	std::streamsize readContent(char* buffer, std::streamsize length);
		/// Reads content bytes, using bulk sgetn when Content-Length is known,
		/// or bulk-read + in-buffer boundary scanning otherwise.

	void consumePostBoundaryDelimiter();
		/// After a boundary match, consumes the trailing delimiter
		/// (CRLF for next part, or "--" for last part).

	std::streamsize scanForBoundary(const char* data, std::streamsize size);
		/// Searches for "\r\n--boundary" or "\n--boundary" in data[0..size).
		/// Returns the offset of the \r or \n that starts the match,
		/// or -1 if not found.

	static constexpr std::streamsize STREAM_BUFFER_SIZE = 32768;

	std::istream&      _istr;
	std::string        _boundary;
	bool               _lastPart;
	bool               _boundaryFound;
	std::streamsize    _contentLength;
	std::streamsize    _bytesRead;
};


class Net_API MultipartIOS: public virtual std::ios
	/// The base class for MultipartInputStream.
{
public:
	MultipartIOS(std::istream& istr, const std::string& boundary);
	MultipartIOS(std::istream& istr, const std::string& boundary, std::streamsize contentLength);
	~MultipartIOS();
	[[nodiscard]]
	MultipartStreamBuf* rdbuf();
	[[nodiscard]]
	bool lastPart() const;

protected:
	MultipartStreamBuf _buf;
};


class Net_API MultipartInputStream: public MultipartIOS, public std::istream
	/// This class is for internal use by MultipartReader only.
{
public:
	MultipartInputStream(std::istream& istr, const std::string& boundary);
	MultipartInputStream(std::istream& istr, const std::string& boundary, std::streamsize contentLength);
	~MultipartInputStream();
};



class Net_API MultipartReader
	/// This class is used to split a MIME multipart
	/// message into its single parts.
	///
	/// The format of multipart messages is described
	/// in section 5.1 of RFC 2046.
	///
	/// To split a multipart message into its parts,
	/// do the following:
	///   - Create a MultipartReader object, passing it
	///     an input stream and optionally a boundary string.
	///   - while hasNextPart() returns true, call nextPart()
	///     and read the part from stream().
	///
	/// Always ensure that you read all data from the part
	/// stream, otherwise the MultipartReader will fail to
	/// find the next part.
{
public:
	explicit MultipartReader(std::istream& istr);
		/// Creates the MultipartReader and attaches it to the
		/// given input stream.
		///
		/// The boundary string is determined from the input
		/// stream. The message must not contain a preamble
		/// preceding the first encapsulation boundary.

	MultipartReader(std::istream& istr, const std::string& boundary);
		/// Creates the MultipartReader and attaches it to the
		/// given input stream. The given boundary string is
		/// used to find message boundaries.

	~MultipartReader();
		/// Destroys the MultipartReader.

	void nextPart(MessageHeader& messageHeader);
		/// Moves to the next part in the message and stores the
		/// part's header fields in messageHeader.
		///
		/// Throws an MultipartException if there are no more parts
		/// available, or if no boundary line can be found in
		/// the input stream.

	[[nodiscard]]
	bool hasNextPart();
		/// Returns true iff more parts are available.
		///
		/// Before the first call to nextPart(), returns
		/// always true.

	[[nodiscard]]
	std::istream& stream() const;
		/// Returns a reference to the reader's stream that
		/// can be used to read the current part.
		///
		/// The returned reference will be valid until
		/// nextPart() is called or the MultipartReader
		/// object is destroyed.

	[[nodiscard]]
	const std::string& boundary() const;
		/// Returns the multipart boundary used by this reader.

protected:
	void findFirstBoundary();
	void guessBoundary();
	void parseHeader(MessageHeader& messageHeader);
	[[nodiscard]]
	bool readLine(std::string& line, std::string::size_type n);

	MultipartReader() = delete;
	MultipartReader(const MultipartReader&) = delete;
	MultipartReader& operator = (const MultipartReader&) = delete;

private:
	std::istream&         _istr;
	std::string           _boundary;
	std::unique_ptr<ReadWindow>       _window;   /// Top-level read-ahead buffer wrapping _istr
	std::unique_ptr<std::istream>     _windowStr; /// Persistent istream over _window
	std::unique_ptr<MultipartInputStream> _pMPI;
};


} // namespace Poco::Net


#endif // Net_MultipartReader_INCLUDED
