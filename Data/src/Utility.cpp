//
// Utility.cpp
//
// Library: Data
// Package: DataCore
// Module:  Utility
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Data/Utility.h"
#include "Poco/Data/Date.h"
#include "Poco/Data/LOB.h"
#include "Poco/Data/Time.h"
#include "Poco/DateTime.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/Dynamic/Var.h"
#include "Poco/Exception.h"
#include "Poco/LocalDateTime.h"
#include "Poco/NumberFormatter.h"
#ifndef POCO_DATA_NO_SQL_PARSER
#include "SQLParser.h"
#include "sql/Expr.h"
#endif
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>


namespace Poco::Data {


namespace
{
	// Pulls the rendered value at (pos, row) from the binder.
	// Scalars have only row 0; containers replicate the last entry once truncated.
	std::string captureAt(const RenderingBinder& rb, std::size_t pos, std::size_t row)
	{
		if (!rb.hasPosition(pos))
			throw Poco::InvalidArgumentException("Utility::boundSQL: too few arguments");
		const std::vector<std::string>& v = rb.renderedAt(pos);
		if (v.empty())
			return "?";
		if (row < v.size())
			return v[row];
		return v.back();
	}

	// Counts how many distinct positions the binder captured.
	std::size_t totalBoundPositions(const RenderingBinder& rb)
	{
		std::size_t count = 0;
		for (std::size_t i = 0; rb.hasPosition(i); ++i) ++count;
		return count;
	}

#ifndef POCO_DATA_NO_SQL_PARSER
	// Renders a single row by substituting placeholders against rb at this row index.
	std::string renderRow(const std::string& sql, const RenderingBinder& rb, std::size_t row,
		std::size_t totalPositions)
	{
		hsql::SQLParserResult result;
		hsql::SQLParser::parse(sql, &result);
		if (!result.isValid())
			throw Poco::InvalidArgumentException(std::string("Utility::boundSQL: ") +
				(result.errorMsg() ? result.errorMsg() : "parse failed"));

		std::vector<const hsql::Expr*> phs(result.parameters().begin(), result.parameters().end());
		std::sort(phs.begin(), phs.end(),
			[](const hsql::Expr* a, const hsql::Expr* b) { return a->ival2 < b->ival2; });

		enum class Style { None, Question, Dollar };
		Style style = Style::None;
		std::size_t questionCount = 0;
		std::size_t maxN = 0;

		std::string out;
		out.reserve(sql.size() + totalPositions * 8);
		std::size_t cursor = 0;

		for (const hsql::Expr* p: phs)
		{
			const std::size_t start = static_cast<std::size_t>(p->ival2);
			out.append(sql, cursor, start - cursor);

			switch (p->type)
			{
			case hsql::kExprParameter:
				if (style == Style::Dollar)
					throw Poco::InvalidArgumentException("Utility::boundSQL: mixed placeholder styles");
				style = Style::Question;
				if (questionCount >= totalPositions)
					throw Poco::InvalidArgumentException("Utility::boundSQL: too few arguments for ? placeholders");
				out.append(captureAt(rb, questionCount++, row));
				cursor = start + 1;
				break;

			case hsql::kExprParameterDollar:
			{
				if (style == Style::Question)
					throw Poco::InvalidArgumentException("Utility::boundSQL: mixed placeholder styles");
				style = Style::Dollar;
				const std::size_t n = static_cast<std::size_t>(p->ival);
				if (n > totalPositions)
					throw Poco::InvalidArgumentException("Utility::boundSQL: $N out of range");
				if (n > maxN) maxN = n;
				out.append(captureAt(rb, n - 1, row));
				std::size_t len = 1;
				for (auto v = p->ival; v > 0; v /= 10) ++len;
				cursor = start + len;
				break;
			}

			case hsql::kExprParameterNamed:
			{
				const std::size_t len = 1 + std::strlen(p->name);
				out.append(sql, start, len);
				cursor = start + len;
				break;
			}

			default:
				break;
			}
		}

		out.append(sql, cursor, std::string::npos);

		if (style == Style::Question && questionCount != totalPositions)
			throw Poco::InvalidArgumentException("Utility::boundSQL: too many arguments for ? placeholders");
		if (style == Style::Dollar && maxN != totalPositions)
			throw Poco::InvalidArgumentException("Utility::boundSQL: $N arity mismatch");

		return out;
	}
#else
	// Hand-rolled scanner fallback for POCO_DATA_NO_SQL_PARSER builds.
	std::string renderRowScanner(const std::string& sql, const RenderingBinder& rb, std::size_t row,
		std::size_t totalPositions)
	{
		std::string out;
		out.reserve(sql.size() + totalPositions * 8);

		enum class Style { None, Question, Dollar };
		Style style = Style::None;
		bool inLit = false;
		std::size_t qi = 0;
		std::size_t maxN = 0;

		std::size_t i = 0;
		while (i < sql.size())
		{
			const char c = sql[i];
			if (inLit)
			{
				out.push_back(c);
				if (c == '\'')
				{
					if (i + 1 < sql.size() && sql[i + 1] == '\'')
					{
						out.push_back('\'');
						i += 2;
						continue;
					}
					inLit = false;
				}
				++i;
			}
			else if (c == '\'')
			{
				inLit = true;
				out.push_back(c);
				++i;
			}
			else if (c == '?')
			{
				if (style == Style::Dollar)
					throw Poco::InvalidArgumentException("Utility::boundSQL: mixed placeholder styles");
				style = Style::Question;
				if (qi >= totalPositions)
					throw Poco::InvalidArgumentException("Utility::boundSQL: too few arguments for ? placeholders");
				out.append(captureAt(rb, qi, row));
				++qi;
				++i;
			}
			else if (c == '$' && i + 1 < sql.size() && std::isdigit(static_cast<unsigned char>(sql[i + 1])))
			{
				if (style == Style::Question)
					throw Poco::InvalidArgumentException("Utility::boundSQL: mixed placeholder styles");
				style = Style::Dollar;
				std::size_t j = i + 1;
				std::size_t n = 0;
				while (j < sql.size() && std::isdigit(static_cast<unsigned char>(sql[j])))
				{
					n = n * 10 + static_cast<std::size_t>(sql[j] - '0');
					++j;
				}
				if (n == 0)
					throw Poco::InvalidArgumentException("Utility::boundSQL: $0 is invalid");
				if (n > totalPositions)
					throw Poco::InvalidArgumentException("Utility::boundSQL: $N out of range");
				if (n > maxN) maxN = n;
				out.append(captureAt(rb, n - 1, row));
				i = j;
			}
			else
			{
				out.push_back(c);
				++i;
			}
		}

		if (style == Style::Question && qi != totalPositions)
			throw Poco::InvalidArgumentException("Utility::boundSQL: too many arguments for ? placeholders");
		if (style == Style::Dollar && maxN != totalPositions)
			throw Poco::InvalidArgumentException("Utility::boundSQL: $N arity mismatch");

		return out;
	}
#endif
}


std::string Utility::boundSQLImpl(const std::string& sql, const RenderingBinder& rb)
{
	const std::size_t totalPositions = totalBoundPositions(rb);
	const std::size_t totalRows = rb.totalRows();
	const std::size_t cap = rb.maxRows();
	const std::size_t toRender = std::min(totalRows, cap);

	std::string out;
	for (std::size_t row = 0; row < toRender; ++row)
	{
		if (row > 0) out.append(";\n");
#ifndef POCO_DATA_NO_SQL_PARSER
		out.append(renderRow(sql, rb, row, totalPositions));
#else
		out.append(renderRowScanner(sql, rb, row, totalPositions));
#endif
	}

	if (totalRows > cap)
	{
		const std::size_t remaining = totalRows - cap;
		out.append("\n-- (+");
		out.append(Poco::NumberFormatter::format(remaining));
		out.append(remaining == 1 ? " more row)" : " more rows)");
	}

	return out;
}


std::string Utility::quoteString(std::string_view s)
{
	std::string out;
	out.reserve(s.size() + 2);
	out.push_back('\'');
	for (char c: s)
	{
		if (c == '\'') out.push_back('\'');
		out.push_back(c);
	}
	out.push_back('\'');
	return out;
}


std::string Utility::escapeJSON(std::string_view s)
{
	std::string out;
	out.reserve(s.size());
	for (char c: s)
	{
		switch (c)
		{
		case '"':  out.append("\\\""); break;
		case '\\': out.append("\\\\"); break;
		case '\b': out.append("\\b");  break;
		case '\f': out.append("\\f");  break;
		case '\n': out.append("\\n");  break;
		case '\r': out.append("\\r");  break;
		case '\t': out.append("\\t");  break;
		default:
			if (static_cast<unsigned char>(c) < 0x20)
			{
				char buf[8];
				std::snprintf(buf, sizeof(buf), "\\u%04x",
					static_cast<unsigned>(static_cast<unsigned char>(c)));
				out.append(buf);
			}
			else
			{
				out.push_back(c);
			}
		}
	}
	return out;
}


std::string Utility::formatDate(const Date& d)
{
	char buf[16];
	std::snprintf(buf, sizeof(buf), "'%04d-%02d-%02d'", d.year(), d.month(), d.day());
	return buf;
}


std::string Utility::formatTime(const Time& t)
{
	char buf[16];
	std::snprintf(buf, sizeof(buf), "'%02d:%02d:%02d'", t.hour(), t.minute(), t.second());
	return buf;
}


std::string Utility::formatDateTime(const Poco::DateTime& dt)
{
	std::string out;
	out.reserve(21);
	out.push_back('\'');
	out.append(Poco::DateTimeFormatter::format(dt, Poco::DateTimeFormat::SORTABLE_FORMAT));
	out.push_back('\'');
	return out;
}


std::string Utility::formatLocalDateTime(const Poco::LocalDateTime& ldt)
{
	std::string out;
	out.reserve(21);
	out.push_back('\'');
	out.append(Poco::DateTimeFormatter::format(ldt, Poco::DateTimeFormat::SORTABLE_FORMAT));
	out.push_back('\'');
	return out;
}


std::string Utility::formatVar(const Poco::Dynamic::Var& v)
{
	if (v.isEmpty())   return "NULL";
	if (v.isBoolean()) return v.convert<bool>() ? "1" : "0";
	if (v.isInteger()) return Poco::NumberFormatter::format(v.convert<Poco::Int64>());
	if (v.isNumeric()) return Poco::NumberFormatter::format(v.convert<double>());

	const auto& ti = v.type();
	if (ti == typeid(Poco::DateTime))      return formatDateTime(v.extract<Poco::DateTime>());
	if (ti == typeid(Poco::LocalDateTime)) return formatLocalDateTime(v.extract<Poco::LocalDateTime>());
	if (ti == typeid(Date))                return formatDate(v.extract<Date>());
	if (ti == typeid(Time))                return formatTime(v.extract<Time>());
	if (ti == typeid(BLOB))                return formatBLOB(v.extract<BLOB>());
	if (ti == typeid(CLOB))                return formatCLOB(v.extract<CLOB>());

	if (v.isString()) return quoteString(v.convert<std::string>());

	return "?";
}


std::string Utility::formatBLOB(const BLOB& b)
{
	static const char hex[] = "0123456789ABCDEF";
	std::string out;
	out.reserve(b.size() * 2 + 3);
	out.append("X'");
	const unsigned char* p = b.rawContent();
	const std::size_t n = b.size();
	for (std::size_t i = 0; i < n; ++i)
	{
		out.push_back(hex[p[i] >> 4]);
		out.push_back(hex[p[i] & 0x0F]);
	}
	out.push_back('\'');
	return out;
}


std::string Utility::formatCLOB(const CLOB& c)
{
	return quoteString(std::string_view(c.rawContent(), c.size()));
}


std::string Utility::toJSON(const ExecResult& r)
{
	std::string out;
	out.reserve(64 + r.sql.size() + r.error.size() + r.context.size());
	out.append("{\"sql\":\"");
	out.append(escapeJSON(r.sql));
	out.append("\",\"ok\":");
	out.append(r.ok ? "true" : "false");
	out.append(",\"tsMicros\":");
	out.append(Poco::NumberFormatter::format(static_cast<Poco::Int64>(r.tsMicros)));
	out.append(",\"executionMicros\":");
	out.append(Poco::NumberFormatter::format(static_cast<Poco::Int64>(r.executionMicros)));
	if (r.ok)
	{
		out.append(",\"rowsAffected\":");
		out.append(Poco::NumberFormatter::format(static_cast<Poco::UInt64>(r.rowsAffected)));
	}
	else
	{
		out.append(",\"error\":\"");
		out.append(escapeJSON(r.error));
		out.push_back('"');
	}
	if (!r.context.empty())
	{
		out.append(",\"context\":\"");
		out.append(escapeJSON(r.context));
		out.push_back('"');
	}
	out.push_back('}');
	return out;
}


} // namespace Poco::Data
