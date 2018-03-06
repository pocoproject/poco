//
// Checksum.h
//
// Library: Foundation
// Package: Core
// Module:  Checksum
//
// Definition of the Checksum class.
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_Checksum_INCLUDED
#define Foundation_Checksum_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/ChecksumImpl.h"


namespace Poco {


class Foundation_API Checksum
	/// This class calculates checksums for arbitrary data.
{
public:
	enum Type
	{
		TYPE_ADLER32 = ChecksumImpl::TYPE_ADLER32_IMPL,
		TYPE_CRC32 = ChecksumImpl::TYPE_CRC32_IMPL,
		TYPE_CRC64 = ChecksumImpl::TYPE_CRC64_IMPL
	};

	Checksum();
		/// Creates a CRC-32 checksum initialized to 0.

	Checksum(Type t);
		/// Creates the Checksum, using the given type.

	~Checksum();
		/// Destroys the Checksum.

	void update(const char* data, unsigned length);
		/// Updates the checksum with the given data.

	void update(const std::string& data);
		/// Updates the checksum with the given data.

	void update(char data);
		/// Updates the checksum with the given data.

	Poco::UInt64 checksum() const;
		/// Returns the calculated checksum.

	Type type() const;
		/// Which type of checksum are we calculating.

private:
	ChecksumImpl* _pImpl;
};


//
// inlines
//

inline void Checksum::update(const std::string& data)
{
	_pImpl->update(data.c_str(), static_cast<unsigned int>(data.size()));
}


inline void Checksum::update(char c)
{
	_pImpl->update(&c, 1);
}


inline void Checksum::update(const char* data, unsigned length)
{
	_pImpl->update(data, length);
}


inline Poco::UInt64 Checksum::checksum() const
{
	return _pImpl->checksum();
}


inline Checksum::Type Checksum::type() const
{
	return static_cast<Checksum::Type>(_pImpl->type());
}


} // namespace Poco


#endif // Foundation_Checksum_INCLUDED
