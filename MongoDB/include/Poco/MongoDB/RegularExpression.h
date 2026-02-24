//
// RegularExpression.h
//
// Library: MongoDB
// Package: MongoDB
// Module:  RegularExpression
//
// Definition of the RegularExpression class.
//
// Copyright (c) 2012-2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef MongoDB_RegularExpression_INCLUDED
#define MongoDB_RegularExpression_INCLUDED


#include "Poco/RegularExpression.h"
#include "Poco/MongoDB/MongoDB.h"
#include "Poco/MongoDB/Element.h"


namespace Poco {
namespace MongoDB {


class MongoDB_API RegularExpression
	/// Represents a regular expression in BSON format.
{
public:
	using Ptr = SharedPtr<RegularExpression>;

	RegularExpression();
		/// Creates an empty RegularExpression.

	RegularExpression(const std::string& pattern, const std::string& options);
		/// Creates a RegularExpression using the given pattern and options.

	RegularExpression(std::string&& pattern, std::string&& options);
		/// Creates a RegularExpression using the given pattern and options (move semantics).

	RegularExpression(const RegularExpression& copy) = default;
		/// Creates a RegularExpression by copying another one.

	RegularExpression(RegularExpression&& other) noexcept = default;
		/// Creates a RegularExpression by moving another one.

	RegularExpression& operator=(const RegularExpression& copy) = default;
		/// Assigns another RegularExpression.

	RegularExpression& operator=(RegularExpression&& other) noexcept = default;
		/// Move-assigns another RegularExpression.

	virtual ~RegularExpression();
		/// Destroys the RegularExpression.

	[[nodiscard]] SharedPtr<Poco::RegularExpression> createRE() const;
		/// Tries to create a Poco::RegularExpression from the MongoDB regular expression.

	[[nodiscard]] const std::string& getOptions() const noexcept;
		/// Returns the options string.

	void setOptions(const std::string& options);
		/// Sets the options string.

	void setOptions(std::string&& options) noexcept;
		/// Sets the options string (move semantics).

	[[nodiscard]] const std::string& getPattern() const noexcept;
		/// Returns the pattern.

	void setPattern(const std::string& pattern);
		/// Sets the pattern.

	void setPattern(std::string&& pattern) noexcept;
		/// Sets the pattern (move semantics).

private:
	std::string _pattern;
	std::string _options;
};


///
/// inlines
///
inline const std::string& RegularExpression::getPattern() const noexcept
{
	return _pattern;
}


inline void RegularExpression::setPattern(const std::string& pattern)
{
	_pattern = pattern;
}


inline void RegularExpression::setPattern(std::string&& pattern) noexcept
{
	_pattern = std::move(pattern);
}


inline const std::string& RegularExpression::getOptions() const noexcept
{
	return _options;
}


inline void RegularExpression::setOptions(const std::string& options)
{
	_options = options;
}


inline void RegularExpression::setOptions(std::string&& options) noexcept
{
	_options = std::move(options);
}


// BSON Regex
// spec: cstring cstring
template<>
struct ElementTraits<RegularExpression::Ptr>
{
	enum { TypeId = 0x0B };

	static std::string toString(const RegularExpression::Ptr& value, int indent = 0)
	{
		if (value.isNull())
		{
			return "null";
		}

		// Format as /pattern/options similar to MongoDB shell, quoted for valid JSON
		std::string formatted;
		formatted.reserve(value->getPattern().size() + value->getOptions().size() + 2);
		formatted += '/';
		formatted += value->getPattern();
		formatted += '/';
		formatted += value->getOptions();
		return ElementTraits<std::string>::toString(formatted, indent);
	}
};


template<>
inline void BSONReader::read<RegularExpression::Ptr>(RegularExpression::Ptr& to)
{
	std::string pattern = readCString();
	std::string options = readCString();

	to = new RegularExpression(pattern, options);
}


template<>
inline void BSONWriter::write<RegularExpression::Ptr>(const RegularExpression::Ptr& from)
{
	writeCString(from->getPattern());
	writeCString(from->getOptions());
}


} } // namespace Poco::MongoDB


#endif // MongoDB_RegularExpression_INCLUDED
