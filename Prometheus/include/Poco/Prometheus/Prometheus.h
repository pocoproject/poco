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
// The following block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the Prometheus_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// Prometheus_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
//
#if defined(_WIN32) && defined(POCO_DLL)
	#if defined(Prometheus_EXPORTS)
		#define Prometheus_API __declspec(dllexport)
	#else
		#define Prometheus_API __declspec(dllimport)
	#endif
#endif


#if !defined(Prometheus_API)
	#if !defined(POCO_NO_GCC_API_ATTRIBUTE) && defined (__GNUC__) && (__GNUC__ >= 4)
		#define Prometheus_API __attribute__ ((visibility ("default")))
	#else
		#define Prometheus_API
	#endif
#endif


//
// Automatically link Prometheus library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(Prometheus_EXPORTS)
		#pragma comment(lib, "PocoPrometheus" POCO_LIB_SUFFIX)
	#endif
#endif


#endif // Prometheus_Prometheus_INCLUDED
