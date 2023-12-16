//
// DataURIStream.h
//
// Library: Foundation
// Package: Streams
// Module:  DataURIStreamFactory
//
// Definition of the DataURIStream class.
//
// Copyright (c) 2019, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_DataURIStream_INCLUDED
#define Foundation_DataURIStream_INCLUDED


#include "Poco/Foundation.h"
#include <istream>
#include <memory>


namespace Poco {


class Base64Decoder;
class MemoryInputStream;
class URI;


class Foundation_API DataURIStreamIOS: public virtual std::ios
	/// The base class for DataURIStream.
	///
	/// This class is needed to ensure the correct initialization
	/// order of the stream buffer and base classes.
{
public:
	DataURIStreamIOS(const URI& uri);
	~DataURIStreamIOS() override;
	std::streambuf* rdbuf();

protected:
	std::streambuf* _buf;

private:
	DataURIStreamIOS(const DataURIStreamIOS&);
	DataURIStreamIOS& operator = (const DataURIStreamIOS&);
	std::string _data;
	std::unique_ptr<MemoryInputStream> _memoryStream;
	std::unique_ptr<Base64Decoder> _base64Decoder;
};


class Foundation_API DataURIStream: public DataURIStreamIOS, public std::istream
	/// An input stream for reading data from a data URI.
	/// For example, when constructed from  "data:text/plain;base64,SGVsbG8sIFdvcmxkIQ%3D%3D" it reads "Hello, World!".
{
public:
	DataURIStream(const URI& uri);
		/// Creates a DataURIStream for the given data URI,
		/// ready for reading data.
		/// Throws a DataFormatException exception if the data is incorrect format.
	~DataURIStream() override;
		/// Destroys the DataURIStream.

private:
	DataURIStream(const DataURIStream&);
	DataURIStream& operator = (const DataURIStream&);
};


} // namespace Poco


#endif // Foundation_DataURIStream_INCLUDED
