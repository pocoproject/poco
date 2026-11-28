//
// ValueTestTypes.h
//
// Value types for the tests of Any and Var.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#ifndef ValueTestTypes_INCLUDED
#define ValueTestTypes_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Exception.h"


class CopyCounter
	/// A value that counts how often values of its type are copied.
{
public:
	CopyCounter() = delete;

	explicit CopyCounter(int value): _value(value)
	{
	}

	CopyCounter(const CopyCounter& other): _value(other._value)
	{
		++_copies;
	}

	CopyCounter(CopyCounter&& other) noexcept = default;

	~CopyCounter() = default;

	CopyCounter& operator = (const CopyCounter& other) = delete;
	CopyCounter& operator = (CopyCounter&& other) = delete;

	[[nodiscard]] bool operator == (const CopyCounter& other) const
	{
		return _value == other._value;
	}

	[[nodiscard]] int value() const
	{
		return _value;
	}

	[[nodiscard]] static int copies()
	{
		return _copies;
	}

	static void resetCopies()
	{
		_copies = 0;
	}

private:
	int _value;
	static inline int _copies = 0;
};


class ThrowOnCopy
	/// A value whose copy constructor throws a RuntimeException
	/// if the value was created as throwing.
{
public:
	ThrowOnCopy() = delete;

	explicit ThrowOnCopy(bool throwing): _throwing(throwing)
	{
	}

	ThrowOnCopy(const ThrowOnCopy& other): _throwing(other._throwing)
	{
		if (_throwing) throw Poco::RuntimeException("copy of ThrowOnCopy");
	}

	ThrowOnCopy(ThrowOnCopy&& other) noexcept = default;

	~ThrowOnCopy() = default;

	ThrowOnCopy& operator = (const ThrowOnCopy& other) = delete;
	ThrowOnCopy& operator = (ThrowOnCopy&& other) = delete;

private:
	bool _throwing;
};


class ThrowOnMove
	/// A small value whose copy constructor is noexcept and whose move constructor
	/// throws a RuntimeException if the value was created as throwing.
{
public:
	ThrowOnMove() = delete;

	explicit ThrowOnMove(bool throwing): _throwing(throwing)
	{
	}

	ThrowOnMove(const ThrowOnMove& other) noexcept = default;

	ThrowOnMove(ThrowOnMove&& other): _throwing(other._throwing)
	{
		if (_throwing) throw Poco::RuntimeException("move of ThrowOnMove");
	}

	~ThrowOnMove() = default;

	ThrowOnMove& operator = (const ThrowOnMove& other) = delete;
	ThrowOnMove& operator = (ThrowOnMove&& other) = delete;

	[[nodiscard]] bool throwing() const
	{
		return _throwing;
	}

private:
	bool _throwing;
};


class MayThrowOnMove
	/// A small value whose copy and move constructors are not noexcept.
{
public:
	MayThrowOnMove() = default;

	MayThrowOnMove(const MayThrowOnMove& other): _value(other._value)
	{
	}

	MayThrowOnMove(MayThrowOnMove&& other): _value(other._value)
	{
	}

	~MayThrowOnMove() = default;

	MayThrowOnMove& operator = (const MayThrowOnMove& other) = delete;
	MayThrowOnMove& operator = (MayThrowOnMove&& other) = delete;

private:
	int _value = 0;
};


class CopyOnly
	/// A copyable value whose move constructor and move assignment are deleted.
{
public:
	CopyOnly() = delete;

	explicit CopyOnly(int value): _value(value)
	{
	}

	CopyOnly(const CopyOnly& other) = default;
	CopyOnly(CopyOnly&& other) = delete;

	~CopyOnly() = default;

	CopyOnly& operator = (const CopyOnly& other) = default;
	CopyOnly& operator = (CopyOnly&& other) = delete;

	[[nodiscard]] int value() const
	{
		return _value;
	}

private:
	int _value;
};


struct alignas(32) OverAligned
	/// A value that needs a stronger alignment than std::max_align_t.
{
	int value = 0;
};


#endif // ValueTestTypes_INCLUDED
