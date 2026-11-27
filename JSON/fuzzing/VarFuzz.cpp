//
// VarFuzz.cpp
//
// Feeds a plain JSON document to Dynamic::Var conversion, typed Object and
// Array accessors, and JSON::Query paths built from the parsed tree.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Dynamic/Struct.h"
#include "Poco/Dynamic/Var.h"
#include "Poco/Exception.h"
#include "Poco/JSON/Array.h"
#include "Poco/JSON/JSONException.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Parser.h"
#include "Poco/JSON/Query.h"
#include "Poco/Timestamp.h"
#include "Poco/Types.h"
#include "Poco/UUID.h"
#include <cstdint>
#include <cstddef>
#include <typeinfo>
#include <string>
#include <vector>


using Poco::Dynamic::Var;
using Poco::DynamicStruct;
using Poco::JSON::Array;
using Poco::JSON::Object;
using Poco::JSON::Parser;
using Poco::JSON::Query;
using Poco::Timestamp;
using Poco::UUID;


namespace
{
	constexpr std::size_t MAX_INPUT_SIZE = 64 * 1024;
	constexpr std::size_t MAX_NODES = 1024;
	constexpr std::size_t MAX_PATHS = 64;
	constexpr std::size_t MAX_PATH_LENGTH = 1024;


	template <typename F>
	void attemptConvert(F f)
		/// Runs f and passes over what conversion and typed accessors say they
		/// throw for incompatible or out-of-range values. Anything else is a
		/// finding.
	{
		try
		{
			f();
		}
		catch (const Poco::BadCastException&)
		{
		}
		catch (const Poco::RangeException&)
		{
		}
		catch (const Poco::SyntaxException&)
		{
		}
		catch (const Poco::InvalidAccessException&)
		{
		}
		catch (const Poco::NotImplementedException&)
		{
		}
	}


	template <typename F>
	void attemptQuery(F f)
		/// Runs f and passes over SyntaxException from Query path evaluation.
		/// Anything else is a finding.
	{
		try
		{
			f();
		}
		catch (const Poco::SyntaxException&)
		{
		}
	}


	void convertMatrix(const Var& var)
	{
		attemptConvert([&var] { (void) var.convert<bool>(); });
		attemptConvert([&var] { (void) var.convert<char>(); });
		attemptConvert([&var] { (void) var.convert<signed char>(); });
		attemptConvert([&var] { (void) var.convert<unsigned char>(); });
		attemptConvert([&var] { (void) var.convert<short>(); });
		attemptConvert([&var] { (void) var.convert<unsigned short>(); });
		attemptConvert([&var] { (void) var.convert<int>(); });
		attemptConvert([&var] { (void) var.convert<unsigned int>(); });
		attemptConvert([&var] { (void) var.convert<long>(); });
		attemptConvert([&var] { (void) var.convert<unsigned long>(); });
		attemptConvert([&var] { (void) var.convert<long long>(); });
		attemptConvert([&var] { (void) var.convert<unsigned long long>(); });
		attemptConvert([&var] { (void) var.convert<float>(); });
		attemptConvert([&var] { (void) var.convert<double>(); });
		attemptConvert([&var] { (void) var.convert<std::string>(); });
		attemptConvert([&var] { (void) var.convert<Timestamp>(); });
		attemptConvert([&var] { (void) var.convert<UUID>(); });
	}


	void convertObjectStructs(const Object::Ptr& obj)
		/// The three struct conversions each walk the whole subtree, so
		/// running them per visited object is quadratic; they belong once
		/// per document, on the root object or on the objects that are
		/// direct elements of a root array.
	{
		if (obj.isNull()) return;

		attemptConvert([&] { (void) Object::makeStruct(obj); });
		attemptConvert([&] { (void) Object::makeOrderedStruct(obj); });
		attemptConvert([&] { (void) static_cast<const DynamicStruct&>(*obj); });
	}


	void typedObject(const Object::Ptr& obj)
	{
		if (obj.isNull()) return;

		std::vector<std::string> names;
		obj->getNames(names);
		for (const auto& name : names)
		{
			attemptConvert([&] { (void) obj->getValue<std::string>(name); });
			attemptConvert([&] { (void) obj->getValue<Poco::Int64>(name); });
			attemptConvert([&] { (void) obj->getValue<double>(name); });
			attemptConvert([&] { (void) obj->getValue<bool>(name); });
			attemptConvert([&] { (void) obj->getObject(name); });
			attemptConvert([&] { (void) obj->getArray(name); });
			attemptConvert([&] { (void) obj->has(name); });
			attemptConvert([&] { (void) obj->isArray(name); });
			attemptConvert([&] { (void) obj->isObject(name); });
			attemptConvert([&] { (void) obj->isNull(name); });
		}
	}


	void typedArray(const Array::Ptr& arr)
	{
		if (arr.isNull()) return;

		const std::size_t n = arr->size();
		for (std::size_t i = 0; i < n; ++i)
		{
			attemptConvert([&] { (void) arr->getElement<std::string>(static_cast<unsigned>(i)); });
			attemptConvert([&] { (void) arr->getElement<Poco::Int64>(static_cast<unsigned>(i)); });
			attemptConvert([&] { (void) arr->getElement<double>(static_cast<unsigned>(i)); });
			attemptConvert([&] { (void) arr->getElement<bool>(static_cast<unsigned>(i)); });
			attemptConvert([&] { (void) arr->getObject(static_cast<unsigned>(i)); });
			attemptConvert([&] { (void) arr->getArray(static_cast<unsigned>(i)); });
			attemptConvert([&] { (void) arr->get(static_cast<unsigned>(i)); });
		}
	}


	void visit(const Var& var, std::size_t& nodes, std::vector<std::string>& paths, const std::string& path)
	{
		if (nodes >= MAX_NODES) return;
		++nodes;

		convertMatrix(var);

		if (var.type() == typeid(Object::Ptr))
		{
			Object::Ptr obj = var.extract<Object::Ptr>();
			typedObject(obj);
			if (obj.isNull()) return;

			std::vector<std::string> names;
			obj->getNames(names);
			for (const auto& name : names)
			{
				const std::string child = path.empty() ? name : path + "." + name;
				if (paths.size() < MAX_PATHS && child.size() <= MAX_PATH_LENGTH)
					paths.push_back(child);
				visit(obj->get(name), nodes, paths, child);
			}
		}
		else if (var.type() == typeid(Array::Ptr))
		{
			Array::Ptr arr = var.extract<Array::Ptr>();
			typedArray(arr);
			if (arr.isNull()) return;

			const std::size_t n = arr->size();
			for (std::size_t i = 0; i < n; ++i)
			{
				const std::string child = path + "[" + std::to_string(i) + "]";
				if (paths.size() < MAX_PATHS && child.size() <= MAX_PATH_LENGTH)
					paths.push_back(child);
				visit(arr->get(static_cast<unsigned>(i)), nodes, paths, child);
			}
		}
	}


	void runQueries(const Var& var, const std::vector<std::string>& paths)
	{
		Query q(var);
		for (const auto& path : paths)
		{
			attemptQuery([&] { (void) q.find(path); });
			attemptQuery([&] { (void) q.findObject(path); });
			attemptQuery([&] { (void) q.findArray(path); });
		}
	}
}


extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
	if (size > MAX_INPUT_SIZE) return 0;

	const std::string input(reinterpret_cast<const char*>(data), size);

	Var var;
	try
	{
		Parser parser;
		var = parser.parse(input);
	}
	catch (const Poco::JSON::JSONException&)
	{
		return 0;
	}
	catch (const Poco::SyntaxException&)
	{
		return 0;
	}

	std::size_t nodes = 0;
	std::vector<std::string> paths;
	paths.reserve(MAX_PATHS);
	visit(var, nodes, paths, std::string());

	// The struct conversions each walk the whole subtree: run them once per
	// document, on the root object or on the objects that are direct
	// elements of a root array.
	if (var.type() == typeid(Object::Ptr))
	{
		convertObjectStructs(var.extract<Object::Ptr>());
	}
	else if (var.type() == typeid(Array::Ptr))
	{
		Array::Ptr arr = var.extract<Array::Ptr>();
		if (!arr.isNull())
		{
			const std::size_t count = arr->size();
			for (std::size_t i = 0; i < count; ++i)
			{
				Var element = arr->get(static_cast<unsigned>(i));
				if (element.type() == typeid(Object::Ptr))
					convertObjectStructs(element.extract<Object::Ptr>());
			}
		}
	}

	if (var.type() == typeid(Object::Ptr) || var.type() == typeid(Array::Ptr))
		runQueries(var, paths);
	return 0;
}
