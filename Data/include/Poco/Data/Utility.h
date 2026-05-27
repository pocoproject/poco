//
// Utility.h
//
// Library: Data
// Package: DataCore
// Module:  Utility
//
// Definition of the Utility class.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Data_Utility_INCLUDED
#define Data_Utility_INCLUDED


#include "Poco/Data/Data.h"
#include "Poco/Data/Session.h"
#include "Poco/Data/Statement.h"
#include "Poco/Data/Binding.h"
#include "Poco/Data/DataException.h"
#include "Poco/Exception.h"
#include "Poco/Nullable.h"
#include "Poco/NumberFormatter.h"
#include "Poco/Types.h"
#include <array>
#include <chrono>
#include <cstddef>
#include <exception>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>


namespace Poco {

class DateTime;
class LocalDateTime;

namespace Dynamic { class Var; }

} // namespace Poco


namespace Poco::Data {


class Date;
class Time;

template <typename T> class LOB;
using BLOB = LOB<unsigned char>;
using CLOB = LOB<char>;


namespace UtilityDetail
{
	template <typename> struct IsPocoNullable: std::false_type {};
	template <typename U> struct IsPocoNullable<Poco::Nullable<U>>: std::true_type {};

	template <typename> struct IsStdOptional: std::false_type {};
	template <typename U> struct IsStdOptional<std::optional<U>>: std::true_type {};
}


class Data_API Utility
	/// General-purpose Poco::Data helpers — SQL rendering for diagnostics
	/// and a templated SQL execution wrapper that reports the outcome via
	/// a structured callback.
{
public:
	struct ExecResult
		/// Outcome of Utility::executeSQL, handed to the callback.
	{
		std::string  sql;                  /// Bound SQL (placeholders substituted).
		bool         ok = false;
		std::size_t  rowsAffected = 0;
		std::string  error;                /// Populated iff !ok.
		std::int64_t tsMicros = 0;         /// Wall-clock at execute start.
		std::int64_t executionMicros = 0;  /// stmt.execute() duration in microseconds.
		std::string  context;              /// Empty iff caller passed nullptr.
	};

	using Callback = std::function<void(const ExecResult&)>;

	static std::string toJSON(const ExecResult& r);
		/// Serialises an ExecResult as a JSON object.
		/// Field order: sql, ok, tsMicros, then rowsAffected (when ok) or
		/// error (when !ok), then context (when non-empty). String values
		/// are escaped per RFC 8259; control characters become `\uXXXX`.

	template <typename T>
	static std::string renderValue(const T& v)
		/// Renders a single C++ value as a SQL literal.
		/// Handled directly: std::nullptr_t ("NULL"); Poco::Nullable and
		/// std::optional (NULL or recurse on the inner value); bool ("1"/"0");
		/// integral types via Poco::NumberFormatter; floating-point via the
		/// shortest round-trip Poco::NumberFormatter; Poco::Data::Date,
		/// Poco::Data::Time, Poco::DateTime, Poco::LocalDateTime as quoted
		/// SQL literals; Poco::Data::BLOB as X'…' hex; Poco::Data::CLOB as a
		/// quoted text literal; Poco::Dynamic::Var inspected and dispatched;
		/// std::string / const char* / string-view single-quoted with internal
		/// ' doubled.
		/// Any other type returns "?" so the diagnostic SQL stays valid-shaped
		/// (the actual statement still binds the value via its TypeHandler).
	{
		if constexpr (std::is_same_v<std::remove_cv_t<T>, std::nullptr_t>)
		{
			(void)v;
			return "NULL";
		}
		else if constexpr (UtilityDetail::IsPocoNullable<std::remove_cv_t<T>>::value)
		{
			return v.isNull() ? std::string("NULL") : renderValue(v.value());
		}
		else if constexpr (UtilityDetail::IsStdOptional<std::remove_cv_t<T>>::value)
		{
			return v.has_value() ? renderValue(*v) : std::string("NULL");
		}
		else if constexpr (std::is_same_v<std::remove_cv_t<T>, bool>)
		{
			return v ? "1" : "0";
		}
		else if constexpr (std::is_integral_v<T>)
		{
			return Poco::NumberFormatter::format(v);
		}
		else if constexpr (std::is_floating_point_v<T>)
		{
			return Poco::NumberFormatter::format(v);
		}
		else if constexpr (std::is_same_v<std::remove_cv_t<T>, Date>)
		{
			return formatDate(v);
		}
		else if constexpr (std::is_same_v<std::remove_cv_t<T>, Time>)
		{
			return formatTime(v);
		}
		else if constexpr (std::is_same_v<std::remove_cv_t<T>, Poco::DateTime>)
		{
			return formatDateTime(v);
		}
		else if constexpr (std::is_same_v<std::remove_cv_t<T>, Poco::LocalDateTime>)
		{
			return formatLocalDateTime(v);
		}
		else if constexpr (std::is_same_v<std::remove_cv_t<T>, Poco::Dynamic::Var>)
		{
			return formatVar(v);
		}
		else if constexpr (std::is_same_v<std::remove_cv_t<T>, BLOB>)
		{
			return formatBLOB(v);
		}
		else if constexpr (std::is_same_v<std::remove_cv_t<T>, CLOB>)
		{
			return formatCLOB(v);
		}
		else if constexpr (std::is_convertible_v<const T&, std::string_view>)
		{
			return quoteString(std::string_view(v));
		}
		else
		{
			(void)v;
			return "?";
		}
	}

	template <typename... Args>
	static std::string boundSQL(const std::string& sql, const Args&... bindings)
		/// Renders a parameterised SQL template by substituting placeholders
		/// with stringified bindings. Supports three placeholder styles:
		///   * `?`     — positional; one binding consumed per occurrence.
		///   * `$N`    — numbered; `$N` substitutes bindings[N-1] and may repeat.
		///   * `:name` — named; left verbatim (pass-through, no substitution).
		/// Placeholder positions are obtained from the SQL parser, so literals
		/// and comments are handled by the parser's lexer rather than a local
		/// scanner; placeholders inside single-quoted strings are inert.
		/// Mixing `?` and `$N` in one statement throws
		/// Poco::InvalidArgumentException.
		/// Arity:
		///   * `?`-style — count of `?` must equal sizeof...(Args).
		///   * `$N`-style — the maximum N referenced must equal sizeof...(Args);
		///     `$0` is rejected by the parser.
		/// Arity mismatches throw Poco::InvalidArgumentException.
		/// When the parser is disabled (POCO_DATA_NO_SQL_PARSER), falls back to
		/// a hand-rolled scanner that supports `?` and `$N` only.
	{
		constexpr std::size_t N = sizeof...(Args);
		if constexpr (N == 0)
			return boundSQLImpl(sql, nullptr, 0);
		const std::array<std::string, N> values = { renderValue(bindings)... };
		return boundSQLImpl(sql, values.data(), N);
	}

	template <typename... Args>
	static std::size_t executeSQL(Session& session,
		const std::string& sql,
		std::unique_ptr<std::string> context,
		Callback callback,
		const Args&... bindings)
		/// Executes sql against session with the given bindings.
		/// Returns the number of affected rows (0 on failure).
		/// If callback is set, invokes it with an ExecResult describing the
		/// outcome. The context unique_ptr is moved into ExecResult::context
		/// when non-null; otherwise ExecResult::context stays empty.
		/// If callback is empty, no ExecResult is constructed.
	{
		const auto tsMicros = std::chrono::duration_cast<std::chrono::microseconds>(
			std::chrono::system_clock::now().time_since_epoch()).count();

		std::string rendered;
		try
		{
			rendered = boundSQL(sql, bindings...);
		}
		catch (const Poco::Exception&)
		{
			rendered = sql;
		}

		bool ok = false;
		std::size_t rowsAffected = 0;
		std::string error;
		std::int64_t executionMicros = 0;

		try
		{
			Statement stmt(session);
			stmt << sql;
			((stmt, Keywords::bind(bindings)), ...);
			const auto t0 = std::chrono::steady_clock::now();
			rowsAffected = stmt.execute();
			const auto t1 = std::chrono::steady_clock::now();
			executionMicros = std::chrono::duration_cast<std::chrono::microseconds>(
				t1 - t0).count();
			ok = true;
		}
		catch (const Poco::Data::DataException& e)
		{
			error = e.displayText();
		}
		catch (const Poco::Exception& e)
		{
			error = e.displayText();
		}
		catch (const std::exception& e)
		{
			error = e.what();
		}
		catch (...)
		{
			error = "unknown exception";
		}

		if (callback)
		{
			ExecResult r;
			r.sql             = std::move(rendered);
			r.ok              = ok;
			r.rowsAffected    = ok ? rowsAffected : 0;
			r.error           = std::move(error);
			r.tsMicros        = static_cast<std::int64_t>(tsMicros);
			r.executionMicros = executionMicros;
			if (context) r.context = std::move(*context);
			callback(r);
		}

		return ok ? rowsAffected : 0;
	}

private:
	static std::string boundSQLImpl(const std::string& sql, const std::string* values, std::size_t n);
		/// Non-template back-end for boundSQL: takes pre-rendered binding strings
		/// and performs the placeholder substitution. Lives in Utility.cpp so
		/// that the SQL parser headers do not appear on the public include
		/// surface.

	static std::string quoteString(std::string_view s);
		/// SQL-quotes a string literal: surrounds with ' and doubles internal '.

	static std::string escapeJSON(std::string_view s);
		/// Escapes a string for inclusion as a JSON string value.

	static std::string formatDate(const Date& d);
		/// Renders Poco::Data::Date as the SQL literal 'YYYY-MM-DD'.

	static std::string formatTime(const Time& t);
		/// Renders Poco::Data::Time as the SQL literal 'HH:MM:SS'.

	static std::string formatDateTime(const Poco::DateTime& dt);
		/// Renders Poco::DateTime as 'YYYY-MM-DD HH:MM:SS' (UTC, sortable).

	static std::string formatLocalDateTime(const Poco::LocalDateTime& ldt);
		/// Renders Poco::LocalDateTime as 'YYYY-MM-DD HH:MM:SS' in local time.

	static std::string formatVar(const Poco::Dynamic::Var& v);
		/// Inspects a Poco::Dynamic::Var and renders it as a SQL literal,
		/// recursively dispatching to the appropriate branch (NULL for
		/// empty; booleans, integers, floats; strings via quoteString;
		/// DateTime / LocalDateTime / Date / Time via the corresponding
		/// helpers; otherwise stringified via Var::convert<std::string>
		/// and quoted).

	static std::string formatBLOB(const BLOB& b);
		/// Renders Poco::Data::BLOB as the SQL hex literal X'AABB…'.

	static std::string formatCLOB(const CLOB& c);
		/// Renders Poco::Data::CLOB as a quoted SQL text literal.
};


} // namespace Poco::Data


#endif // Data_Utility_INCLUDED
