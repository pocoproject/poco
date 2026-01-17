//
// FPEnvironment_SUN.h
//
// Library: Foundation
// Package: Core
// Module:  FPEnvironment
//
// Definitions of class FPEnvironmentImpl for Solaris.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_FPEnvironment_SUN_INCLUDED
#define Foundation_FPEnvironment_SUN_INCLUDED


#include "Poco/Foundation.h"
#include <ieeefp.h>


namespace Poco {


class Foundation_API FPEnvironmentImpl
{
protected:
	enum RoundingModeImpl
	{
		FP_ROUND_DOWNWARD_IMPL   = FP_RM,
		FP_ROUND_UPWARD_IMPL     = FP_RP,
		FP_ROUND_TONEAREST_IMPL  = FP_RN,
		FP_ROUND_TOWARDZERO_IMPL = FP_RZ
	};
	enum FlagImpl
	{
		FP_DIVIDE_BY_ZERO_IMPL = FP_X_DZ,
		FP_INEXACT_IMPL        = FP_X_IMP,
		FP_OVERFLOW_IMPL       = FP_X_OFL,
		FP_UNDERFLOW_IMPL      = FP_X_UFL,
		FP_INVALID_IMPL        = FP_X_INV
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
	fp_rnd    _rnd;
	fp_except _exc;
};


} // namespace Poco


#endif // Foundation_FPEnvironment_SUN_INCLUDED
