//
// ErrorQueueCleaner.h
//
// Definition of the ErrorQueueCleaner class.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ErrorQueueCleaner_INCLUDED
#define ErrorQueueCleaner_INCLUDED


#include <openssl/err.h>


class ErrorQueueCleaner
	/// Empties the OpenSSL error queue of the thread, also when an assertion fails.
{
public:
	~ErrorQueueCleaner()
	{
		ERR_clear_error();
	}
};


#endif // ErrorQueueCleaner_INCLUDED
