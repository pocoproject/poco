//
// RenderingBinder.cpp
//
// Library: Data
// Package: DataCore
// Module:  RenderingBinder
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Data/RenderingBinder.h"
#include "Poco/Data/Utility.h"
#include "Poco/Data/LOB.h"
#include "Poco/Data/Date.h"
#include "Poco/Data/Time.h"
#include "Poco/DateTime.h"
#include "Poco/Exception.h"
#include "Poco/NumberFormatter.h"
#include "Poco/UnicodeConverter.h"
#include "Poco/UUID.h"
#include <algorithm>


namespace Poco {
namespace Data {


namespace {


// Per-scalar formatters mirror Utility::renderValue<T> but as plain
// non-template functions so the container code path can dispatch by overload.

inline std::string fmt(const Poco::Int8&  v) { return Poco::NumberFormatter::format(static_cast<int>(v)); }
inline std::string fmt(const Poco::UInt8& v) { return Poco::NumberFormatter::format(static_cast<unsigned>(v)); }
inline std::string fmt(const Poco::Int16& v) { return Poco::NumberFormatter::format(v); }
inline std::string fmt(const Poco::UInt16& v) { return Poco::NumberFormatter::format(v); }
inline std::string fmt(const Poco::Int32& v) { return Poco::NumberFormatter::format(v); }
inline std::string fmt(const Poco::UInt32& v) { return Poco::NumberFormatter::format(v); }
inline std::string fmt(const Poco::Int64& v) { return Poco::NumberFormatter::format(v); }
inline std::string fmt(const Poco::UInt64& v) { return Poco::NumberFormatter::format(v); }
#ifndef POCO_INT64_IS_LONG
inline std::string fmt(const long& v) { return Poco::NumberFormatter::format(v); }
inline std::string fmt(const unsigned long& v) { return Poco::NumberFormatter::format(v); }
#endif
inline std::string fmt(const bool& v) { return v ? "1" : "0"; }
inline std::string fmt(const float& v) { return Poco::NumberFormatter::format(v); }
inline std::string fmt(const double& v) { return Poco::NumberFormatter::format(v); }
inline std::string fmt(const char& v) { return Utility::quoteString(std::string(1, v)); }
inline std::string fmt(const std::string& v) { return Utility::quoteString(v); }
inline std::string fmt(const UTF16String& v)
{
	std::string utf8;
	Poco::UnicodeConverter::convert(v, utf8);
	return Utility::quoteString(utf8);
}
inline std::string fmt(const BLOB& v) { return Utility::formatBLOB(v); }
inline std::string fmt(const CLOB& v) { return Utility::formatCLOB(v); }
inline std::string fmt(const DateTime& v) { return Utility::formatDateTime(v); }
inline std::string fmt(const Date& v) { return Utility::formatDate(v); }
inline std::string fmt(const Time& v) { return Utility::formatTime(v); }
inline std::string fmt(const UUID& v) { return Utility::quoteString(v.toString()); }
inline std::string fmt(const NullData&) { return "NULL"; }


inline const std::string& outSentinel()
{
	static const std::string s("?");
	return s;
}


} // anonymous namespace


RenderingBinder::RenderingBinder(std::size_t maxRows):
	_maxRows(maxRows == 0 ? 1 : maxRows),
	_totalRows(1)
{
}


RenderingBinder::~RenderingBinder() = default;


void RenderingBinder::reset()
{
	_values.clear();
	_totalRows = 1;
}


template <typename T>
void RenderingBinder::recordScalar(std::size_t pos, const T& val, Direction dir)
{
	if (dir == PD_OUT)
		_values[pos] = { outSentinel() };
	else
		_values[pos] = { fmt(val) };
}


template <typename C>
void RenderingBinder::recordContainer(std::size_t pos, const C& container, Direction dir)
{
	const std::size_t actual = container.size();
	if (actual > _totalRows) _totalRows = actual;

	std::vector<std::string>& slot = _values[pos];
	slot.clear();
	if (dir == PD_OUT)
	{
		slot.push_back(outSentinel());
		return;
	}

	const std::size_t toRender = std::min(actual, _maxRows);
	slot.reserve(toRender);
	std::size_t i = 0;
	for (auto it = container.begin(); it != container.end() && i < toRender; ++it, ++i)
		slot.push_back(fmt(*it));
}


// --- Scalar bind() overrides ---

#define POCO_RENDERING_BINDER_SCALAR(T)                                          \
	void RenderingBinder::bind(std::size_t pos, const T& val, Direction dir)     \
	{ recordScalar(pos, val, dir); }

POCO_RENDERING_BINDER_SCALAR(Poco::Int8)
POCO_RENDERING_BINDER_SCALAR(Poco::UInt8)
POCO_RENDERING_BINDER_SCALAR(Poco::Int16)
POCO_RENDERING_BINDER_SCALAR(Poco::UInt16)
POCO_RENDERING_BINDER_SCALAR(Poco::Int32)
POCO_RENDERING_BINDER_SCALAR(Poco::UInt32)
POCO_RENDERING_BINDER_SCALAR(Poco::Int64)
POCO_RENDERING_BINDER_SCALAR(Poco::UInt64)
#ifndef POCO_INT64_IS_LONG
POCO_RENDERING_BINDER_SCALAR(long)
POCO_RENDERING_BINDER_SCALAR(unsigned long)
#endif
POCO_RENDERING_BINDER_SCALAR(bool)
POCO_RENDERING_BINDER_SCALAR(float)
POCO_RENDERING_BINDER_SCALAR(double)
POCO_RENDERING_BINDER_SCALAR(char)
POCO_RENDERING_BINDER_SCALAR(std::string)
POCO_RENDERING_BINDER_SCALAR(UTF16String)
POCO_RENDERING_BINDER_SCALAR(BLOB)
POCO_RENDERING_BINDER_SCALAR(CLOB)
POCO_RENDERING_BINDER_SCALAR(DateTime)
POCO_RENDERING_BINDER_SCALAR(Date)
POCO_RENDERING_BINDER_SCALAR(Time)
POCO_RENDERING_BINDER_SCALAR(UUID)
POCO_RENDERING_BINDER_SCALAR(NullData)

#undef POCO_RENDERING_BINDER_SCALAR


void RenderingBinder::bind(std::size_t pos, const char* const& pVal, Direction dir)
{
	if (dir == PD_OUT)
		_values[pos] = { outSentinel() };
	else
		_values[pos] = { Utility::quoteString(pVal ? pVal : "") };
}


// --- Container bind() overrides ---

#define POCO_RENDERING_BINDER_CONTAINER(T)                                                                  \
	void RenderingBinder::bind(std::size_t pos, const std::vector<T>& val, Direction dir)                   \
	{ recordContainer(pos, val, dir); }                                                                     \
	void RenderingBinder::bind(std::size_t pos, const std::deque<T>& val, Direction dir)                    \
	{ recordContainer(pos, val, dir); }                                                                     \
	void RenderingBinder::bind(std::size_t pos, const std::list<T>& val, Direction dir)                     \
	{ recordContainer(pos, val, dir); }

POCO_RENDERING_BINDER_CONTAINER(Poco::Int8)
POCO_RENDERING_BINDER_CONTAINER(Poco::UInt8)
POCO_RENDERING_BINDER_CONTAINER(Poco::Int16)
POCO_RENDERING_BINDER_CONTAINER(Poco::UInt16)
POCO_RENDERING_BINDER_CONTAINER(Poco::Int32)
POCO_RENDERING_BINDER_CONTAINER(Poco::UInt32)
POCO_RENDERING_BINDER_CONTAINER(Poco::Int64)
POCO_RENDERING_BINDER_CONTAINER(Poco::UInt64)
#ifndef POCO_INT64_IS_LONG
POCO_RENDERING_BINDER_CONTAINER(long)
#endif
POCO_RENDERING_BINDER_CONTAINER(bool)
POCO_RENDERING_BINDER_CONTAINER(float)
POCO_RENDERING_BINDER_CONTAINER(double)
POCO_RENDERING_BINDER_CONTAINER(char)
POCO_RENDERING_BINDER_CONTAINER(std::string)
POCO_RENDERING_BINDER_CONTAINER(UTF16String)
POCO_RENDERING_BINDER_CONTAINER(BLOB)
POCO_RENDERING_BINDER_CONTAINER(CLOB)
POCO_RENDERING_BINDER_CONTAINER(DateTime)
POCO_RENDERING_BINDER_CONTAINER(Date)
POCO_RENDERING_BINDER_CONTAINER(Time)
POCO_RENDERING_BINDER_CONTAINER(UUID)
POCO_RENDERING_BINDER_CONTAINER(NullData)

#undef POCO_RENDERING_BINDER_CONTAINER


// --- Capture readout ---

const std::vector<std::string>& RenderingBinder::renderedAt(std::size_t pos) const
{
	auto it = _values.find(pos);
	if (it == _values.end())
		throw Poco::NotFoundException("RenderingBinder: position not bound");
	return it->second;
}


bool RenderingBinder::hasPosition(std::size_t pos) const
{
	return _values.find(pos) != _values.end();
}


} } // namespace Poco::Data
