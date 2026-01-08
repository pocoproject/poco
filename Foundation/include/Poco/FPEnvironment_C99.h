//
// FPEnvironment_C99.h
//
// Library: Foundation
// Package: Core
// Module:  FPEnvironment
//
// Definitions of class FPEnvironmentImpl for C99.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_FPEnvironment_C99_INCLUDED
#define Foundation_FPEnvironment_C99_INCLUDED


#include "Poco/Foundation.h"
#include <fenv.h>
#include <cmath>


namespace Poco {


class Foundation_API FPEnvironmentImpl
{
protected:
	enum RoundingModeImpl
	{
#if defined(FE_DOWNWARD)
		FP_ROUND_DOWNWARD_IMPL   = FE_DOWNWARD,
#else
		FP_ROUND_DOWNWARD_IMPL   = 0,
#endif
#if defined(FE_UPWARD)
		FP_ROUND_UPWARD_IMPL     = FE_UPWARD,
#else
		FP_ROUND_UPWARD_IMPL     = 0,
#endif
#if defined(FE_TONEAREST)
		FP_ROUND_TONEAREST_IMPL  = FE_TONEAREST,
#else
		FP_ROUND_TONEAREST_IMPL  = 0,
#endif
#if defined(FE_TOWARDZERO)
		FP_ROUND_TOWARDZERO_IMPL = FE_TOWARDZERO
#else
		FP_ROUND_TOWARDZERO_IMPL = 0
#endif
	};
	enum FlagImpl
	{
#if defined(FE_DIVBYZERO)
		FP_DIVIDE_BY_ZERO_IMPL = FE_DIVBYZERO,
#else
		FP_DIVIDE_BY_ZERO_IMPL = 0,
#endif
#if defined(FE_INEXACT)
		FP_INEXACT_IMPL        = FE_INEXACT,
#else
		FP_INEXACT_IMPL        = 0,
#endif
#if defined(FE_OVERFLOW)
		FP_OVERFLOW_IMPL       = FE_OVERFLOW,
#else
		FP_OVERFLOW_IMPL       = 0,
#endif
#if defined(FE_UNDERFLOW)
		FP_UNDERFLOW_IMPL      = FE_UNDERFLOW,
#else
		FP_UNDERFLOW_IMPL      = 0,
#endif
#if defined(FE_INVALID)
		FP_INVALID_IMPL        = FE_INVALID
#else
		FP_INVALID_IMPL        = 0
#endif
	};
	FPEnvironmentImpl();
	FPEnvironmentImpl(const FPEnvironmentImpl& env);
	~FPEnvironmentImpl();
	FPEnvironmentImpl& operator = (const FPEnvironmentImpl& env);
	void keepCurrentImpl();
	static void clearFlagsImpl();
	[[nodiscard]]
	static bool isFlagImpl(FlagImpl flag);
	static void setRoundingModeImpl(RoundingModeImpl mode);
	[[nodiscard]]
	static RoundingModeImpl getRoundingModeImpl();
	[[nodiscard]]
	static bool isInfiniteImpl(float value);
	[[nodiscard]]
	static bool isInfiniteImpl(double value);
	[[nodiscard]]
	static bool isInfiniteImpl(long double value);
	[[nodiscard]]
	static bool isNaNImpl(float value);
	[[nodiscard]]
	static bool isNaNImpl(double value);
	[[nodiscard]]
	static bool isNaNImpl(long double value);
	[[nodiscard]]
	static float copySignImpl(float target, float source);
	[[nodiscard]]
	static double copySignImpl(double target, double source);
	[[nodiscard]]
	static long double copySignImpl(long double target, long double source);

private:
	fenv_t _env;
};


//
// inlines
//
inline bool FPEnvironmentImpl::isInfiniteImpl(float value)
{
#if POCO_OS == POCO_OS_AIX
	return ::isinf(value) != 0;
#else
	return std::isinf(value) != 0;
#endif
}


inline bool FPEnvironmentImpl::isInfiniteImpl(double value)
{
#if POCO_OS == POCO_OS_AIX
	return ::isinf(value) != 0;
#else
	return std::isinf(value) != 0;
#endif
}


inline bool FPEnvironmentImpl::isInfiniteImpl(long double value)
{
#if POCO_OS == POCO_OS_AIX
	return ::isinf((double) value) != 0;
#else
	return std::isinf((double) value) != 0;
#endif
}


inline bool FPEnvironmentImpl::isNaNImpl(float value)
{
#if POCO_OS == POCO_OS_AIX
	return ::isnan(value) != 0;
#else
	return std::isnan(value) != 0;
#endif
}


inline bool FPEnvironmentImpl::isNaNImpl(double value)
{
#if POCO_OS == POCO_OS_AIX
	return ::isnan(value) != 0;
#else
	return std::isnan(value) != 0;
#endif
}


inline bool FPEnvironmentImpl::isNaNImpl(long double value)
{
#if POCO_OS == POCO_OS_AIX
	return ::isnan((double) value) != 0;
#else
	return std::isnan((double) value) != 0;
#endif
}


inline float FPEnvironmentImpl::copySignImpl(float target, float source)
{
	return copysignf(target, source);
}


inline double FPEnvironmentImpl::copySignImpl(double target, double source)
{
	return copysign(target, source);
}


} // namespace Poco


#endif // Foundation_FPEnvironment_C99_INCLUDED
