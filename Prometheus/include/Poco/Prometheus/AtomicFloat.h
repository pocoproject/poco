//
// AtomicFloat.h
//
// Library: Prometheus
// Package: Core
// Module:  AtomicFloat
//
// Definition of the AtomicFloat class template.
//
// Copyright (c) 2022, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Prometheus_AtomicFloat_INCLUDED
#define Prometheus_AtomicFloat_INCLUDED


#include <atomic>


namespace Poco {
namespace Prometheus {


template <typename T>
class AtomicFloat
	/// This class provides efficient atomic operations
	/// on float and double values.
{
public:
	AtomicFloat():
		_value(0.0)
	{
	}

	explicit AtomicFloat(T v):
		_value(v)
	{
	}

	AtomicFloat(const AtomicFloat& v):
		_value(v.value())
	{
	}

	AtomicFloat& operator = (const AtomicFloat& v)
	{
		_value.store(v.value());
		return *this;
	}

	AtomicFloat& operator = (T v)
	{
		_value.store(v);
		return *this;
	}

	operator T () const
	{
		return _value.load();
	}

	T value() const
	{
		return _value.load();
	}

	AtomicFloat& operator += (T value)
	{
		T expected = _value.load();
		T desired;
		do
		{
			desired = expected + value;
		}
		while (!_value.compare_exchange_weak(expected, desired));

		return *this;
	}

	AtomicFloat& operator -= (T value)
	{
		T expected = _value.load();
		T desired;
		do
		{
			desired = expected - value;
		}
		while (!_value.compare_exchange_weak(expected, desired));

		return *this;
	}

private:
	std::atomic<T> _value;
};


} } // namespace Poco::Prometheus


#endif // Prometheus_AtomicFloat_INCLUDED
