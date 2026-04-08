//
// Poco.cppm
//
// C++ module file, re-exporting all other modules
//
// Copyright (c) 2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

export module Poco;

export import :Foundation;

#ifdef ENABLE_ACTIVERECORD
export import :ActiveRecord;
#endif
#ifdef ENABLE_CPPPARSER
export import :CppParser;
#endif
#ifdef ENABLE_CRYPTO
export import :Crypto;
#endif
#ifdef ENABLE_DNSSD
export import :DNSSD;
#endif
#ifdef ENABLE_DATA
export import :Data;
#endif
#ifdef ENABLE_JSON
export import :JSON;
#endif
#ifdef ENABLE_JWT
export import :JWT;
#endif
#ifdef ENABLE_MONGODB
export import :MongoDB;
#endif
#ifdef ENABLE_NET
export import :Net;
#endif
#ifdef ENABLE_PDF
export import :PDF;
#endif
#ifdef ENABLE_PROMETHEUS
export import :Prometheus;
#endif
#ifdef ENABLE_REDIS
export import :Redis;
#endif
#ifdef ENABLE_SEVENZIP
export import :SevenZip;
#endif
#ifdef ENABLE_UTIL
export import :Util;
#endif
#ifdef ENABLE_XML
export import :XML;
#endif
#ifdef ENABLE_ZIP
export import :Zip;
#endif
