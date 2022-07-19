//
// DataURIStreamFactory.h
//
// Library: Foundation
// Package: URI
// Module:  DataURIStreamFactory
//
// Definition of the DataURIStreamFactory class.
//
// Copyright (c) 2019, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_DataURIStreamFactory_INCLUDED
#define Foundation_DataURIStreamFactory_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/URIStreamFactory.h"


namespace Poco {


class Foundation_API DataURIStreamFactory: public URIStreamFactory
	/// An implementation of the URIStreamFactory interface
	/// that read data from data URIs.
	/// For example, for URI "data:text/plain;base64,SGVsbG8sIFdvcmxkIQ%3D%3D" it read "Hello, World!".
{
public:
	DataURIStreamFactory();
		/// Creates the DataURIStreamFactory.

	~DataURIStreamFactory();
		/// Destroys the DataURIStreamFactory.

	std::istream* open(const URI& uri);
		/// Creates an input stream returning decoded data from the given data URI.
		///
		/// Throws a DataFormatException exception if the data is incorrect format.

	static void registerFactory();
		/// Registers the DataURIStreamFactory with the
		/// default URIStreamOpener instance.

	static void unregisterFactory();
		/// Unregisters the DataURIStreamFactory with the
		/// default URIStreamOpener instance.
};


} // namespace Poco


#endif // Foundation_DataURIStreamFactory_INCLUDED
