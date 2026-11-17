//
// DecoderError.h
//
// Library: Foundation
// Package: Streams
// Module:  DecoderError
//
// Helper for the messages of the DataFormatExceptions of the
// Base64, Base32 and hex decoders.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_DecoderError_INCLUDED
#define Foundation_DecoderError_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/NumberFormatter.h"
#include <string>


namespace Poco {


inline std::string describeDecoderChar(int c)
	/// Returns c in quotes if it is printable, and as 0xNN otherwise.
{
	if (c > 0x20 && c < 0x7F)
		return std::string(1, '\'') + static_cast<char>(c) + '\'';
	else
		return "0x" + NumberFormatter::formatHex(static_cast<unsigned>(c), 2);
}


} // namespace Poco


#endif // Foundation_DecoderError_INCLUDED
