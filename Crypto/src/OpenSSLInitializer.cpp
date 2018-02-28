//
// OpenSSLInitializer.cpp
//
// Library: Crypto
// Package: CryptoCore
// Module:  OpenSSLInitializer
//
// Copyright (c) 2006-2009, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Crypto/OpenSSLInitializer.h"
#include "Poco/RandomStream.h"
#include "Poco/Thread.h"
#include <openssl/ssl.h>
#include <openssl/rand.h>
#include <openssl/crypto.h>
#include <openssl/err.h>
#if OPENSSL_VERSION_NUMBER >= 0x0907000L
#include <openssl/conf.h>
#endif
#if defined(POCO_OS_FAMILY_WINDOWS)
	#define POCO_STR_HELPER(x) #x
	#define POCO_STR(x) POCO_STR_HELPER(x)
	#if defined POCO_INTERNAL_OPENSSL_MSVC_VER
		#define POCO_INTERNAL_OPENSSL_BUILD          \
				" (POCO internal build, MSVC version " \
				POCO_STR(POCO_INTERNAL_OPENSSL_MSVC_VER) ")"
	#else
		#define POCO_INTERNAL_OPENSSL_BUILD ""
	#endif
	#pragma message (OPENSSL_VERSION_TEXT POCO_INTERNAL_OPENSSL_BUILD)
#endif


using Poco::RandomInputStream;
using Poco::Thread;


#if defined(_MSC_VER) && !defined(_DLL) && defined(POCO_INTERNAL_OPENSSL_MSVC_VER)

	#if (POCO_MSVS_VERSION >= 2015)
		FILE _iob[] = { *stdin, *stdout, *stderr };
		extern "C" FILE * __cdecl __iob_func(void) { return _iob; }
	#endif // (POCO_MSVS_VERSION >= 2015)

	#if (POCO_MSVS_VERSION < 2012)
		extern "C" __declspec(noreturn) void __cdecl __report_rangecheckfailure(void) { ::ExitProcess(1); }
	#endif // (POCO_MSVS_VERSION < 2012)

#endif // defined(_MSC_VER) && !defined(_DLL) && defined(POCO_INTERNAL_OPENSSL_MSVC_VER)


namespace Poco {
namespace Crypto {

Poco::FastMutex OpenSSLInitializer::_mutex;
Poco::FastMutex* OpenSSLInitializer::_mutexes(0);
int OpenSSLInitializer::_rc(0);
bool OpenSSLInitializer::_disableSSLInitialization = false;

OpenSSLInitializer::OpenSSLInitializer()
{
	initialize();
}


OpenSSLInitializer::~OpenSSLInitializer()
{
	try
	{
		uninitialize();
	}
	catch (...)
	{
		poco_unexpected();
	}
}


void OpenSSLInitializer::initialize()
{
	FastMutex::ScopedLock lock(_mutex);
	if (++_rc == 1)
	{
#if OPENSSL_VERSION_NUMBER >= 0x0907000L
		OPENSSL_config(NULL);
#endif
		if(! _disableSSLInitialization)
		{
			SSL_library_init();
			SSL_load_error_strings();
			OpenSSL_add_all_algorithms();
		}

		char seed[SEEDSIZE];
		RandomInputStream rnd;
		rnd.read(seed, sizeof(seed));
		RAND_seed(seed, SEEDSIZE);

		if(CRYPTO_get_locking_callback() == NULL)
		{
			int nMutexes = CRYPTO_num_locks();
			_mutexes = new Poco::FastMutex[nMutexes];
			CRYPTO_set_locking_callback(&OpenSSLInitializer::lock);
#ifndef POCO_OS_FAMILY_WINDOWS
// Not needed on Windows (see SF #110: random unhandled exceptions when linking with ssl).
// https://sourceforge.net/p/poco/bugs/110/
//
// From http://www.openssl.org/docs/crypto/threads.html :
// "If the application does not register such a callback using CRYPTO_THREADID_set_callback(),
//  then a default implementation is used - on Windows and BeOS this uses the system's
//  default thread identifying APIs"
#ifndef OPENSSL_NO_DEPRECATED
			CRYPTO_set_id_callback(&OpenSSLInitializer::id);
#else
			CRYPTO_THREADID tid;
			CRYPTO_THREADID_set_numeric(&tid, OpenSSLInitializer::id());
#endif /* OPENSSL_NO_DEPRECATED */
#endif
			CRYPTO_set_dynlock_create_callback(&OpenSSLInitializer::dynlockCreate);
			CRYPTO_set_dynlock_lock_callback(&OpenSSLInitializer::dynlock);
			CRYPTO_set_dynlock_destroy_callback(&OpenSSLInitializer::dynlockDestroy);
		}
	}
}


void OpenSSLInitializer::uninitialize()
{
	FastMutex::ScopedLock lock(_mutex);
	if (--_rc == 0)
	{
		if(_mutexes != NULL)
		{
			CRYPTO_set_dynlock_create_callback(0);
			CRYPTO_set_dynlock_lock_callback(0);
			CRYPTO_set_dynlock_destroy_callback(0);
			CRYPTO_set_locking_callback(0);
#ifndef POCO_OS_FAMILY_WINDOWS
#ifndef OPENSSL_NO_DEPRECATED
			CRYPTO_set_id_callback(0);
#else
			CRYPTO_THREADID tid;
			CRYPTO_THREADID_set_numeric(&tid, 0);
#endif /* OPENSSL_NO_DEPRECATED */
#endif
			delete [] _mutexes;
		}

		if(! _disableSSLInitialization)
		{
			EVP_cleanup();
			ERR_free_strings();
			CONF_modules_free();
		}
	}
}


void OpenSSLInitializer::lock(int mode, int n, const char* file, int line)
{
	if (mode & CRYPTO_LOCK)
		_mutexes[n].lock();
	else
		_mutexes[n].unlock();
}


#ifndef POCO_OS_FAMILY_WINDOWS

unsigned long OpenSSLInitializer::id()
{
	// Note: we use an old-style C cast here because
	// neither static_cast<> nor reinterpret_cast<>
	// work uniformly across all platforms.
	return (unsigned long) Poco::Thread::currentTid();
}

#endif


struct CRYPTO_dynlock_value* OpenSSLInitializer::dynlockCreate(const char* file, int line)
{
	return new CRYPTO_dynlock_value;
}


void OpenSSLInitializer::dynlock(int mode, struct CRYPTO_dynlock_value* lock, const char* file, int line)
{
	poco_check_ptr (lock);

	if (mode & CRYPTO_LOCK)
		lock->_mutex.lock();
	else
		lock->_mutex.unlock();
}


void OpenSSLInitializer::dynlockDestroy(struct CRYPTO_dynlock_value* lock, const char* file, int line)
{
	delete lock;
}


void initializeCrypto()
{
	OpenSSLInitializer::initialize();
}


void uninitializeCrypto()
{
	OpenSSLInitializer::uninitialize();
}


} } // namespace Poco::Crypto
