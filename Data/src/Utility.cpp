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
#include "Poco/LocalDateTime.h"
#include "Poco/NumberFormatter.h"
#include <cstdio>
#include <string>


namespace Poco::Data {


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
