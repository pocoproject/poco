//
// EscapeHTMLStream.h
//
// Library: Net
// Package: HTTP
// Module:  EscapeHTMLStream
//
// Definition of the EscapeHTMLStream class.
//
// Copyright (c) 1029, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_EscapeHTMLStream_INCLUDED
#define Net_EscapeHTMLStream_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/UnbufferedStreamBuf.h"
#include <ostream>


namespace Poco {
namespace Net {


class Net_API EscapeHTMLStreamBuf: public Poco::UnbufferedStreamBuf
	/// This stream buffer replaces all occurrences of special HTML
	/// characters < > " & with their respective character
	/// entities &lt; &gt; &quot; &amp;.
{
public:
	EscapeHTMLStreamBuf(std::ostream& ostr);
		/// Creates the EscapeHTMLStreamBuf and connects it
		/// to the given output stream.

	~EscapeHTMLStreamBuf();
		/// Destroys the EscapeHTMLStreamBuf.

protected:
	int readFromDevice();
	int writeToDevice(char c);

private:
	std::ostream* _pOstr;
};


class Net_API EscapeHTMLIOS: public virtual std::ios
	/// The base class for EscapeHTMLOutputStream.
{
public:
	EscapeHTMLIOS(std::ostream& ostr);
		/// Creates the MailIOS and connects it
		/// to the given output stream.

	~EscapeHTMLIOS();
		/// Destroys the stream.

	EscapeHTMLStreamBuf* rdbuf();
		/// Returns a pointer to the underlying streambuf.

protected:
	EscapeHTMLStreamBuf _buf;
};


class Net_API EscapeHTMLOutputStream: public EscapeHTMLIOS, public std::ostream
	/// This stream replaces all occurrences of special HTML
	/// characters < > " & with their respective character
	/// entities &lt; &gt; &quot; &amp;.
{
public:
	EscapeHTMLOutputStream(std::ostream& ostr);
		/// Creates the MailOutputStream and connects it
		/// to the given input stream.

	~EscapeHTMLOutputStream();
		/// Destroys the MailOutputStream.
};


} } // namespace Poco::Net


#endif // Net_EscapeHTMLStream_INCLUDED
