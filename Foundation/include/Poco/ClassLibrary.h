//
// ClassLibrary.h
//
// Library: Foundation
// Package: SharedLibrary
// Module:  ClassLoader
//
// Definitions for class libraries.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_ClassLibrary_INCLUDED
#define Foundation_ClassLibrary_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Manifest.h"
#include <typeinfo>


#if defined(_WIN32)
	#define POCO_LIBRARY_API __declspec(dllexport)
#elif defined(__GNUC__) && (__GNUC__ >= 4)
	#define POCO_LIBRARY_API __attribute__ ((visibility ("default")))
#else
	#define POCO_LIBRARY_API
#endif


//
// the entry points for every class library
//
extern "C"
{
	bool POCO_LIBRARY_API pocoBuildManifest(Poco::ManifestBase* pManifest);
	void POCO_LIBRARY_API pocoInitializeLibrary();
	void POCO_LIBRARY_API pocoUninitializeLibrary();
}


//
// additional support for named manifests
//
#define POCO_DECLARE_NAMED_MANIFEST(name) \
extern "C"	\
{			\
	bool POCO_LIBRARY_API POCO_JOIN(pocoBuildManifest, name)(Poco::ManifestBase* pManifest); \
}


//
// Macros to automatically implement pocoBuildManifest
//
// usage:
//
// POCO_BEGIN_MANIFEST(MyBaseClass)
//     POCO_EXPORT_CLASS(MyFirstClass)
//     POCO_EXPORT_CLASS(MySecondClass)
//     ...
// POCO_END_MANIFEST
//
#define POCO_BEGIN_MANIFEST_IMPL(fnName, base) \
	bool fnName(Poco::ManifestBase* pManifest_)										\
	{																				\
		using BaseType = base;														\
		using ManifestType = Poco::Manifest<BaseType>;								\
		const std::string requiredType(typeid(ManifestType).name());				\
		const std::string actualType(pManifest_->className());						\
		if (requiredType == actualType)												\
		{																			\
			Poco::Manifest<BaseType>* pManifest = static_cast<ManifestType*>(pManifest_);


#define POCO_BEGIN_MANIFEST(base) \
	POCO_BEGIN_MANIFEST_IMPL(pocoBuildManifest, base)


#define POCO_BEGIN_NAMED_MANIFEST(name, base)	\
	POCO_DECLARE_NAMED_MANIFEST(name)			\
	POCO_BEGIN_MANIFEST_IMPL(POCO_JOIN(pocoBuildManifest, name), base)


#define POCO_END_MANIFEST \
			return true;	\
		}					\
		else return false;	\
	}


#define POCO_EXPORT_CLASS(cls) \
	pManifest->insert(new Poco::MetaObject<cls, BaseType>(#cls));


#define POCO_EXPORT_INTERFACE(cls, itf) \
	pManifest->insert(new Poco::MetaObject<cls, BaseType>(itf));


#define POCO_EXPORT_SINGLETON(cls) \
	pManifest->insert(new Poco::MetaSingleton<cls, BaseType>(#cls));


#endif // Foundation_ClassLibrary_INCLUDED
