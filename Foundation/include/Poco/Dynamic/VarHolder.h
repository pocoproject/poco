//
// VarHolder.h
//
// Library: Foundation
// Package: Dynamic
// Module:  VarHolder
//
// Definition of the VarHolder class.
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_VarHolder_INCLUDED
#define Foundation_VarHolder_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/NumberFormatter.h"
#include "Poco/NumberParser.h"
#include "Poco/DateTime.h"
#include "Poco/Timestamp.h"
#include "Poco/LocalDateTime.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeParser.h"
#include "Poco/String.h"
#include "Poco/UnicodeConverter.h"
#include "Poco/UTFString.h"
#include "Poco/UTF8String.h"
#include "Poco/UUID.h"
#include "Poco/Any.h"
#include "Poco/Format.h"
#include "Poco/Debugger.h"
#include "Poco/Exception.h"
#include <vector>
#include <list>
#include <deque>
#include <typeinfo>
#include <type_traits>
#include <string_view>
#undef min
#undef max
#include <limits>


#define POCO_VAR_RANGE_EXCEPTION(str, from) \
	throw RangeException(Poco::format("%v ((%s/%d) %s > (%s/%d) %s) @ %s.", \
		std::string_view(#str), Poco::demangle<F>(), numValDigits(from), std::to_string(from), \
		Poco::demangle<T>(), numTypeDigits<T>(), std::to_string(static_cast<T>(from)), \
		poco_src_loc))


namespace Poco {
namespace Dynamic {


class Var;


namespace Impl {


bool Foundation_API isJSONString(const Var& any);
	/// Returns true for values that should be JSON-formatted as string.


void Foundation_API appendJSONKey(std::string& val, const Var& any);
	/// Converts the any to a JSON key (i.e. wraps it into double quotes
	/// regardless of the underlying type) and appends it to val.


void Foundation_API appendJSONString(std::string& val, const Var& any);
	/// Converts the any to a JSON string (i.e. wraps it into double quotes)
	/// regardless of the underlying type) and appends it to val.


void Foundation_API appendJSONValue(std::string& val, const Var& any, bool wrap = true);
	/// Converts the any to a JSON value (if underlying type qualifies
	/// as string - see isJSONString() - it is wrapped into double quotes)
	/// and appends it to val.
	/// Wrapping can be prevented (useful for appending JSON fragments) by setting
	/// the wrap argument to false.


template <typename C>
void containerToJSON(C& cont, std::string& val)
{
	// Serialize in JSON format. Note: although this is a vector<T>, the code only
	// supports vector<Var>. Total specialization is not possible
	// because of the cyclic dependency between Var and VarHolder

	// JSON format definition: [ n times: elem ',' ], no ',' for last elem
	val.append("[ ");
	typename C::const_iterator it = cont.begin();
	typename C::const_iterator itEnd = cont.end();
	if (!cont.empty())
	{
		appendJSONValue(val, *it);
		++it;
	}
	for (; it != itEnd; ++it)
	{
		val.append(", ");
		appendJSONValue(val, *it);
	}
	val.append(" ]");
}


} // namespace Impl


class Foundation_API VarHolder
	/// Interface for a data holder used by the Var class.
	/// Provides methods to convert between data types.
	/// Only data types for which VarHolder specialization exists are supported.
	///
	/// Provided are specializations for all C++ built-in types with addition of
	/// std::string, Poco::UTF16String, DateTime, LocalDateTime, Timestamp, std::vector<Var> and DynamicStruct.
	///
	/// Additional types can be supported by adding specializations. When implementing specializations,
	/// the only condition is that they reside in Poco namespace and implement the pure virtual functions
	/// clone() and type().
	///
	/// Those conversions that are not implemented shall fail back to this base
	/// class implementation. All the convert() function overloads in this class
	/// throw BadCastException.
{
public:
	using ArrayValueType = Var;

	virtual ~VarHolder();
		/// Destroys the VarHolder.

	virtual VarHolder* clone(Placeholder<VarHolder>* pHolder = nullptr) const = 0;
		/// Implementation must implement this function to
		/// deep-copy the VarHolder.
		/// If small object optimization is enabled (i.e. if
		/// POCO_NO_SOO is not defined), VarHolder will be
		/// instantiated in-place if it's size is smaller
		/// than POCO_SMALL_OBJECT_SIZE.

	virtual const std::type_info& type() const = 0;
		/// Implementation must return the type information
		/// (typeid) for the stored content.

	virtual void convert(Int8& val) const;
		/// Throws BadCastException. Must be overridden in a type
		/// specialization in order to support the conversion.

	virtual void convert(Int16& val) const;
		/// Throws BadCastException. Must be overridden in a type
		/// specialization in order to support the conversion.

	virtual void convert(Int32& val) const;
		/// Throws BadCastException. Must be overridden in a type
		/// specialization in order to support the conversion.

	virtual void convert(Int64& val) const;
		/// Throws BadCastException. Must be overridden in a type
		/// specialization in order to support the conversion.

	virtual void convert(UInt8& val) const;
		/// Throws BadCastException. Must be overridden in a type
		/// specialization in order to support the conversion.

	virtual void convert(UInt16& val) const;
		/// Throws BadCastException. Must be overridden in a type
		/// specialization in order to support the conversion.

	virtual void convert(UInt32& val) const;
		/// Throws BadCastException. Must be overridden in a type
		/// specialization in order to support the conversion.

	virtual void convert(UInt64& val) const;
		/// Throws BadCastException. Must be overridden in a type
		/// specialization in order to support the conversion.

	virtual void convert(DateTime& val) const;
		/// Throws BadCastException. Must be overridden in a type
		/// specialization in order to support the conversion.

	virtual void convert(LocalDateTime& val) const;
		/// Throws BadCastException. Must be overridden in a type
		/// specialization in order to support the conversion.

	virtual void convert(Timestamp& val) const;
		/// Throws BadCastException. Must be overridden in a type
		/// specialization in order to support the conversion.

	virtual void convert(UUID& val) const;
		/// Throws BadCastException. Must be overridden in a type
		/// specialization in order to support the conversion.

#ifndef POCO_INT64_IS_LONG

	void convert(long& val) const;
		/// Calls convert(Int32).

	void convert(unsigned long& val) const;
		/// Calls convert(UInt32).

#else

	virtual void convert(long long& val) const;
		/// Throws BadCastException. Must be overridden in a type
		/// specialization in order to suport the conversion.

	virtual void convert(unsigned long long & val) const;
		/// Throws BadCastException. Must be overridden in a type
		/// specialization in order to suport the conversion.

#endif

	virtual void convert(bool& val) const;
		/// Throws BadCastException. Must be overridden in a type
		/// specialization in order to support the conversion.

	virtual void convert(float& val) const;
		/// Throws BadCastException. Must be overridden in a type
		/// specialization in order to support the conversion.

	virtual void convert(double& val) const;
		/// Throws BadCastException. Must be overridden in a type
		/// specialization in order to support the conversion.

	virtual void convert(char& val) const;
		/// Throws BadCastException. Must be overridden in a type
		/// specialization in order to support the conversion.

	virtual void convert(std::string& val) const;
		/// Throws BadCastException. Must be overridden in a type
		/// specialization in order to support the conversion.

	virtual void convert(Poco::UTF16String& val) const;
	/// Throws BadCastException. Must be overridden in a type
	/// specialization in order to support the conversion.

	virtual bool isArray() const;
		/// Returns true.

	virtual bool isVector() const;
		/// Returns false. Must be properly overridden in a type
		/// specialization in order to support the diagnostic.

	virtual bool isList() const;
		/// Returns false. Must be properly overridden in a type
		/// specialization in order to support the diagnostic.

	virtual bool isDeque() const;
		/// Returns false. Must be properly overridden in a type
		/// specialization in order to support the diagnostic.

	virtual bool isStruct() const;
		/// Returns false. Must be properly overridden in a type
		/// specialization in order to support the diagnostic.

	virtual bool isOrdered() const;
		/// Returns false. Must be properly overridden in a type
		/// specialization in order to support the diagnostic.

	virtual bool isInteger() const;
		/// Returns false. Must be properly overridden in a type
		/// specialization in order to support the diagnostic.

	virtual bool isSigned() const;
		/// Returns false. Must be properly overridden in a type
		/// specialization in order to support the diagnostic.

	virtual bool isNumeric() const;
		/// Returns false. Must be properly overridden in a type
		/// specialization in order to support the diagnostic.

	virtual bool isBoolean() const;
		/// Returns false. Must be properly overridden in a type
		/// specialization in order to support the diagnostic.

	virtual bool isString() const;
		/// Returns false. Must be properly overridden in a type
		/// specialization in order to support the diagnostic.

	virtual bool isDate() const;
		/// Returns false. Must be properly overridden in a type
		/// specialization in order to support the diagnostic.

	virtual bool isTime() const;
		/// Returns false. Must be properly overridden in a type
		/// specialization in order to support the diagnostic.

	virtual bool isDateTime() const;
		/// Returns false. Must be properly overridden in a type
		/// specialization in order to support the diagnostic.

	virtual bool isUUID() const;
		/// Returns false. Must be properly overridden in a type
		/// specialization in order to support the diagnostic.

	virtual std::size_t size() const;
		/// Returns 1 iff Var is not empty or this function overridden.

protected:
	VarHolder();
		/// Creates the VarHolder.

	template <typename T>
	VarHolder* cloneHolder(Placeholder<VarHolder>* pVarHolder, const T& val) const
		/// Instantiates value holder wrapper.
		///
		/// Called from clone() member function of the implementation.
		///
		/// When the smal object optimization is enabled (POCO_NO_SOO not
		/// defined), if size of the wrapper is larger than
		/// POCO_SMALL_OBJECT_SIZE, holder is instantiated on
		/// the heap, otherwise it is instantiated in-place (in the
		/// pre-allocated buffer inside the holder).
	{
		poco_check_ptr (pVarHolder);
		return pVarHolder->assign<VarHolderImpl<T>, T>(val);
	}

	template <typename F, typename T,
		std::enable_if_t<(std::is_integral_v<F> && std::is_signed_v<F>) ||
			std::is_floating_point_v<F>, F>* = nullptr,
		std::enable_if_t<(std::is_integral_v<T> && std::is_signed_v<T>) ||
			std::is_floating_point_v<F>, T>* = nullptr>
	static void convertToSmaller(const F& from, T& to)
		/// Converts signed integral, as well as floating-point, values from
		/// larger to smaller type. It checks the upper and lower bound and
		/// if from value is within limits of type T (i.e. check calls do not throw),
		/// it is converted.
	{
		if constexpr((std::is_integral_v<F>) && (std::is_floating_point_v<T>))
		{
			if (isPrecisionLost<F, T>(from))
				POCO_VAR_RANGE_EXCEPTION ("Lost precision", from);
		}
		checkUpperLimit<F,T>(from);
		checkLowerLimit<F,T>(from);
		to = static_cast<T>(from);
	}

	template <typename F, typename T,
		std::enable_if_t<std::is_integral_v<F> && std::is_signed_v<T>, F>* = nullptr,
		std::enable_if_t<std::is_floating_point_v<T>, T>* = nullptr>
	static void convertToSmaller(const F& from, T& to)
		/// Converts signed integral values from integral to floating-point type. Checks for
		/// the loss of precision and if from value is within limits of type T (i.e. check calls do not throw),
		/// it is converted.
	{
		if (isPrecisionLost<F, T>(from))
			POCO_VAR_RANGE_EXCEPTION ("Lost precision", from);
		to = static_cast<T>(from);
	}

	template <typename F, typename T,
		std::enable_if_t<std::is_same_v<F, bool>>* = nullptr,
		std::enable_if_t<std::is_floating_point_v<T>, T>* = nullptr>
	static void convertToSmaller(const F& from, T& to)
		/// Converts boolean values to floating-point type.
	{
		to = static_cast<T>(from);
	}

	template <typename F, typename T,
		std::enable_if_t<std::is_integral_v<F> && !std::is_signed_v<F>, F>* = nullptr,
		std::enable_if_t<(std::is_integral_v<T> && !std::is_signed_v<T>) || std::is_floating_point_v<T>, T>* = nullptr>
	static void convertToSmallerUnsigned(const F& from, T& to)
		/// Converts unsigned integral data types from larger to smaller, as well as to floating-point, types.
		/// Since lower limit is always 0 for unsigned types, only the upper limit is checked, thus
		/// saving some cycles compared to the signed version of the function. If the
		/// value to be converted is smaller than the maximum value for the target type,
		/// the conversion is performed.
	{
		checkUpperLimit<F,T>(from);
		to = static_cast<T>(from);
	}

	template <typename F, typename T,
		std::enable_if_t<std::is_integral_v<F> && std::is_signed_v<F>, F>* = nullptr,
		std::enable_if_t<std::is_integral_v<T> && !std::is_signed_v<T>, T>* = nullptr>
	static void convertSignedToUnsigned(const F& from, T& to)
		/// Converts signed integral data types to unsigned data types.
		/// Negative values can not be converted and if one is encountered, RangeException is thrown.
		/// If upper limit is within the target data type limits, the conversion is performed.
	{
		if (from < 0)
			POCO_VAR_RANGE_EXCEPTION ("Value too small", from);
		checkUpperLimit<std::make_unsigned_t<F>,T>(from);
		to = static_cast<T>(from);
	}

	template <typename F, typename T, std::enable_if_t<std::is_floating_point_v<F>, bool> = true,
		std::enable_if_t<std::is_integral_v<T> && !std::is_signed_v<T>, T>* = nullptr>
	static void convertSignedFloatToUnsigned(const F& from, T& to)
		/// Converts floating point data types to unsigned integral data types. Negative values
		/// can not be converted and if one is encountered, RangeException is thrown.
		/// If upper limit is within the target data type limits, the conversion is performed.
	{
		if (from < 0)
			POCO_VAR_RANGE_EXCEPTION ("Value too small", from);
		checkUpperLimit<F,T>(from);
		to = static_cast<T>(from);
	}

	template <typename F, typename T,
		std::enable_if_t<std::is_integral_v<F> && !std::is_signed_v<F>, F>* = nullptr,
		std::enable_if_t<std::is_integral_v<T> && std::is_signed_v<T>, T>* = nullptr>
	static void convertUnsignedToSigned(const F& from, T& to)
		/// Converts unsigned integral data types to signed integral data types.
		/// If upper limit is within the target data type limits, the conversion is performed.
	{
		checkUpperLimit<F,T>(from);
		to = static_cast<T>(from);
	}

	template <typename F, typename T, std::enable_if_t<std::is_signed_v<F> && std::is_signed_v<T> && (sizeof(F) <= sizeof(T))>* = nullptr>
	void convertToSigned(const F& from, T& to) const
	{
		to = static_cast<T>(from);
	}

	template <typename F, typename T, std::enable_if_t<std::is_signed_v<F> && std::is_signed_v<T> && (sizeof(F) > sizeof(T))>* = nullptr>
	void convertToSigned(const F& from, T& to) const
	{
		convertToSmaller(from, to);
	}

	template <typename F, typename T, std::enable_if_t<!std::is_signed_v<F> && std::is_signed_v<T>>* = nullptr>
	void convertToSigned(const F& from, T& to) const
	{
		convertUnsignedToSigned(from, to);
	}

	template <typename F, typename T, std::enable_if_t<!std::is_signed_v<F> && !std::is_signed_v<T> && (sizeof(F) <= sizeof(T))>* = nullptr>
	void convertToUnsigned(const F& from, T& to) const
	{
		to = static_cast<T>(from);
	}

	template <typename F, typename T, std::enable_if_t<!std::is_signed_v<F> && !std::is_signed_v<T> && (sizeof(F) > sizeof(T))>* = nullptr>
	void convertToUnsigned(const F& from, T& to) const
	{
		convertToSmallerUnsigned(from, to);
	}

	template <typename F, typename T, std::enable_if_t<std::is_signed_v<F> && !std::is_signed_v<T>>* = nullptr>
	void convertToUnsigned(const F& from, T& to) const
	{
		convertSignedToUnsigned(from, to);
	}

	template <typename F, typename T,
		std::enable_if_t<std::is_integral_v<F>, bool> = true,
		std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
	static void convertToFP(F& from, T& to)
		/// Converts unsigned integral data types to floating-point data types.
		/// If the number of significant digits used for the integer vaue exceeds the number
		/// of available digits in the floatinng-point destination (ie. if precision would be lost
		/// by casting the value), RangeException is thrown.
	{
		if (isPrecisionLost<F, T>(from))
			POCO_VAR_RANGE_EXCEPTION ("Lost precision", from);
		to = static_cast<T>(from);
	}

private:

	template <typename T>
	static constexpr T unpreserveSign(const T& value)
	{
		if constexpr (std::is_signed_v<T>)
		{
			return (value < 0) ? -value : value;
		}
		else
		{
			return value;
		}
	}

	template <typename T, std::enable_if_t<std::is_same_v<T, bool>, bool> = true>
	static constexpr int numValDigits(const T& value)
	{
		return 1;
	}

	template <typename T, std::enable_if_t<std::is_integral_v<T> && !std::is_same_v<T, bool>, bool> = true>
	static constexpr int numValDigits(const T& value)
	{
		using U = std::make_unsigned_t<T>;
		if (value == 0) return 0;
		int digitCount = 0;
		U locVal = static_cast<U>(unpreserveSign(value)); // to prevent sign preservation
		while (locVal >>= 1) ++digitCount;
		return digitCount;
	}

	template <typename T, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
	static constexpr int numValDigits(T value)
	{
		return numValDigits<int64_t>(static_cast<int64_t>(value));
	}

	template <typename T, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
	static constexpr int numTypeDigits()
	{
		return std::numeric_limits<T>::digits;
	}

	template <typename T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
	static constexpr int numTypeDigits()
	{
		return numValDigits(std::numeric_limits<T>::max());
	}

	template <typename F, typename T,
		std::enable_if_t<std::is_integral_v<F>, bool> = true,
		std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
	static bool isPrecisionLost(const F& from)
		// Checks for loss of precision in integral -> floating point conversion.
	{
		return numValDigits(from) > numTypeDigits<T>();
	}

	template <typename F, typename T, std::enable_if_t<std::is_integral_v<F>, bool> = true>
	static void checkUpperLimit(const F& from)
	{
		if (from > static_cast<F>(std::numeric_limits<T>::max()))
			POCO_VAR_RANGE_EXCEPTION ("Value too big", from);
	}

	template <typename F, typename T, std::enable_if_t<std::is_integral_v<F>, bool> = true>
	static void checkLowerLimit(const F& from)
	{
		if (from < static_cast<F>(std::numeric_limits<T>::min()))
			POCO_VAR_RANGE_EXCEPTION ("Value too small", from);
	}

	template <typename F, typename T, std::enable_if_t<std::is_floating_point_v<F>, bool> = true>
	static void checkUpperLimit(const F& from)
	{
		if ((from > static_cast<F>(std::numeric_limits<T>::max())))
			POCO_VAR_RANGE_EXCEPTION ("Value too big", from);
	}

	template <typename F, typename T, std::enable_if_t<std::is_floating_point_v<F>, bool> = true>
	static void checkLowerLimit(const F& from)
	{
		if constexpr(std::is_floating_point_v<T>)
		{
			if (static_cast<F>(-std::numeric_limits<T>::max()) > from)
				POCO_VAR_RANGE_EXCEPTION ("Value too small", from);
		}
		else if (from < static_cast<F>(std::numeric_limits<T>::min()))
			POCO_VAR_RANGE_EXCEPTION ("Value too small", from);
	}
};


//
// inlines
//


inline void VarHolder::convert(Int8& /*val*/) const
{
	throw BadCastException("Can not convert to Int8");
}


inline void VarHolder::convert(Int16& /*val*/) const
{
	throw BadCastException("Can not convert to Int16");
}


inline void VarHolder::convert(Int32& /*val*/) const
{
	throw BadCastException("Can not convert to Int32");
}


inline void VarHolder::convert(Int64& /*val*/) const
{
	throw BadCastException("Can not convert to Int64");
}


inline void VarHolder::convert(UInt8& /*val*/) const
{
	throw BadCastException("Can not convert to UInt8");
}


inline void VarHolder::convert(UInt16& /*val*/) const
{
	throw BadCastException("Can not convert to UInt16");
}


inline void VarHolder::convert(UInt32& /*val*/) const
{
	throw BadCastException("Can not convert to UInt32");
}


inline void VarHolder::convert(UInt64& /*val*/) const
{
	throw BadCastException("Can not convert to UInt64");
}


inline void VarHolder::convert(DateTime& /*val*/) const
{
	throw BadCastException("Can not convert to DateTime");
}


inline void VarHolder::convert(LocalDateTime& /*val*/) const
{
	throw BadCastException("Can not convert to LocalDateTime");
}


inline void VarHolder::convert(Timestamp& /*val*/) const
{
	throw BadCastException("Can not convert to Timestamp");
}


inline void VarHolder::convert(UUID& /*val*/) const
{
	throw BadCastException("Can not convert to UUID");
}


#ifndef POCO_INT64_IS_LONG


inline void VarHolder::convert(long& val) const
{
	Int32 tmp;
	convert(tmp);
	val = tmp;
}


inline void VarHolder::convert(unsigned long& val) const
{
	UInt32 tmp;
	convert(tmp);
	val = tmp;
}


#else


inline void VarHolder::convert(long long& /*val*/) const
{
	throw BadCastException("Can not convert to long long");
}


inline void VarHolder::convert(unsigned long long& /*val*/) const
{
	throw BadCastException("Can not convert to unsigned long long");
}


#endif


inline void VarHolder::convert(bool& /*val*/) const
{
	throw BadCastException("Can not convert to bool");
}


inline void VarHolder::convert(float& /*val*/) const
{
	throw BadCastException("Can not convert to float");
}


inline void VarHolder::convert(double& /*val*/) const
{
	throw BadCastException("Can not convert to double");
}


inline void VarHolder::convert(char& /*val*/) const
{
	throw BadCastException("Can not convert to char");
}


inline void VarHolder::convert(std::string& /*val*/) const
{
	throw BadCastException("Can not convert to std::string");
}


inline void VarHolder::convert(Poco::UTF16String& /*val*/) const
{
	throw BadCastException("Can not convert to Poco::UTF16String");
}


inline bool VarHolder::isArray() const
{
	return true;
}


inline bool VarHolder::isVector() const
{
	return false;
}


inline bool VarHolder::isList() const
{
	return false;
}


inline bool VarHolder::isDeque() const
{
	return false;
}


inline bool VarHolder::isStruct() const
{
	return false;
}


inline bool VarHolder::isOrdered() const
{
	return false;
}


inline bool VarHolder::isInteger() const
{
	return false;
}


inline bool VarHolder::isSigned() const
{
	return false;
}


inline bool VarHolder::isNumeric() const
{
	return false;
}


inline bool VarHolder::isBoolean() const
{
	return false;
}


inline bool VarHolder::isString() const
{
	return false;
}


inline bool VarHolder::isDate() const
{
	return false;
}


inline bool VarHolder::isTime() const
{
	return false;
}


inline bool VarHolder::isDateTime() const
{
	return false;
}


inline bool VarHolder::isUUID() const
{
	return false;
}


inline std::size_t VarHolder::size() const
{
	return 1u;
}


template <typename T>
class VarHolderImpl: public VarHolder
	/// Template based implementation of a VarHolder.
	/// This class provides type storage for user-defined types
	/// that do not have VarHolderImpl specialization.
	///
	/// The actual conversion work happens in the template specializations
	/// of this class.
	///
	/// VarHolderImpl throws following exceptions:
	///		BadCastException (if the requested conversion is not implemented)
	///		RangeException (if an attempt is made to assign a numeric value outside of the target min/max limits
	///		SyntaxException (if an attempt is made to convert a string containing non-numeric characters to number)
	///
	/// In order to support efficient direct extraction of the held value,
	/// all specializations must additionally implement a public member function:
	///
	///     const T& value() const
	///
	/// returning a const reference to the actual stored value.
{
public:
	VarHolderImpl(const T& val): _val(val)
	{
	}

	~VarHolderImpl() override = default;

	VarHolderImpl() = delete;
	VarHolderImpl(const VarHolderImpl&) = delete;
	VarHolderImpl& operator=(const VarHolderImpl&) = delete;

	const std::type_info& type() const override
	{
		return typeid(T);
	}

	void convert(Int8& val) const override
	{
		if constexpr (std::is_enum_v<T>)
		{
			convertToSigned(std::underlying_type_t<T>(_val), val);
		}
		else
		{
			VarHolder::convert(val);
		}
	}

	void convert(Int16& val) const override
	{
		if constexpr (std::is_enum_v<T>)
		{
			convertToSigned(std::underlying_type_t<T>(_val), val);
		}
		else
		{
			VarHolder::convert(val);
		}
	}

	void convert(Int32& val) const override
	{
		if constexpr (std::is_enum_v<T>)
		{
			convertToSigned(std::underlying_type_t<T>(_val), val);
		}
		else
		{
			VarHolder::convert(val);
		}
	}

	void convert(Int64& val) const override
	{
		if constexpr (std::is_enum_v<T>)
		{
			convertToSigned(std::underlying_type_t<T>(_val), val);
		}
		else
		{
			VarHolder::convert(val);
		}
	}

	void convert(UInt8& val) const override
	{
		if constexpr (std::is_enum_v<T>)
		{
			convertToUnsigned(std::underlying_type_t<T>(_val), val);
		}
		else
		{
			VarHolder::convert(val);
		}
	}

	void convert(UInt16& val) const override
	{
		if constexpr (std::is_enum_v<T>)
		{
			convertToUnsigned(std::underlying_type_t<T>(_val), val);
		}
		else
		{
			VarHolder::convert(val);
		}
	}

	void convert(UInt32& val) const override
	{
		if constexpr (std::is_enum_v<T>)
		{
			convertToUnsigned(std::underlying_type_t<T>(_val), val);
		}
		else
		{
			VarHolder::convert(val);
		}
	}

	void convert(UInt64& val) const override
	{
		if constexpr (std::is_enum_v<T>)
		{
			convertToUnsigned(std::underlying_type_t<T>(_val), val);
		}
		else
		{
			VarHolder::convert(val);
		}
	}

#ifdef POCO_INT64_IS_LONG

	void convert(long long& val) const override
	{
		if constexpr (std::is_enum_v<T>)
		{
			convertToSigned(std::underlying_type_t<T>(_val), val);
		}
		else
		{
			VarHolder::convert(val);
		}
	}

	void convert(unsigned long long& val) const override
	{
		if constexpr (std::is_enum_v<T>)
		{
			convertToUnsigned(std::underlying_type_t<T>(_val), val);
		}
		else
		{
			VarHolder::convert(val);
		}
	}

#endif

	void convert(bool& val) const override
	{
		if constexpr (std::is_enum_v<T>)
		{
			val = (std::underlying_type_t<T>(_val) != 0);
		}
		else
		{
			VarHolder::convert(val);
		}
	}

	void convert(float& val) const override
	{
		if constexpr (std::is_enum_v<T>)
		{
			val = static_cast<float>(_val);
		}
		else
		{
			VarHolder::convert(val);
		}
	}

	void convert(double& val) const override
	{
		if constexpr (std::is_enum_v<T>)
		{
			val = static_cast<double>(_val);
		}
		else
		{
			VarHolder::convert(val);
		}
	}

	void convert(char& val) const override
	{
		if constexpr (std::is_enum_v<T>)
		{
			val = static_cast<char>(_val);
		}
		else
		{
			VarHolder::convert(val);
		}
	}

	void convert(std::string& val) const override
	{
		if constexpr (std::is_enum_v<T>)
		{
			val = NumberFormatter::format(std::underlying_type_t<T>(_val));
		}
		else
		{
			VarHolder::convert(val);
		}
	}

	void convert(Poco::UTF16String& val) const override
	{
		if constexpr (std::is_enum_v<T>)
		{
			std::string str = NumberFormatter::format(std::underlying_type_t<T>(_val));
			Poco::UnicodeConverter::convert(str, val);
		}
		else
		{
			VarHolder::convert(val);
		}
	}

	bool isArray() const override
	{
		if constexpr (std::is_enum_v<T>)
		{
			return false;
		}
		else
		{
			return VarHolder::isArray();
		}
	}

	bool isStruct() const override
	{
		if constexpr (std::is_enum_v<T>)
		{
			return false;
		}
		else
		{
			return VarHolder::isStruct();
		}
	}

	bool isInteger() const override
	{
		if constexpr (std::is_enum_v<T>)
		{
			return std::numeric_limits<std::underlying_type_t<T>>::is_integer;
		}
		else
		{
			return VarHolder::isInteger();
		}
	}

	bool isSigned() const override
	{
		if constexpr (std::is_enum_v<T>)
		{
			return std::numeric_limits<std::underlying_type_t<T>>::is_signed;
		}
		else
		{
			return VarHolder::isSigned();
		}
	}

	bool isNumeric() const override
	{
		if constexpr (std::is_enum_v<T>)
		{
			return std::numeric_limits<std::underlying_type_t<T>>::is_specialized;
		}
		else
		{
			return VarHolder::isNumeric();
		}
	}

	bool isBoolean() const override
	{
		if constexpr (std::is_enum_v<T>)
		{
			return false;
		}
		else
		{
			return VarHolder::isBoolean();
		}
	}

	bool isString() const override
	{
		if constexpr (std::is_enum_v<T>)
		{
			return false;
		}
		else
		{
			return VarHolder::isString();
		}
	}

	VarHolder* clone(Placeholder<VarHolder>* pVarHolder = nullptr) const override
	{
		return cloneHolder(pVarHolder, _val);
	}

	const T& value() const
	{
		return _val;
	}

private:
	T _val;
};


template <>
class VarHolderImpl<Int8>: public VarHolder
{
public:
	VarHolderImpl(Int8 val): _val(val)
	{
	}

	~VarHolderImpl() override = default;

	VarHolderImpl() = delete;
	VarHolderImpl(const VarHolderImpl&) = delete;
	VarHolderImpl& operator=(const VarHolderImpl&) = delete;

	const std::type_info& type() const override
	{
		return typeid(Int8);
	}

	void convert(Int8& val) const override
	{
		val = _val;
	}

	void convert(Int16& val) const override
	{
		val = _val;
	}

	void convert(Int32& val) const override
	{
		val = _val;
	}

	void convert(Int64& val) const override
	{
		val = _val;
	}

	void convert(UInt8& val) const override
	{
		convertSignedToUnsigned(_val, val);
	}

	void convert(UInt16& val) const override
	{
		convertSignedToUnsigned(_val, val);
	}

	void convert(UInt32& val) const override
	{
		convertSignedToUnsigned(_val, val);
	}

	void convert(UInt64& val) const override
	{
		convertSignedToUnsigned(_val, val);
	}

#ifdef POCO_INT64_IS_LONG

	void convert(long long& val) const override
	{
		val = _val;
	}

	void convert(unsigned long long& val) const override
	{
		convertSignedToUnsigned(_val, val);
	}

#endif

	void convert(bool& val) const override
	{
		val = (_val != 0);
	}

	void convert(float& val) const override
	{
		convertToFP(_val, val);
	}

	void convert(double& val) const override
	{
		convertToFP(_val, val);
	}

	void convert(char& val) const override
	{
		val = static_cast<char>(_val);
	}

	void convert(std::string& val) const override
	{
		val = NumberFormatter::format(_val);
	}

	void convert(Poco::UTF16String& val) const override
	{
		const std::string str = NumberFormatter::format(_val);
		Poco::UnicodeConverter::convert(str, val);
	}

	VarHolder* clone(Placeholder<VarHolder>* pVarHolder = nullptr) const override
	{
		return cloneHolder(pVarHolder, _val);
	}

	const Int8& value() const
	{
		return _val;
	}

	bool isArray() const override
	{
		return false;
	}

	bool isStruct() const override
	{
		return false;
	}

	bool isInteger() const override
	{
		return std::numeric_limits<Int8>::is_integer;
	}

	bool isSigned() const override
	{
		return std::numeric_limits<Int8>::is_signed;
	}

	bool isNumeric() const override
	{
		return std::numeric_limits<Int8>::is_specialized;
	}

	bool isBoolean() const override
	{
		return false;
	}

	bool isString() const override
	{
		return false;
	}

private:
	Int8 _val;
};


template <>
class VarHolderImpl<Int16>: public VarHolder
{
public:
	VarHolderImpl(Int16 val): _val(val)
	{
	}

	~VarHolderImpl() override = default;

	VarHolderImpl() = delete;
	VarHolderImpl(const VarHolderImpl&) = delete;
	VarHolderImpl& operator=(const VarHolderImpl&) = delete;

	const std::type_info& type() const override
	{
		return typeid(Int16);
	}

	void convert(Int8& val) const override
	{
		convertToSmaller(_val, val);
	}

	void convert(Int16& val) const override
	{
		val = _val;
	}

	void convert(Int32& val) const override
	{
		val = _val;
	}

	void convert(Int64& val) const override
	{
		val = _val;
	}

	void convert(UInt8& val) const override
	{
		convertSignedToUnsigned(_val, val);
	}

	void convert(UInt16& val) const override
	{
		convertSignedToUnsigned(_val, val);
	}

	void convert(UInt32& val) const override
	{
		convertSignedToUnsigned(_val, val);
	}

	void convert(UInt64& val) const override
	{
		convertSignedToUnsigned(_val, val);
	}

#ifdef POCO_INT64_IS_LONG

	void convert(long long& val) const override
	{
		val = _val;
	}

	void convert(unsigned long long& val) const override
	{
		convertSignedToUnsigned(_val, val);
	}

#endif

	void convert(bool& val) const override
	{
		val = (_val != 0);
	}

	void convert(float& val) const override
	{
		convertToFP(_val, val);
	}

	void convert(double& val) const override
	{
		convertToFP(_val, val);
	}

	void convert(char& val) const override
	{
		UInt8 tmp;
		convert(tmp);
		val = static_cast<char>(tmp);
	}

	void convert(std::string& val) const override
	{
		val = NumberFormatter::format(_val);
	}

	void convert(Poco::UTF16String& val) const override
	{
		const std::string str = NumberFormatter::format(_val);
		Poco::UnicodeConverter::convert(str, val);
	}

	VarHolder* clone(Placeholder<VarHolder>* pVarHolder = nullptr) const override
	{
		return cloneHolder(pVarHolder, _val);
	}

	const Int16& value() const
	{
		return _val;
	}

	bool isArray() const override
	{
		return false;
	}

	bool isStruct() const override
	{
		return false;
	}

	bool isInteger() const override
	{
		return std::numeric_limits<Int16>::is_integer;
	}

	bool isSigned() const override
	{
		return std::numeric_limits<Int16>::is_signed;
	}

	bool isNumeric() const override
	{
		return std::numeric_limits<Int16>::is_specialized;
	}

	bool isString() const override
	{
		return false;
	}

private:
	Int16 _val;
};


template <>
class VarHolderImpl<Int32>: public VarHolder
{
public:
	VarHolderImpl(Int32 val): _val(val)
	{
	}

	~VarHolderImpl() override = default;

	VarHolderImpl() = delete;
	VarHolderImpl(const VarHolderImpl&) = delete;
	VarHolderImpl& operator=(const VarHolderImpl&) = delete;

	const std::type_info& type() const override
	{
		return typeid(Int32);
	}

	void convert(Int8& val) const override
	{
		convertToSmaller(_val, val);
	}

	void convert(Int16& val) const override
	{
		convertToSmaller(_val, val);
	}

	void convert(Int32& val) const override
	{
		val = _val;
	}

	void convert(Int64& val) const override
	{
		val = _val;
	}

	void convert(UInt8& val) const override
	{
		convertSignedToUnsigned(_val, val);
	}

	void convert(UInt16& val) const override
	{
		convertSignedToUnsigned(_val, val);
	}

	void convert(UInt32& val) const override
	{
		convertSignedToUnsigned(_val, val);
	}

	void convert(UInt64& val) const override
	{
		convertSignedToUnsigned(_val, val);
	}

#ifdef POCO_INT64_IS_LONG

	void convert(long long& val) const override
	{
		val = _val;
	}

	void convert(unsigned long long& val) const override
	{
		convertSignedToUnsigned(_val, val);
	}

#endif

	void convert(bool& val) const override
	{
		val = (_val != 0);
	}

	void convert(float& val) const override
	{
		convertToFP(_val, val);
	}

	void convert(double& val) const override
	{
		convertToFP(_val, val);
	}

	void convert(char& val) const override
	{
		UInt8 tmp;
		convert(tmp);
		val = static_cast<char>(tmp);
	}

	void convert(std::string& val) const override
	{
		val = NumberFormatter::format(_val);
	}

	VarHolder* clone(Placeholder<VarHolder>* pVarHolder = nullptr) const override
	{
		return cloneHolder(pVarHolder, _val);
	}

	const Int32& value() const
	{
		return _val;
	}

	bool isArray() const override
	{
		return false;
	}

	bool isStruct() const override
	{
		return false;
	}

	bool isInteger() const override
	{
		return std::numeric_limits<Int32>::is_integer;
	}

	bool isSigned() const override
	{
		return std::numeric_limits<Int32>::is_signed;
	}

	bool isNumeric() const override
	{
		return std::numeric_limits<Int32>::is_specialized;
	}

	bool isBoolean() const override
	{
		return false;
	}

	bool isString() const override
	{
		return false;
	}

private:
	Int32 _val;
};


template <>
class VarHolderImpl<Int64>: public VarHolder
{
public:
	VarHolderImpl(Int64 val): _val(val)
	{
	}

	~VarHolderImpl() override = default;

	VarHolderImpl() = delete;
	VarHolderImpl(const VarHolderImpl&) = delete;
	VarHolderImpl& operator=(const VarHolderImpl&) = delete;

	const std::type_info& type() const override
	{
		return typeid(Int64);
	}

	void convert(Int8& val) const override
	{
		convertToSmaller(_val, val);
	}

	void convert(Int16& val) const override
	{
		convertToSmaller(_val, val);
	}

	void convert(Int32& val) const override
	{
		convertToSmaller(_val, val);
	}

	void convert(Int64& val) const override
	{
		val = _val;
	}

	void convert(UInt8& val) const override
	{
		convertSignedToUnsigned(_val, val);
	}

	void convert(UInt16& val) const override
	{
		convertSignedToUnsigned(_val, val);
	}

	void convert(UInt32& val) const override
	{
		convertSignedToUnsigned(_val, val);
	}

	void convert(UInt64& val) const override
	{
		convertSignedToUnsigned(_val, val);
	}

#ifdef POCO_INT64_IS_LONG

	void convert(long long& val) const override
	{
		val = _val;
	}

	void convert(unsigned long long& val) const override
	{
		convertSignedToUnsigned(_val, val);
	}

#endif

	void convert(bool& val) const override
	{
		val = (_val != 0);
	}

	void convert(float& val) const override
	{
		convertToFP(_val, val);
	}

	void convert(double& val) const override
	{
		convertToFP(_val, val);
	}

	void convert(char& val) const override
	{
		UInt8 tmp;
		convert(tmp);
		val = static_cast<char>(tmp);
	}

	void convert(std::string& val) const override
	{
		val = NumberFormatter::format(_val);
	}

	void convert(DateTime& dt) const override
	{
		dt = Timestamp(_val);
	}

	void convert(LocalDateTime& ldt) const override
	{
		ldt = Timestamp(_val);
	}

	void convert(Timestamp& val) const override
	{
		val = Timestamp(_val);
	}

	VarHolder* clone(Placeholder<VarHolder>* pVarHolder = nullptr) const override
	{
		return cloneHolder(pVarHolder, _val);
	}

	const Int64& value() const
	{
		return _val;
	}

	bool isArray() const override
	{
		return false;
	}

	bool isStruct() const override
	{
		return false;
	}

	bool isInteger() const override
	{
		return std::numeric_limits<Int64>::is_integer;
	}

	bool isSigned() const override
	{
		return std::numeric_limits<Int64>::is_signed;
	}

	bool isNumeric() const override
	{
		return std::numeric_limits<Int64>::is_specialized;
	}

	bool isBoolean() const override
	{
		return false;
	}

	bool isString() const override
	{
		return false;
	}

private:
	Int64 _val;
};


template <>
class VarHolderImpl<UInt8>: public VarHolder
{
public:
	VarHolderImpl(UInt8 val): _val(val)
	{
	}

	~VarHolderImpl() override = default;

	VarHolderImpl() = delete;
	VarHolderImpl(const VarHolderImpl&) = delete;
	VarHolderImpl& operator=(const VarHolderImpl&) = delete;

	const std::type_info& type() const override
	{
		return typeid(UInt8);
	}

	void convert(Int8& val) const override
	{
		convertUnsignedToSigned(_val, val);
	}

	void convert(Int16& val) const override
	{
		convertUnsignedToSigned(_val, val);
	}

	void convert(Int32& val) const override
	{
		val = static_cast<Int32>(_val);
	}

	void convert(Int64& val) const override
	{
		val = static_cast<Int64>(_val);
	}

	void convert(UInt8& val) const override
	{
		val = _val;
	}

	void convert(UInt16& val) const override
	{
		val = _val;
	}

	void convert(UInt32& val) const override
	{
		val = _val;
	}

	void convert(UInt64& val) const override
	{
		val = _val;
	}

#ifdef POCO_INT64_IS_LONG

	void convert(long long& val) const override
	{
		val = static_cast<long long>(_val);
	}

	void convert(unsigned long long& val) const override
	{
		val = _val;
	}

#endif

	void convert(bool& val) const override
	{
		val = (_val != 0);
	}

	void convert(float& val) const override
	{
		convertToFP(_val, val);
	}

	void convert(double& val) const override
	{
		convertToFP(_val, val);
	}

	void convert(char& val) const override
	{
		UInt8 tmp;
		convert(tmp);
		val = static_cast<char>(tmp);
	}

	void convert(std::string& val) const override
	{
		val = NumberFormatter::format(_val);
	}

	VarHolder* clone(Placeholder<VarHolder>* pVarHolder = nullptr) const override
	{
		return cloneHolder(pVarHolder, _val);
	}

	const UInt8& value() const
	{
		return _val;
	}

	bool isArray() const override
	{
		return false;
	}

	bool isStruct() const override
	{
		return false;
	}

	bool isInteger() const override
	{
		return std::numeric_limits<UInt8>::is_integer;
	}

	bool isSigned() const override
	{
		return std::numeric_limits<UInt8>::is_signed;
	}

	bool isNumeric() const override
	{
		return std::numeric_limits<UInt8>::is_specialized;
	}

	bool isBoolean() const override
	{
		return false;
	}

	bool isString() const override
	{
		return false;
	}

private:
	UInt8 _val;
};


template <>
class VarHolderImpl<UInt16>: public VarHolder
{
public:
	VarHolderImpl(UInt16 val): _val(val)
	{
	}

	~VarHolderImpl() override = default;

	VarHolderImpl() = delete;
	VarHolderImpl(const VarHolderImpl&) = delete;
	VarHolderImpl& operator=(const VarHolderImpl&) = delete;

	const std::type_info& type() const override
	{
		return typeid(UInt16);
	}

	void convert(Int8& val) const override
	{
		convertUnsignedToSigned(_val, val);
	}

	void convert(Int16& val) const override
	{
		convertUnsignedToSigned(_val, val);
	}

	void convert(Int32& val) const override
	{
		convertUnsignedToSigned(_val, val);
	}

	void convert(Int64& val) const override
	{
		val = static_cast<Int64>(_val);
	}

	void convert(UInt8& val) const override
	{
		convertToSmallerUnsigned(_val, val);
	}

	void convert(UInt16& val) const override
	{
		val = _val;
	}

	void convert(UInt32& val) const override
	{
		val = _val;
	}

	void convert(UInt64& val) const override
	{
		val = _val;
	}

#ifdef POCO_INT64_IS_LONG

	void convert(long long& val) const override
	{
		val = static_cast<long long>(_val);
	}

	void convert(unsigned long long& val) const override
	{
		val = _val;
	}

#endif

	void convert(bool& val) const override
	{
		val = (_val != 0);
	}

	void convert(float& val) const override
	{
		convertToFP(_val, val);
	}

	void convert(double& val) const override
	{
		convertToFP(_val, val);
	}

	void convert(char& val) const override
	{
		UInt8 tmp;
		convert(tmp);
		val = static_cast<char>(tmp);
	}

	void convert(std::string& val) const override
	{
		val = NumberFormatter::format(_val);
	}

	VarHolder* clone(Placeholder<VarHolder>* pVarHolder = nullptr) const override
	{
		return cloneHolder(pVarHolder, _val);
	}

	const UInt16& value() const
	{
		return _val;
	}

	bool isArray() const override
	{
		return false;
	}

	bool isStruct() const override
	{
		return false;
	}

	bool isInteger() const override
	{
		return std::numeric_limits<UInt16>::is_integer;
	}

	bool isSigned() const override
	{
		return std::numeric_limits<UInt16>::is_signed;
	}

	bool isNumeric() const override
	{
		return std::numeric_limits<UInt16>::is_specialized;
	}

	bool isBoolean() const override
	{
		return false;
	}

	bool isString() const override
	{
		return false;
	}

private:
	UInt16 _val;
};


template <>
class VarHolderImpl<UInt32>: public VarHolder
{
public:
	VarHolderImpl(UInt32 val): _val(val)
	{
	}

	~VarHolderImpl() override = default;

	VarHolderImpl() = delete;
	VarHolderImpl(const VarHolderImpl&) = delete;
	VarHolderImpl& operator=(const VarHolderImpl&) = delete;

	const std::type_info& type() const override
	{
		return typeid(UInt32);
	}

	void convert(Int8& val) const override
	{
		convertUnsignedToSigned(_val, val);
	}

	void convert(Int16& val) const override
	{
		convertUnsignedToSigned(_val, val);
	}

	void convert(Int32& val) const override
	{
		convertUnsignedToSigned(_val, val);
	}

	void convert(Int64& val) const override
	{
		convertUnsignedToSigned(_val, val);
	}

	void convert(UInt8& val) const override
	{
		convertToSmallerUnsigned(_val, val);
	}

	void convert(UInt16& val) const override
	{
		convertToSmallerUnsigned(_val, val);
	}

	void convert(UInt32& val) const override
	{
		val = _val;
	}

	void convert(UInt64& val) const override
	{
		val = _val;
	}

#ifdef POCO_INT64_IS_LONG

	void convert(long long& val) const override
	{
		convertUnsignedToSigned(_val, val);
	}

	void convert(unsigned long long& val) const override
	{
		val = _val;
	}

#endif

	void convert(bool& val) const override
	{
		val = (_val != 0);
	}

	void convert(float& val) const override
	{
		convertToFP(_val, val);
	}

	void convert(double& val) const override
	{
		convertToFP(_val, val);
	}

	void convert(char& val) const override
	{
		UInt8 tmp;
		convert(tmp);
		val = static_cast<char>(tmp);
	}

	void convert(std::string& val) const override
	{
		val = NumberFormatter::format(_val);
	}

	VarHolder* clone(Placeholder<VarHolder>* pVarHolder = nullptr) const override
	{
		return cloneHolder(pVarHolder, _val);
	}

	const UInt32& value() const
	{
		return _val;
	}

	bool isArray() const override
	{
		return false;
	}

	bool isStruct() const override
	{
		return false;
	}

	bool isInteger() const override
	{
		return std::numeric_limits<UInt32>::is_integer;
	}

	bool isSigned() const override
	{
		return std::numeric_limits<UInt32>::is_signed;
	}

	bool isNumeric() const override
	{
		return std::numeric_limits<UInt32>::is_specialized;
	}

	bool isBoolean() const override
	{
		return false;
	}

	bool isString() const override
	{
		return false;
	}

private:
	UInt32 _val;
};


template <>
class VarHolderImpl<UInt64>: public VarHolder
{
public:
	VarHolderImpl(UInt64 val): _val(val)
	{
	}

	~VarHolderImpl() override = default;

	VarHolderImpl() = delete;
	VarHolderImpl(const VarHolderImpl&) = delete;
	VarHolderImpl& operator=(const VarHolderImpl&) = delete;

	const std::type_info& type() const override
	{
		return typeid(UInt64);
	}

	void convert(Int8& val) const override
	{
		convertUnsignedToSigned(_val, val);
	}

	void convert(Int16& val) const override
	{
		convertUnsignedToSigned(_val, val);
	}

	void convert(Int32& val) const override
	{
		convertUnsignedToSigned(_val, val);
	}

	void convert(Int64& val) const override
	{
		convertUnsignedToSigned(_val, val);
	}

	void convert(UInt8& val) const override
	{
		convertToSmallerUnsigned(_val, val);
	}

	void convert(UInt16& val) const override
	{
		convertToSmallerUnsigned(_val, val);
	}

	void convert(UInt32& val) const override
	{
		convertToSmallerUnsigned(_val, val);
	}

	void convert(UInt64& val) const override
	{
		val = _val;
	}

#ifdef POCO_INT64_IS_LONG

	void convert(long long& val) const override
	{
		convertUnsignedToSigned(_val, val);
	}

	void convert(unsigned long long& val) const override
	{
		val = _val;
	}

#endif

	void convert(bool& val) const override
	{
		val = (_val != 0);
	}

	void convert(float& val) const override
	{
		convertToFP(_val, val);
	}

	void convert(double& val) const override
	{
		convertToFP(_val, val);
	}

	void convert(char& val) const override
	{
		UInt8 tmp;
		convert(tmp);
		val = static_cast<char>(tmp);
	}

	void convert(std::string& val) const override
	{
		val = NumberFormatter::format(_val);
	}

	void convert(DateTime& dt) const override
	{
		Int64 val;
		convertUnsignedToSigned(_val, val);
		dt = Timestamp(val);
	}

	void convert(LocalDateTime& ldt) const override
	{
		Int64 val;
		convertUnsignedToSigned(_val, val);
		ldt = Timestamp(val);
	}

	void convert(Timestamp& val) const override
	{
		Int64 tmp;
		convertUnsignedToSigned(_val, tmp);
		val = Timestamp(tmp);
	}

	VarHolder* clone(Placeholder<VarHolder>* pVarHolder = nullptr) const override
	{
		return cloneHolder(pVarHolder, _val);
	}

	const UInt64& value() const
	{
		return _val;
	}

	bool isArray() const override
	{
		return false;
	}

	bool isStruct() const override
	{
		return false;
	}

	bool isInteger() const override
	{
		return std::numeric_limits<UInt64>::is_integer;
	}

	bool isSigned() const override
	{
		return std::numeric_limits<UInt64>::is_signed;
	}

	bool isNumeric() const override
	{
		return std::numeric_limits<UInt64>::is_specialized;
	}

	bool isBoolean() const override
	{
		return false;
	}

	bool isString() const override
	{
		return false;
	}

private:
	UInt64 _val;
};


template <>
class VarHolderImpl<bool>: public VarHolder
{
public:
	VarHolderImpl(bool val): _val(val)
	{
	}

	~VarHolderImpl() override = default;
	VarHolderImpl() = delete;
	VarHolderImpl(const VarHolderImpl&) = delete;
	VarHolderImpl& operator=(const VarHolderImpl&) = delete;

	const std::type_info& type() const override
	{
		return typeid(bool);
	}

	void convert(Int8& val) const override
	{
		val = static_cast<Int8>(_val ? 1 : 0);
	}

	void convert(Int16& val) const override
	{
		val = static_cast<Int16>(_val ? 1 : 0);
	}

	void convert(Int32& val) const override
	{
		val = static_cast<Int32>(_val ? 1 : 0);
	}

	void convert(Int64& val) const override
	{
		val = static_cast<Int64>(_val ? 1 : 0);
	}

	void convert(UInt8& val) const override
	{
		val = static_cast<UInt8>(_val ? 1 : 0);
	}

	void convert(UInt16& val) const override
	{
		val = static_cast<UInt16>(_val ? 1 : 0);
	}

	void convert(UInt32& val) const override
	{
		val = static_cast<UInt32>(_val ? 1 : 0);
	}

	void convert(UInt64& val) const override
	{
		val = static_cast<UInt64>(_val ? 1 : 0);
	}

#ifdef POCO_INT64_IS_LONG

	void convert(long long& val) const override
	{
		val = static_cast<long long>(_val ? 1 : 0);
	}

	void convert(unsigned long long& val) const override
	{
		val = static_cast<unsigned long long>(_val ? 1 : 0);
	}

#endif

	void convert(bool& val) const override
	{
		val = _val;
	}

	void convert(float& val) const override
	{
		val = (_val ? 1.0f : 0.0f);
	}

	void convert(double& val) const override
	{
		val = (_val ? 1.0 : 0.0);
	}

	void convert(char& val) const override
	{
		val = static_cast<char>(_val ? 1 : 0);
	}

	void convert(std::string& val) const override
	{
		val = (_val ? "true" : "false");
	}

	VarHolder* clone(Placeholder<VarHolder>* pVarHolder = nullptr) const override
	{
		return cloneHolder(pVarHolder, _val);
	}

	const bool& value() const
	{
		return _val;
	}

	bool isArray() const override
	{
		return false;
	}

	bool isStruct() const override
	{
		return false;
	}

	bool isInteger() const override
	{
		return std::numeric_limits<bool>::is_integer;
	}

	bool isSigned() const override
	{
		return std::numeric_limits<bool>::is_signed;
	}

	bool isNumeric() const override
	{
		return std::numeric_limits<bool>::is_specialized;
	}

	bool isBoolean() const override
	{
		return true;
	}

	bool isString() const override
	{
		return false;
	}

private:
	bool _val;
};


template <>
class VarHolderImpl<float>: public VarHolder
{
public:
	VarHolderImpl(float val): _val(val)
	{
	}

	~VarHolderImpl() override = default;

	VarHolderImpl() = delete;
	VarHolderImpl(const VarHolderImpl&) = delete;
	VarHolderImpl& operator=(const VarHolderImpl&) = delete;

	const std::type_info& type() const override
	{
		return typeid(float);
	}

	void convert(Int8& val) const override
	{
		convertToSmaller(_val, val);
	}

	void convert(Int16& val) const override
	{
		convertToSmaller(_val, val);
	}

	void convert(Int32& val) const override
	{
		convertToSmaller(_val, val);
	}

	void convert(Int64& val) const override
	{
		convertToSmaller(_val, val);
	}

	void convert(UInt8& val) const override
	{
		convertSignedFloatToUnsigned(_val, val);
	}

	void convert(UInt16& val) const override
	{
		convertSignedFloatToUnsigned(_val, val);
	}

	void convert(UInt32& val) const override
	{
		convertSignedFloatToUnsigned(_val, val);
	}

	void convert(UInt64& val) const override
	{
		convertSignedFloatToUnsigned(_val, val);
	}

#ifdef POCO_INT64_IS_LONG

	void convert(long long& val) const override
	{
		convertToSmaller(_val, val);
	}

	void convert(unsigned long long& val) const override
	{
		convertSignedFloatToUnsigned(_val, val);
	}

#endif

	void convert(bool& val) const override
	{
		val = _val > std::numeric_limits<float>::min() ||
			_val < -1 * std::numeric_limits<float>::min();
	}

	void convert(float& val) const override
	{
		val = _val;
	}

	void convert(double& val) const override
	{
		val = _val;
	}

	void convert(char& val) const override
	{
		UInt8 tmp;
		convert(tmp);
		val = static_cast<char>(tmp);
	}

	void convert(std::string& val) const override
	{
		val = NumberFormatter::format(_val);
	}

	VarHolder* clone(Placeholder<VarHolder>* pVarHolder = nullptr) const override
	{
		return cloneHolder(pVarHolder, _val);
	}

	const float& value() const
	{
		return _val;
	}

	bool isArray() const override
	{
		return false;
	}

	bool isStruct() const override
	{
		return false;
	}

	bool isInteger() const override
	{
		return std::numeric_limits<float>::is_integer;
	}

	bool isSigned() const override
	{
		return std::numeric_limits<float>::is_signed;
	}

	bool isNumeric() const override
	{
		return std::numeric_limits<float>::is_specialized;
	}

	bool isBoolean() const override
	{
		return false;
	}

	bool isString() const override
	{
		return false;
	}

private:
	float _val;
};


template <>
class VarHolderImpl<double>: public VarHolder
{
public:
	VarHolderImpl(double val): _val(val)
	{
	}

	~VarHolderImpl() override = default;

	VarHolderImpl() = delete;
	VarHolderImpl(const VarHolderImpl&) = delete;
	VarHolderImpl& operator=(const VarHolderImpl&) = delete;

	const std::type_info& type() const override
	{
		return typeid(double);
	}

	void convert(Int8& val) const override
	{
		convertToSmaller(_val, val);
	}

	void convert(Int16& val) const override
	{
		convertToSmaller(_val, val);
	}

	void convert(Int32& val) const override
	{
		convertToSmaller(_val, val);
	}

	void convert(Int64& val) const override
	{
		convertToSmaller(_val, val);
	}

	void convert(UInt8& val) const override
	{
		convertSignedFloatToUnsigned(_val, val);
	}

	void convert(UInt16& val) const override
	{
		convertSignedFloatToUnsigned(_val, val);
	}

	void convert(UInt32& val) const override
	{
		convertSignedFloatToUnsigned(_val, val);
	}

	void convert(UInt64& val) const override
	{
		convertSignedFloatToUnsigned(_val, val);
	}

#ifdef POCO_INT64_IS_LONG

	void convert(long long& val) const override
	{
		convertToSmaller(_val, val);
	}

	void convert(unsigned long long& val) const override
	{
		convertSignedFloatToUnsigned(_val, val);
	}

#endif

	void convert(bool& val) const override
	{
		val = _val > std::numeric_limits<double>::min() ||
			_val < -1 * std::numeric_limits<double>::min();
	}

	void convert(float& val) const override
	{
		const double fMin = -1 * std::numeric_limits<float>::max();
		const double fMax = std::numeric_limits<float>::max();

		if (_val < fMin) throw RangeException("Value too small.");
		if (_val > fMax) throw RangeException("Value too large.");

		val = static_cast<float>(_val);
	}

	void convert(double& val) const override
	{
		val = _val;
	}

	void convert(char& val) const override
	{
		UInt8 tmp;
		convert(tmp);
		val = static_cast<char>(tmp);
	}

	void convert(std::string& val) const override
	{
		val = NumberFormatter::format(_val);
	}

	VarHolder* clone(Placeholder<VarHolder>* pVarHolder = nullptr) const override
	{
		return cloneHolder(pVarHolder, _val);
	}

	const double& value() const
	{
		return _val;
	}

	bool isArray() const override
	{
		return false;
	}

	bool isStruct() const override
	{
		return false;
	}

	bool isInteger() const override
	{
		return std::numeric_limits<double>::is_integer;
	}

	bool isSigned() const override
	{
		return std::numeric_limits<double>::is_signed;
	}

	bool isNumeric() const override
	{
		return std::numeric_limits<double>::is_specialized;
	}

	bool isBoolean() const override
	{
		return false;
	}

	bool isString() const override
	{
		return false;
	}

private:
	double _val;
};


template <>
class VarHolderImpl<char>: public VarHolder
{
public:
	VarHolderImpl(char val): _val(val)
	{
	}

	~VarHolderImpl() override = default;

	VarHolderImpl() = delete;
	VarHolderImpl(const VarHolderImpl&) = delete;
	VarHolderImpl& operator=(const VarHolderImpl&) = delete;

	const std::type_info& type() const override
	{
		return typeid(char);
	}

	void convert(Int8& val) const override
	{
		val = static_cast<Int8>(_val);
	}

	void convert(Int16& val) const override
	{
		val = static_cast<UInt8>(_val);
	}

	void convert(Int32& val) const override
	{
		val = static_cast<UInt8>(_val);
	}

	void convert(Int64& val) const override
	{
		val = static_cast<UInt8>(_val);
	}

	void convert(UInt8& val) const override
	{
		val = static_cast<UInt8>(_val);
	}

	void convert(UInt16& val) const override
	{
		val = static_cast<UInt8>(_val);
	}

	void convert(UInt32& val) const override
	{
		val = static_cast<UInt8>(_val);
	}

	void convert(UInt64& val) const override
	{
		val = static_cast<UInt8>(_val);
	}

#ifdef POCO_INT64_IS_LONG

	void convert(long long& val) const override
	{
		val = static_cast<long long>(_val);
	}

	void convert(unsigned long long& val) const override
	{
		val = static_cast<unsigned long long>(_val);
	}

#endif

	void convert(bool& val) const override
	{
		val = (_val != '\0');
	}

	void convert(float& val) const override
	{
		val = static_cast<float>(_val);
	}

	void convert(double& val) const override
	{
		val = static_cast<double>(_val);
	}

	void convert(char& val) const override
	{
		val = _val;
	}

	void convert(std::string& val) const override
	{
		val = std::string(1, _val);
	}

	VarHolder* clone(Placeholder<VarHolder>* pVarHolder = nullptr) const override
	{
		return cloneHolder(pVarHolder, _val);
	}

	const char& value() const
	{
		return _val;
	}

	bool isArray() const override
	{
		return false;
	}

	bool isStruct() const override
	{
		return false;
	}

	bool isInteger() const override
	{
		return std::numeric_limits<char>::is_integer;
	}

	bool isSigned() const override
	{
		return std::numeric_limits<char>::is_signed;
	}

	bool isNumeric() const override
	{
		return std::numeric_limits<char>::is_specialized;
	}

	bool isBoolean() const override
	{
		return false;
	}

	bool isString() const override
	{
		return false;
	}

private:
	char _val;
};


template <>
class VarHolderImpl<std::string>: public VarHolder
{
public:
	VarHolderImpl(const char* pVal): _val(pVal)
	{
	}

	VarHolderImpl(const std::string& val) : _val(val)
	{
	}

	~VarHolderImpl() override = default;

	VarHolderImpl() = delete;
	VarHolderImpl(const VarHolderImpl&) = delete;
	VarHolderImpl& operator=(const VarHolderImpl&) = delete;

	const std::type_info& type() const override
	{
		return typeid(std::string);
	}

	void convert(Int8& val) const override
	{
		const int v = NumberParser::parse(_val);
		convertToSmaller(v, val);
	}

	void convert(Int16& val) const override
	{
		const int v = NumberParser::parse(_val);
		convertToSmaller(v, val);
	}

	void convert(Int32& val) const override
	{
		val = NumberParser::parse(_val);
	}

	void convert(Int64& val) const override
	{
		val = NumberParser::parse64(_val);
	}

	void convert(UInt8& val) const override
	{
		const unsigned int v = NumberParser::parseUnsigned(_val);
		convertToSmallerUnsigned(v, val);
	}

	void convert(UInt16& val) const override
	{
		const unsigned int v = NumberParser::parseUnsigned(_val);
		convertToSmallerUnsigned(v, val);
	}

	void convert(UInt32& val) const override
	{
		val = NumberParser::parseUnsigned(_val);
	}

	void convert(UInt64& val) const override
	{
		val = NumberParser::parseUnsigned64(_val);
	}

#ifdef POCO_INT64_IS_LONG

	void convert(long long& val) const override
	{
		val = NumberParser::parse64(_val);
	}

	void convert(unsigned long long& val) const override
	{
		val = NumberParser::parseUnsigned64(_val);
	}

#endif

	void convert(bool& val) const override
	{
		if (_val.empty())
		{
			val = false;
			return;
		}

		static const std::string VAL_FALSE("false");
		static const std::string VAL_INT_FALSE("0");
		val = (_val != VAL_INT_FALSE &&
			(icompare(_val, VAL_FALSE) != 0));
	}

	void convert(float& val) const override
	{
		const double v = NumberParser::parseFloat(_val);
		convertToSmaller(v, val);
	}

	void convert(double& val) const override
	{
		val = NumberParser::parseFloat(_val);
	}

	void convert(char& val) const override
	{
		if (_val.empty())
			val = '\0';
		else
			val = _val[0];
	}

	void convert(std::string& val) const override
	{
		val = _val;
	}

	void convert(Poco::UTF16String& val) const override
	{
		Poco::UnicodeConverter::convert(_val, val);
	}

	void convert(DateTime& val) const override
	{
		int tzd = 0;
		if (!DateTimeParser::tryParse(DateTimeFormat::ISO8601_FORMAT, _val, val, tzd))
			throw BadCastException("string -> DateTime");
	}

	void convert(LocalDateTime& ldt) const override
	{
		int tzd = 0;
		DateTime tmp;
		if (!DateTimeParser::tryParse(DateTimeFormat::ISO8601_FORMAT, _val, tmp, tzd))
			throw BadCastException("string -> LocalDateTime");

		ldt = LocalDateTime(tzd, tmp, false);
	}

	void convert(Timestamp& ts) const override
	{
		int tzd = 0;
		DateTime tmp;
		if (!DateTimeParser::tryParse(DateTimeFormat::ISO8601_FORMAT, _val, tmp, tzd))
			throw BadCastException("string -> Timestamp");

		ts = tmp.timestamp();
	}

	void convert(UUID& uuid) const override
	{
		uuid.parse(_val);
	}

	VarHolder* clone(Placeholder<VarHolder>* pVarHolder = nullptr) const override
	{
		return cloneHolder(pVarHolder, _val);
	}

	const std:: string& value() const
	{
		return _val;
	}

	bool isString() const override
	{
		return true;
	}

	std::size_t size() const override
	{
		return _val.length();
	}

	char& operator[](std::string::size_type n)
	{
		if (n < size()) return _val.operator[](n);

		throw RangeException("String index out of range");
	}

	const char& operator[](std::string::size_type n) const
	{
		if (n < size()) return _val.operator[](n);

		throw RangeException("String index out of range");
	}

private:
	std::string _val;
};


template <>
class VarHolderImpl<UTF16String>: public VarHolder
{
public:
	VarHolderImpl(const char* pVal) : _val(Poco::UnicodeConverter::to<UTF16String>(pVal))
	{
	}

	VarHolderImpl(const Poco::UTF16String& val) : _val(val)
	{
	}

	~VarHolderImpl() override = default;

	VarHolderImpl() = delete;
	VarHolderImpl(const VarHolderImpl&) = delete;
	VarHolderImpl& operator=(const VarHolderImpl&) = delete;

	const std::type_info& type() const override
	{
		return typeid(Poco::UTF16String);
	}

	void convert(Int8& val) const override
	{
		const int v = NumberParser::parse(toStdString());
		convertToSmaller(v, val);
	}

	void convert(Int16& val) const override
	{
		const int v = NumberParser::parse(toStdString());
		convertToSmaller(v, val);
	}

	void convert(Int32& val) const override
	{
		val = NumberParser::parse(toStdString());
	}

	void convert(Int64& val) const override
	{
		val = NumberParser::parse64(toStdString());
	}

	void convert(UInt8& val) const override
	{
		const unsigned int v = NumberParser::parseUnsigned(toStdString());
		convertToSmallerUnsigned(v, val);
	}

	void convert(UInt16& val) const override
	{
		const unsigned int v = NumberParser::parseUnsigned(toStdString());
		convertToSmallerUnsigned(v, val);
	}

	void convert(UInt32& val) const override
	{
		val = NumberParser::parseUnsigned(toStdString());
	}

	void convert(UInt64& val) const override
	{
		val = NumberParser::parseUnsigned64(toStdString());
	}

#ifdef POCO_INT64_IS_LONG

	void convert(long long& val) const override
	{
		val = NumberParser::parse64(toStdString());
	}

	void convert(unsigned long long& val) const override
	{
		val = NumberParser::parseUnsigned64(toStdString());
	}

#endif

	void convert(bool& val) const override
	{
		static const std::string VAL_FALSE("false");
		static const std::string VAL_INT_FALSE("0");

		if (_val.empty()) val = false;

		std::string str;
		UnicodeConverter::convert(_val, str);
		val = (str != VAL_INT_FALSE &&
			(icompare(str, VAL_FALSE) != 0));
	}

	void convert(float& val) const override
	{
		const double v = NumberParser::parseFloat(toStdString());
		convertToSmaller(v, val);
	}

	void convert(double& val) const override
	{
		val = NumberParser::parseFloat(toStdString());
	}

	void convert(char& val) const override
	{
		if (_val.empty())
			val = '\0';
		else
		{
			std::string s;
			UnicodeConverter::convert(_val, s);
			val = s[0];
		}
	}

	void convert(Poco::UTF16String& val) const override
	{
		val = _val;
	}

	void convert(std::string& val) const override
	{
		UnicodeConverter::convert(_val, val);
	}

	void convert(DateTime& val) const override
	{
		int tzd = 0;
		if (!DateTimeParser::tryParse(DateTimeFormat::ISO8601_FORMAT, toStdString(), val, tzd))
			throw BadCastException("string -> DateTime");
	}

	void convert(LocalDateTime& ldt) const override
	{
		int tzd = 0;
		DateTime tmp;
		if (!DateTimeParser::tryParse(DateTimeFormat::ISO8601_FORMAT, toStdString(), tmp, tzd))
			throw BadCastException("string -> LocalDateTime");

		ldt = LocalDateTime(tzd, tmp, false);
	}

	void convert(Timestamp& ts) const override
	{
		int tzd = 0;
		DateTime tmp;
		if (!DateTimeParser::tryParse(DateTimeFormat::ISO8601_FORMAT, toStdString(), tmp, tzd))
			throw BadCastException("string -> Timestamp");

		ts = tmp.timestamp();
	}

	VarHolder* clone(Placeholder<VarHolder>* pVarHolder = nullptr) const override
	{
		return cloneHolder(pVarHolder, _val);
	}

	const Poco::UTF16String& value() const
	{
		return _val;
	}

	bool isString() const override
	{
		return true;
	}

	std::size_t size() const override
	{
		return _val.length();
	}

	UTF16Char& operator[](Poco::UTF16String::size_type n)
	{
		if (n < size()) return _val.operator[](n);

		throw RangeException("String index out of range");
	}

	const UTF16Char& operator[](Poco::UTF16String::size_type n) const
	{
		if (n < size()) return _val.operator[](n);

		throw RangeException("String index out of range");
	}

private:
	std::string toStdString() const
	{
		std::string str;
		UnicodeConverter::convert(_val, str);
		return str;
	}

	Poco::UTF16String _val;
};


#ifndef POCO_INT64_IS_LONG


template <>
class VarHolderImpl<long>: public VarHolder
{
public:
	VarHolderImpl(long val): _val(val)
	{
	}

	~VarHolderImpl() override = default;

	VarHolderImpl() = delete;
	VarHolderImpl(const VarHolderImpl&) = delete;
	VarHolderImpl& operator = (const VarHolderImpl&) = delete;

	const std::type_info& type() const override
	{
		return typeid(long);
	}

	void convert(Int8& val) const override
	{
		convertToSmaller(_val, val);
	}

	void convert(Int16& val) const override
	{
		convertToSmaller(_val, val);
	}

	void convert(Int32& val) const override
	{
		val = static_cast<Int32>(_val);
	}

	void convert(Int64& val) const override
	{
		val = static_cast<Int64>(_val);
	}

	void convert(UInt8& val) const override
	{
		convertSignedToUnsigned(_val, val);
	}

	void convert(UInt16& val) const override
	{
		convertSignedToUnsigned(_val, val);
	}

	void convert(UInt32& val) const override
	{
		convertSignedToUnsigned(_val, val);
	}

	void convert(UInt64& val) const override
	{
		convertSignedToUnsigned(_val, val);
	}

	void convert(bool& val) const override
	{
		val = (_val != 0);
	}

	void convert(float& val) const override
	{
		val = static_cast<float>(_val);
	}

	void convert(double& val) const override
	{
		val = static_cast<double>(_val);
	}

	void convert(char& val) const override
	{
		UInt8 tmp;
		convert(tmp);
		val = static_cast<char>(tmp);
	}

	void convert(std::string& val) const override
	{
		val = NumberFormatter::format(_val);
	}

	VarHolder* clone(Placeholder<VarHolder>* pVarHolder = 0) const override
	{
		return cloneHolder(pVarHolder, _val);
	}

	const long& value() const
	{
		return _val;
	}

	bool isArray() const override
	{
		return false;
	}

	bool isStruct() const override
	{
		return false;
	}

	bool isInteger() const override
	{
		return std::numeric_limits<long>::is_integer;
	}

	bool isSigned() const override
	{
		return std::numeric_limits<long>::is_signed;
	}

	bool isNumeric() const override
	{
		return std::numeric_limits<long>::is_specialized;
	}

	bool isBoolean() const override
	{
		return false;
	}

	bool isString() const override
	{
		return false;
	}

private:
	long _val;
};


template <>
class VarHolderImpl<unsigned long>: public VarHolder
{
public:
	VarHolderImpl(unsigned long val): _val(val)
	{
	}

	~VarHolderImpl() override = default;

	VarHolderImpl() = delete;
	VarHolderImpl(const VarHolderImpl&) = delete;
	VarHolderImpl& operator = (const VarHolderImpl&) = delete;

	const std::type_info& type() const override
	{
		return typeid(unsigned long);
	}

	void convert(Int8& val) const override
	{
		convertUnsignedToSigned(_val, val);
	}

	void convert(Int16& val) const override
	{
		convertUnsignedToSigned(_val, val);
	}

	void convert(Int32& val) const override
	{
		convertUnsignedToSigned(_val, val);
	}

	void convert(Int64& val) const override
	{
		convertUnsignedToSigned(_val, val);
	}

	void convert(UInt8& val) const override
	{
		convertToSmallerUnsigned(_val, val);
	}

	void convert(UInt16& val) const override
	{
		convertToSmallerUnsigned(_val, val);
	}

	void convert(UInt32& val) const override
	{
		convertToSmallerUnsigned(_val, val);
	}

	void convert(UInt64& val) const override
	{
		val = static_cast<UInt64>(_val);
	}

	void convert(bool& val) const override
	{
		val = (_val != 0);
	}

	void convert(float& val) const override
	{
		val = static_cast<float>(_val);
	}

	void convert(double& val) const override
	{
		val = static_cast<double>(_val);
	}

	void convert(char& val) const override
	{
		UInt8 tmp;
		convert(tmp);
		val = static_cast<char>(tmp);
	}

	void convert(std::string& val) const override
	{
		val = NumberFormatter::format(_val);
	}

	VarHolder* clone(Placeholder<VarHolder>* pVarHolder = 0) const override
	{
		return cloneHolder(pVarHolder, _val);
	}

	const unsigned long& value() const
	{
		return _val;
	}

	bool isArray() const override
	{
		return false;
	}

	bool isStruct() const override
	{
		return false;
	}

	bool isInteger() const override
	{
		return std::numeric_limits<unsigned long>::is_integer;
	}

	bool isSigned() const override
	{
		return std::numeric_limits<unsigned long>::is_signed;
	}

	bool isNumeric() const override
	{
		return std::numeric_limits<unsigned long>::is_specialized;
	}

	bool isBoolean() const override
	{
		return false;
	}

	bool isString() const override
	{
		return false;
	}

private:
	unsigned long _val;
};


#else // if defined (POCO_INT64_IS_LONG)


template <>
class VarHolderImpl<long long>: public VarHolder
{
public:
	VarHolderImpl(long long val): _val(val)
	{
	}

	~VarHolderImpl() override = default;

	VarHolderImpl() = delete;
	VarHolderImpl(const VarHolderImpl&) = delete;
	VarHolderImpl& operator=(const VarHolderImpl&) = delete;

	const std::type_info& type() const override
	{
		return typeid(long long);
	}

	void convert(Int8& val) const override
	{
		convertToSmaller(_val, val);
	}

	void convert(Int16& val) const override
	{
		convertToSmaller(_val, val);
	}

	void convert(Int32& val) const override
	{
		convertToSmaller(_val, val);
	}

	void convert(Int64& val) const override
	{
		val = static_cast<Int64>(_val);
	}

	void convert(UInt8& val) const override
	{
		convertSignedToUnsigned(_val, val);
	}

	void convert(UInt16& val) const override
	{
		convertSignedToUnsigned(_val, val);
	}

	void convert(UInt32& val) const override
	{
		convertSignedToUnsigned(_val, val);
	}

	void convert(UInt64& val) const override
	{
		convertSignedToUnsigned(_val, val);
	}

	void convert(long long& val) const override
	{
		val = _val;
	}

	void convert(unsigned long long& val) const override
	{
		convertSignedToUnsigned(_val, val);
	}

	void convert(bool& val) const override
	{
		val = (_val != 0);
	}

	void convert(float& val) const override
	{
		val = static_cast<float>(_val);
	}

	void convert(double& val) const override
	{
		val = static_cast<double>(_val);
	}

	void convert(char& val) const override
	{
		UInt8 tmp;
		convert(tmp);
		val = static_cast<char>(tmp);
	}

	void convert(std::string& val) const override
	{
		val = NumberFormatter::format(_val);
	}

	VarHolder* clone(Placeholder<VarHolder>* pVarHolder = nullptr) const override
	{
		return cloneHolder(pVarHolder, _val);
	}

	const long long& value() const
	{
		return _val;
	}

	bool isArray() const override
	{
		return false;
	}

	bool isStruct() const override
	{
		return false;
	}

	bool isInteger() const override
	{
		return std::numeric_limits<long long>::is_integer;
	}

	bool isSigned() const override
	{
		return std::numeric_limits<long long>::is_signed;
	}

	bool isNumeric() const override
	{
		return std::numeric_limits<long long>::is_specialized;
	}

	bool isBoolean() const override
	{
		return false;
	}

	bool isString() const override
	{
		return false;
	}

private:
	long long _val;
};


template <>
class VarHolderImpl<unsigned long long>: public VarHolder
{
public:
	VarHolderImpl(unsigned long long val): _val(val)
	{
	}

	~VarHolderImpl() override = default;

	VarHolderImpl() = delete;
	VarHolderImpl(const VarHolderImpl&) = delete;
	VarHolderImpl& operator=(const VarHolderImpl&) = delete;

	const std::type_info& type() const override
	{
		return typeid(unsigned long long);
	}

	void convert(Int8& val) const override
	{
		convertUnsignedToSigned(_val, val);
	}

	void convert(Int16& val) const override
	{
		convertUnsignedToSigned(_val, val);
	}

	void convert(Int32& val) const override
	{
		convertUnsignedToSigned(_val, val);
	}

	void convert(Int64& val) const override
	{
		convertUnsignedToSigned(_val, val);
	}

	void convert(UInt8& val) const override
	{
		convertToSmallerUnsigned(_val, val);
	}

	void convert(UInt16& val) const override
	{
		convertToSmallerUnsigned(_val, val);
	}

	void convert(UInt32& val) const override
	{
		convertToSmallerUnsigned(_val, val);
	}

	void convert(UInt64& val) const override
	{
		val = static_cast<UInt64>(_val);
	}

	void convert(long long& val) const override
	{
		convertUnsignedToSigned(_val, val);
	}

	void convert(unsigned long long& val) const override
	{
		val = _val;
	}

	void convert(bool& val) const override
	{
		val = (_val != 0);
	}

	void convert(float& val) const override
	{
		val = static_cast<float>(_val);
	}

	void convert(double& val) const override
	{
		val = static_cast<double>(_val);
	}

	void convert(char& val) const override
	{
		UInt8 tmp;
		convert(tmp);
		val = static_cast<char>(tmp);
	}

	void convert(std::string& val) const override
	{
		val = NumberFormatter::format(_val);
	}

	VarHolder* clone(Placeholder<VarHolder>* pVarHolder = nullptr) const override
	{
		return cloneHolder(pVarHolder, _val);
	}

	const unsigned long long& value() const
	{
		return _val;
	}

	bool isArray() const override
	{
		return false;
	}

	bool isStruct() const override
	{
		return false;
	}

	bool isInteger() const override
	{
		return std::numeric_limits<unsigned long long>::is_integer;
	}

	bool isSigned() const override
	{
		return std::numeric_limits<unsigned long long>::is_signed;
	}

	bool isNumeric() const override
	{
		return std::numeric_limits<unsigned long long>::is_specialized;
	}

	bool isBoolean() const override
	{
		return false;
	}

	bool isString() const override
	{
		return false;
	}

private:
	unsigned long long _val;
};


#endif // POCO_INT64_IS_LONG


template <typename T>
class VarHolderImpl<std::vector<T>>: public VarHolder
{
public:
	VarHolderImpl(const std::vector<T>& val): _val(val)
	{
	}

	~VarHolderImpl() override = default;

	VarHolderImpl() = delete;
	VarHolderImpl(const VarHolderImpl&) = delete;
	VarHolderImpl& operator=(const VarHolderImpl&) = delete;

	const std::type_info& type() const override
	{
		return typeid(std::vector<T>);
	}

	void convert(std::string& val) const override
	{
		Impl::containerToJSON(_val, val);
	}

	VarHolder* clone(Placeholder<VarHolder>* pVarHolder = nullptr) const override
	{
		return cloneHolder(pVarHolder, _val);
	}

	const std::vector<T>& value() const
	{
		return _val;
	}

	bool isVector() const override
	{
		return true;
	}

	std::size_t size() const override
	{
		return _val.size();
	}

	T& operator[](typename std::vector<T>::size_type n)
	{
		if (n < size()) return _val.operator[](n);

		throw RangeException("List index out of range");
	}

	const T& operator[](typename std::vector<T>::size_type n) const
	{
		if (n < size()) return _val.operator[](n);

		throw RangeException("List index out of range");
	}

private:
	std::vector<T> _val;
};


template <typename T>
class VarHolderImpl<std::list<T>>: public VarHolder
{
public:
	VarHolderImpl(const std::list<T>& val): _val(val)
	{
	}

	~VarHolderImpl() override = default;

	VarHolderImpl() = delete;
	VarHolderImpl(const VarHolderImpl&) = delete;
	VarHolderImpl& operator=(const VarHolderImpl&) = delete;

	const std::type_info& type() const override
	{
		return typeid(std::list<T>);
	}

	void convert(std::string& val) const override
	{
		Impl::containerToJSON(_val, val);
	}

	VarHolder* clone(Placeholder<VarHolder>* pVarHolder = nullptr) const override
	{
		return cloneHolder(pVarHolder, _val);
	}

	const std::list<T>& value() const
	{
		return _val;
	}

	bool isList() const override
	{
		return true;
	}

	std::size_t size() const override
	{
		return _val.size();
	}

	T& operator[](typename std::list<T>::size_type n)
	{
		if (n >= size())
			throw RangeException("List index out of range");

		typename std::list<T>::size_type counter = 0;
		typename std::list<T>::iterator it = _val.begin();
		for (; counter < n; ++counter) ++it;

		return *it;
	}

	const T& operator[](typename std::list<T>::size_type n) const
	{
		if (n >= size())
			throw RangeException("List index out of range");

		typename std::list<T>::size_type counter = 0;
		typename std::list<T>::const_iterator it = _val.begin();
		for (; counter < n; ++counter) ++it;

		return *it;
	}

private:
	std::list<T> _val;
};


template <typename T>
class VarHolderImpl<std::deque<T>>: public VarHolder
{
public:
	VarHolderImpl(const std::deque<T>& val): _val(val)
	{
	}

	~VarHolderImpl() override = default;

	VarHolderImpl() = delete;
	VarHolderImpl(const VarHolderImpl&) = delete;
	VarHolderImpl& operator=(const VarHolderImpl&) = delete;

	const std::type_info& type() const override
	{
		return typeid(std::deque<T>);
	}

	void convert(std::string& val) const override
	{
		Impl::containerToJSON(_val, val);
	}

	VarHolder* clone(Placeholder<VarHolder>* pVarHolder = nullptr) const override
	{
		return cloneHolder(pVarHolder, _val);
	}

	const std::deque<T>& value() const
	{
		return _val;
	}

	bool isDeque() const override
	{
		return true;
	}

	std::size_t size() const override
	{
		return _val.size();
	}

	T& operator[](typename std::deque<T>::size_type n)
	{
		if (n < size()) return _val.operator[](n);

		throw RangeException("List index out of range");
	}

	const T& operator[](typename std::deque<T>::size_type n) const
	{
		if (n < size()) return _val.operator[](n);

		throw RangeException("List index out of range");
	}

private:
	std::deque<T> _val;
};


template <>
class VarHolderImpl<DateTime>: public VarHolder
{
public:
	VarHolderImpl(const DateTime& val): _val(val)
	{
	}

	~VarHolderImpl() override = default;

	VarHolderImpl() = delete;
	VarHolderImpl(const VarHolderImpl&) = delete;
	VarHolderImpl& operator=(const VarHolderImpl&) = delete;

	const std::type_info& type() const override
	{
		return typeid(DateTime);
	}

	void convert(Int8 & /*val*/) const override
	{
		throw BadCastException();
	}

	void convert(Int16 & /*val*/) const override
	{
		throw BadCastException();
	}

	void convert(Int32 & /*val*/) const override
	{
		throw BadCastException();
	}

	void convert(Int64& val) const override
	{
		val = _val.timestamp().epochMicroseconds();
	}

	void convert(UInt64& val) const override
	{
		val = _val.timestamp().epochMicroseconds();
	}

#ifdef POCO_INT64_IS_LONG

	void convert(long long& val) const override
	{
		val = _val.timestamp().epochMicroseconds();
	}

	void convert(unsigned long long& val) const override
	{
		val = _val.timestamp().epochMicroseconds();
	}

#endif

	void convert(std::string& val) const override
	{
		val = DateTimeFormatter::format(_val, Poco::DateTimeFormat::ISO8601_FORMAT);
	}

	void convert(DateTime& val) const override
	{
		val = _val;
	}

	void convert(LocalDateTime &ldt) const override
	{
		ldt = _val.timestamp();
	}

	void convert(Timestamp &ts) const override
	{
		ts = _val.timestamp();
	}

	VarHolder* clone(Placeholder<VarHolder>* pVarHolder = nullptr) const override
	{
		return cloneHolder(pVarHolder, _val);
	}

	const DateTime& value() const
	{
		return _val;
	}

	bool isArray() const override
	{
		return false;
	}

	bool isStruct() const override
	{
		return false;
	}

	bool isInteger() const override
	{
		return false;
	}

	bool isSigned() const override
	{
		return false;
	}

	bool isNumeric() const override
	{
		return false;
	}

	bool isBoolean() const override
	{
		return false;
	}

	bool isString() const override
	{
		return false;
	}

	bool isDate() const override
	{
		return true;
	}

	bool isTime() const override
	{
		return true;
	}

	bool isDateTime() const override
	{
		return true;
	}

	bool isUUID() const override
	{
		return false;
	}

private:
	DateTime _val;
};


template <>
class VarHolderImpl<LocalDateTime>: public VarHolder
{
public:
	VarHolderImpl(const LocalDateTime& val): _val(val)
	{
	}

	~VarHolderImpl() override = default;

	VarHolderImpl() = delete;
	VarHolderImpl(const VarHolderImpl&) = delete;
	VarHolderImpl& operator=(const VarHolderImpl&) = delete;

	const std::type_info& type() const override
	{
		return typeid(LocalDateTime);
	}

	void convert(Int64& val) const override
	{
		val = _val.timestamp().epochMicroseconds();
	}

	void convert(UInt64& val) const override {
		val = _val.timestamp().epochMicroseconds();
	}

#ifdef POCO_INT64_IS_LONG

	void convert(long long& val) const override
	{
		val = _val.timestamp().epochMicroseconds();
	}

	void convert(unsigned long long& val) const override
	{
		val = _val.timestamp().epochMicroseconds();
	}

#endif

	void convert(std::string& val) const override
	{
		val = DateTimeFormatter::format(_val, Poco::DateTimeFormat::ISO8601_FORMAT);
	}

	void convert(DateTime& val) const override
	{
		val = _val.timestamp();
	}

	void convert(LocalDateTime &ldt) const override
	{
		ldt = _val;
	}

	void convert(Timestamp &ts) const override
	{
		ts = _val.timestamp();
	}

	VarHolder* clone(Placeholder<VarHolder>* pVarHolder = nullptr) const override
	{
		return cloneHolder(pVarHolder, _val);
	}

	const LocalDateTime& value() const
	{
		return _val;
	}

	bool isArray() const override
	{
		return false;
	}

	bool isStruct() const override
	{
		return false;
	}

	bool isInteger() const override
	{
		return false;
	}

	bool isSigned() const override
	{
		return false;
	}

	bool isNumeric() const override
	{
		return false;
	}

	bool isBoolean() const override
	{
		return false;
	}

	bool isString() const override
	{
		return false;
	}

	bool isDate() const override
	{
		return true;
	}

	bool isTime() const override
	{
		return true;
	}

	bool isDateTime() const override
	{
		return true;
	}

	bool isUUID() const override
	{
		return false;
	}

private:
	LocalDateTime _val;
};


template <>
class VarHolderImpl<Timestamp>: public VarHolder
{
public:
	VarHolderImpl(const Timestamp& val): _val(val)
	{
	}

	~VarHolderImpl() override = default;

	VarHolderImpl() = delete;
	VarHolderImpl(const VarHolderImpl&) = delete;
	VarHolderImpl& operator=(const VarHolderImpl&) = delete;

	const std::type_info& type() const override
	{
		return typeid(Timestamp);
	}

	void convert(Int64& val) const override
	{
		val = _val.epochMicroseconds();
	}

	void convert(UInt64& val) const override
	{
		val = _val.epochMicroseconds();
	}

#ifdef POCO_INT64_IS_LONG

	void convert(long long& val) const override
	{
		val = _val.epochMicroseconds();
	}

	void convert(unsigned long long& val) const override
	{
		val = _val.epochMicroseconds();
	}

#endif

	void convert(std::string& val) const override
	{
		val = DateTimeFormatter::format(_val, Poco::DateTimeFormat::ISO8601_FORMAT);
	}

	void convert(DateTime& val) const override
	{
		val = _val;
	}

	void convert(LocalDateTime &ldt) const override
	{
		ldt = _val;
	}

	void convert(Timestamp &ts) const override
	{
		ts = _val;
	}

	VarHolder* clone(Placeholder<VarHolder>* pVarHolder = nullptr) const override
	{
		return cloneHolder(pVarHolder, _val);
	}

	const Timestamp& value() const
	{
		return _val;
	}

	bool isArray() const override
	{
		return false;
	}

	bool isStruct() const override
	{
		return false;
	}

	bool isInteger() const override
	{
		return false;
	}

	bool isSigned() const override
	{
		return false;
	}

	bool isNumeric() const override
	{
		return false;
	}

	bool isBoolean() const override
	{
		return false;
	}

	bool isString() const override
	{
		return false;
	}

	bool isDate() const override
	{
		return true;
	}

	bool isTime() const override
	{
		return true;
	}

	bool isDateTime() const override
	{
		return true;
	}

	bool isUUID() const override
	{
		return false;
	}

private:
	Timestamp _val;
};


template <>
class VarHolderImpl<UUID>: public VarHolder
{
public:
	VarHolderImpl(const UUID& val): _val(val)
	{
	}

	~VarHolderImpl() override = default;

	VarHolderImpl() = delete;
	VarHolderImpl(const VarHolderImpl&) = delete;
	VarHolderImpl& operator=(const VarHolderImpl&) = delete;

	const std::type_info& type() const override
	{
		return typeid(UUID);
	}

	void convert(std::string& val) const override
	{
		val = _val.toString();
	}

	VarHolder* clone(Placeholder<VarHolder>* pVarHolder = nullptr) const override
	{
		return cloneHolder(pVarHolder, _val);
	}

	const UUID& value() const
	{
		return _val;
	}

	bool isArray() const override
	{
		return false;
	}

	bool isStruct() const override
	{
		return false;
	}

	bool isInteger() const override
	{
		return false;
	}

	bool isSigned() const override
	{
		return false;
	}

	bool isNumeric() const override
	{
		return false;
	}

	bool isBoolean() const override
	{
		return false;
	}

	bool isString() const override
	{
		return false;
	}

	bool isDate() const override
	{
		return false;
	}

	bool isTime() const override
	{
		return false;
	}

	bool isDateTime() const override
	{
		return false;
	}

	bool isUUID() const override
	{
		return true;
	}

private:
	Poco::UUID _val;
};


using Vector = std::vector<Var>;
using Deque = std::deque<Var>;
using List = std::list<Var>;
using Array = Vector;


} } // namespace Poco::Dynamic


#endif // Foundation_VarHolder_INCLUDED
