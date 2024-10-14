//
// VarVisitor.h
//
// Library: Foundation
// Package: Dynamic
// Module:  VarVisitor
//
// Definition of the VarVisitor class.
//
// Copyright (c) 2023, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_VarVisitor_INCLUDED
#define Foundation_VarVisitor_INCLUDED


#include "Poco/Dynamic/Var.h"
#include <unordered_map>
#include <functional>


namespace Poco {
namespace Details {


#ifndef POCO_DOC


struct TypeInfoHash
{
	inline std::size_t operator()(std::type_info const& t) const { return t.hash_code(); }
};


struct EqualRef
{
	template <typename T>
	bool operator()(std::reference_wrapper<T> a, std::reference_wrapper<T> b) const
	{
		return a.get() == b.get();
	}
};


using TypeInfoRef = std::reference_wrapper<std::type_info const>;
using HandlerCaller = std::function<void(const Poco::Dynamic::Var&)>;


template <typename T>
using HandlerPointer = void (*)(const T&);


template <typename T>
using Handler = std::function<void(const T&)>;


#endif // POCO_DOC


} // Details


namespace Dynamic {


class Foundation_API Visitor
	/// VarVisitor class.
{
	std::unordered_map<Details::TypeInfoRef,
			Details::HandlerCaller,
			Details::TypeInfoHash,
			Details::EqualRef> _handlers;
	
public:
	template <typename T>
	bool addHandler(const Details::Handler<T>& f)
		/// Add handler for specific type T which holds in Var.
		/// This method is more safe, because it saves copy of handler : lambda or std::function.
		/// Returns true if handler was added.
	{
		auto result = _handlers.emplace(std::ref(typeid(T)),
			Details::HandlerCaller([handler = f](const Poco::Dynamic::Var& x)
			{
				handler(x.extract<T>());
			}));
		return result.second;
	}
	
	template <typename T>
	bool addHandler(Details::HandlerPointer<T> f)
		/// Add handler for specific type T which holds in Var.
		/// This method is less safe, because it saves only copy of function pointer.
		/// Returns true if handler was added.
	{
		auto result = _handlers.emplace(std::ref(typeid(T)),
			Details::HandlerCaller([handlerPointer = f](const Poco::Dynamic::Var& x)
			{
				handlerPointer(x.extract<T>());
			}));
		return result.second;
	}
	
	bool visit(const Poco::Dynamic::Var& x) const;
		/// Find handler for held type and if it exists call handler.
		/// Returns true if hanlder was found othrewise returns false.
};


} } // namespace Poco::Dynamic


#endif // Foundation_VarVisitor_INCLUDED
