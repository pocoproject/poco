//
// Transcoder.h
//
// Library: Data
// Package: DataCore
// Module:  Transcoder
//
// Definition of the Transcoder class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Data_Transcoder_INCLUDED
#define Data_Transcoder_INCLUDED


#include "Poco/Data/Data.h"
#include "Poco/TextConverter.h"
#include "Poco/TextEncoding.h"
#include <memory>
#include <string>


namespace Poco {
namespace Data {


class Data_API Transcoder
	/// Utility class used to convert string data encoding.
	///
	/// The purpose of this class is to help with deciding
	/// whether conversion is actually required, and to keep
	/// the encodings lifetimes aligned with the converter lifetime.
{
public:
	using Ptr = std::unique_ptr<Transcoder>;
	using ConverterPtr = std::unique_ptr<Poco::TextConverter>;

	virtual ~Transcoder();
		/// Destroys the Transcoder.

	static Ptr create(Poco::TextEncoding::Ptr pFromEncoding = nullptr,
		Poco::TextEncoding::Ptr pToEncoding = nullptr);
		/// Returns a unique pointer to Transcode instance;
		/// if there is no need for transcoding, null pointer
		/// is returned.

	std::string fromEncoding() const;
		/// Returns "from" encoding canonical name.

	std::string toEncoding() const;
		/// Returns "from" encoding canonical name.

	void transcode(const std::string& from, std::string& to);
		/// Performs the conversion. Any prior content of the
		/// destination string is cleared.

	void reverseTranscode(const std::string& from, std::string& to);
		/// Performs the reverse conversion. Any prior content of the
		/// destination string is cleared.

private:
	Transcoder(Poco::TextEncoding::Ptr pFromEncoding,
		Poco::TextEncoding::Ptr pToEncoding);
		/// Creates the Transcoder.

	Poco::TextEncoding::Ptr _pFromEncoding;
	Poco::TextEncoding::Ptr _pToEncoding;
	ConverterPtr _pConverter;
	ConverterPtr _pReverseConverter;
};


//
// inlines
//

inline std::string Transcoder::fromEncoding() const
{
	return _pFromEncoding->canonicalName();
}


inline std::string Transcoder::toEncoding() const
{
	return _pToEncoding->canonicalName();
}


} } // namespace Poco::Data


#endif // Data_Transcoder_INCLUDED
