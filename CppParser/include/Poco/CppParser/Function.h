//
// Function.h
//
// Library: CppParser
// Package: SymbolTable
// Module:  Function
//
// Definition of the Function class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef CppParser_Function_INCLUDED
#define CppParser_Function_INCLUDED


#include "Poco/CppParser/CppParser.h"
#include "Poco/CppParser/Decl.h"
#include <vector>


namespace Poco {
namespace CppParser {


class Parameter;


class CppParser_API Function: public Decl
	/// This class represents a (member) function declaration.
{
public:
	enum Flags
	{
		FN_STATIC       = 1,   /// The function is static.
		FN_VIRTUAL      = 2,   /// The function is virtual.
		FN_INLINE       = 4,   /// The function is inline.
		FN_CONST        = 8,   /// The function is const.
		FN_TEMPLATE     = 16,  /// The function is a template.
		FN_PURE_VIRTUAL = 32,  /// The function is pure virtual.
		FN_FINAL        = 64,  /// The function is final.
		FN_OVERRIDE     = 128, /// The function is override.
		FN_NOEXCEPT     = 256, /// The function is noexcept.
		FN_DEFAULT      = 512, /// The function is default.
		FN_DELETE       = 1024 /// The function has been deleted.
	};

	using Parameters = std::vector<Parameter*>;
	using Iterator = Parameters::const_iterator;

	Function(const std::string& decl, NameSpace* pNameSpace);
		/// Creates the Function.

	~Function();
		/// Destroys the Function.

	void addParameter(Parameter* pParam);
		/// Adds a parameter to the function.

	[[nodiscard]]
	const std::string& getReturnParameter() const;

	[[nodiscard]]
	Iterator begin() const;
		/// Returns an iterator for iterating over the Function's Parameter's.

	[[nodiscard]]
	Iterator end() const;
		/// Returns an iterator for iterating over the Function's Parameter's.

	void makeInline();
		/// Sets the FN_INLINE flag.

	void makeConst();
		/// Sets the FN_CONST flag.

	void makePureVirtual();
		/// Sets the FN_PURE_VIRTUAL flag.

	void makeFinal();
		/// Sets the FN_FINAL flag.

	void makeOverride();
		/// Sets the FN_OVERRIDE flag.

	void makeNoexcept();
		/// Sets the FN_NOEXCEPT flag.

	void makeDefault();
		/// Sets the FN_DEFAULT flag.

	void makeDelete();
		/// Sets the FN_DELETE flag.

	[[nodiscard]]
	int flags() const;
		/// Returns the function's flags.

	[[nodiscard]]
	bool isConstructor() const;
		/// Returns true iff the function is a constructor.

	[[nodiscard]]
	bool isDestructor() const;
		/// Returns true iff the function is a destructor.

	[[nodiscard]]
	bool isMethod() const;
		/// Returns true iff the function is a method (it's part of
		/// a Struct and it's neither a constructor nor a destructor).

	[[nodiscard]]
	bool isFunction() const;
		/// Returns true iff the function is not a member of a class
		/// (a freestanding function).

	[[nodiscard]]
	bool isConst() const;
		/// Returns true iff the method is const.

	[[nodiscard]]
	bool isDefault() const;
		/// Returns true iff the method has a default implementation.

	[[nodiscard]]
	bool isDeleted() const;
		/// Returns true iff the method has been deleted.

	[[nodiscard]]
	bool isVirtual() const;
		/// Returns true if the method is virtual. Also examines base
		/// classes to check for a virtual function with the same
		/// signature.

	[[nodiscard]]
	bool isStatic() const;
		/// Returns true iff the method is static.

	[[nodiscard]]
	int countParameters() const;
		/// Returns the number of parameters.

	[[nodiscard]]
	std::string signature() const;
		/// Returns the signature of the function.

	[[nodiscard]]
	Function* getOverridden() const;
		/// If the function is virtual and overrides a function in a
		/// base class, the base class function is returned.
		/// Otherwise, null is returned.

	[[nodiscard]]
	Symbol::Kind kind() const;
	[[nodiscard]]
	std::string toString() const;

private:
	Parameters  _params;
	int         _flags;
	std::string _retParam;
};


//
// inlines
//
inline int Function::flags() const
{
	return _flags;
}


inline const std::string& Function::getReturnParameter() const
{
	return _retParam;
}


inline bool Function::isConst() const
{
	return (flags() & FN_CONST) != 0;
}


inline bool Function::isDefault() const
{
	return (flags() & FN_DEFAULT) != 0;
}


inline bool Function::isDeleted() const
{
	return (flags() & FN_DELETE) != 0;
}


inline bool Function::isStatic() const
{
	return (flags() & FN_STATIC) != 0;
}


} } // namespace Poco::CppParser


#endif // CppParser_Function_INCLUDED
