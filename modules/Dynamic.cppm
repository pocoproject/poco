module;

#ifdef ENABLE_FOUNDATION
#include "Poco/Dynamic/Pair.h"
#include "Poco/Dynamic/Struct.h"
#include "Poco/Dynamic/Var.h"
#include "Poco/Dynamic/VarHolder.h"
#include "Poco/Dynamic/VarIterator.h"
#include "Poco/Dynamic/VarVisitor.h"
#endif

export module Poco.Dynamic;

export namespace Poco::Dynamic {
	#ifdef ENABLE_FOUNDATION
	using Poco::Dynamic::Pair;
	using Poco::Dynamic::Struct;
	using Poco::Dynamic::Var;
	using Poco::Dynamic::VarHolder;
	using Poco::Dynamic::VarHolderImpl;
	using Poco::Dynamic::VarIterator;
	using Poco::Dynamic::Visitor;

	using Poco::Dynamic::Array;
	using Poco::Dynamic::Deque;
	using Poco::Dynamic::List;
	using Poco::Dynamic::Vector;

	using Poco::Dynamic::operator!=;
	using Poco::Dynamic::operator*;
	using Poco::Dynamic::operator*=;
	using Poco::Dynamic::operator+;
	using Poco::Dynamic::operator+=;
	using Poco::Dynamic::operator-;
	using Poco::Dynamic::operator-=;
	using Poco::Dynamic::operator/;
	using Poco::Dynamic::operator/=;
	using Poco::Dynamic::operator<;
	using Poco::Dynamic::operator<=;
	using Poco::Dynamic::operator==;
	using Poco::Dynamic::operator>;
	using Poco::Dynamic::operator>=;
	using Poco::Dynamic::structToString;

	namespace Impl {
		using Poco::Dynamic::Impl::appendJSONKey;
		using Poco::Dynamic::Impl::appendJSONString;
		using Poco::Dynamic::Impl::appendJSONValue;
		using Poco::Dynamic::Impl::containerToJSON;
		using Poco::Dynamic::Impl::isJSONString;
	}
	#endif
}
