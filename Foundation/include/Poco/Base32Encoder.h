//
// Base32Encoder.h
//
// Library: Foundation
// Package: Streams
// Module:  Base32
//
// Definition of class Base32Encoder.
//
// Copyright (c) 2004-2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_Base32Encoder_INCLUDED
#define Foundation_Base32Encoder_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/UnbufferedStreamBuf.h"
#include <ostream>


namespace Poco {


enum Base32EncodingOptions
{
	BASE32_NO_PADDING = 0x00,
		/// Don't append padding characters ('=') at end.
		///
		/// NOTE: This is provided for consistency with Base64EncodingOptions.

	BASE32_USE_PADDING = 0x01,
		/// Append padding characters ('=') at end.
		///
		/// NOTE: This is provided for backwards compatibility with constructor
		/// taking a bool padding argument.

	BASE32_USE_HEX_ALPHABET = 0x02,
		/// Use the RFC 4648 Base32 Extended Hex alphabet.

	BASE32_USE_CROCKFORD_ALPHABET = 0x04
		/// Use Crockford's Base 32 alphabet for encoding
		/// (https://www.crockford.com/base32.html)
		/// instead of RFC 4648 Base32 alphabet.
		///
		/// Note: decoding is not case sensitive.
		/// Furthermore characters 0/O/o and 1/I/i/L/l 
		/// are decoded as 0 and 1, respectively.
};


class Foundation_API Base32EncoderBuf: public UnbufferedStreamBuf
	/// This streambuf base32-encodes all data written
	/// to it and forwards it to a connected
	/// ostream.
	///
	/// Note: The characters are directly written
	/// to the ostream's streambuf, thus bypassing
	/// the ostream. The ostream's state is therefore
	/// not updated to match the buffer's state.
{
public:
	Base32EncoderBuf(std::ostream& ostr, int options = BASE32_USE_PADDING);
	~Base32EncoderBuf() override;

	int close();
		/// Closes the stream buffer.

protected:
	static const unsigned char* encoding(int options);
		/// Returns the alphabet to be used for encoding/decoding
		/// according to the specified options.

private:
	int writeToDevice(char c) override;

	unsigned char   _group[5];
	int             _groupLength;
	std::streambuf& _buf;
	bool		_doPadding;
	const unsigned char* _encoding;

	static const unsigned char DEFAULT_ENCODING[32];
	static const unsigned char HEX_ENCODING[32];
	static const unsigned char CROCKFORD_ENCODING[32];

	friend class Base32DecoderBuf;

	Base32EncoderBuf(const Base32EncoderBuf&);
	Base32EncoderBuf& operator = (const Base32EncoderBuf&);
};


class Foundation_API Base32EncoderIOS: public virtual std::ios
	/// The base class for Base32Encoder.
	///
	/// This class is needed to ensure the correct initialization
	/// order of the stream buffer and base classes.
{
public:
	Base32EncoderIOS(std::ostream& ostr, int options = BASE32_USE_PADDING);
	~Base32EncoderIOS() override;
	int close();
	[[nodiscard]]
	Base32EncoderBuf* rdbuf();

protected:
	Base32EncoderBuf _buf;

private:
	Base32EncoderIOS(const Base32EncoderIOS&);
	Base32EncoderIOS& operator = (const Base32EncoderIOS&);
};


class Foundation_API Base32Encoder: public Base32EncoderIOS, public std::ostream
	/// This ostream base32-encodes all data
	/// written to it and forwards it to
	/// a connected ostream.
	/// Always call close() when done
	/// writing data, to ensure proper
	/// completion of the encoding operation.
	///
	/// The class implements RFC 4648 - https://tools.ietf.org/html/rfc4648
	/// and additionally supports Crockford's Base 32.
	///
	/// Note: The characters are directly written
	/// to the ostream's streambuf, thus bypassing
	/// the ostream. The ostream's state is therefore
	/// not updated to match the buffer's state.
{
public:
	Base32Encoder(std::ostream& ostr, int options = BASE32_USE_PADDING);
		/// Creates the Base32Encoder with the given options.
		/// See Base32EncodingOptions for supported options.

	~Base32Encoder() override;

private:
	Base32Encoder(const Base32Encoder&);
	Base32Encoder& operator = (const Base32Encoder&);
};


} // namespace Poco


#endif // Foundation_Base32Encoder_INCLUDED
