//
// CpuFeatures.cpp
//
// Library: Foundation
// Package: Core
// Module:  CpuFeatures
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "CpuFeatures.h"


#if POCO_ARCH == POCO_ARCH_AMD64
#include "CpuFeatures_X86.cpp"
#elif POCO_ARCH == POCO_ARCH_AARCH64
#include "CpuFeatures_ARM.cpp"
#else


namespace Poco {


namespace {


[[nodiscard]] CpuFeatures detectCpuFeatures()
	// other architectures detect nothing
{
	return {};
}


} // namespace


} // namespace Poco


#endif


namespace Poco {


const CpuFeatures& cpuFeatures()
{
	static const CpuFeatures features = detectCpuFeatures();
	return features;
}


} // namespace Poco
