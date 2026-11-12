//
// Prometheus.h
//
// Library: Prometheus
// Package: Core
// Module:  Prometheus
//
// Basic definitions for the Poco Prometheus library.
// This file must be the first file included by every other Prometheus
// header file.
//
// Copyright (c) 2022, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Prometheus_Prometheus_INCLUDED
#define Prometheus_Prometheus_INCLUDED


#include "Poco/Foundation.h"


//
// Prometheus_EXPORTS is defined only while this library is compiled (see POCO_API in Poco/Foundation.h).
//
#define Prometheus_API POCO_API(Prometheus_EXPORTS)


//
// Automatically link Prometheus library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(Prometheus_EXPORTS)
		#pragma comment(lib, "PocoPrometheus" POCO_LIB_SUFFIX)
	#endif
#endif


#endif // Prometheus_Prometheus_INCLUDED
