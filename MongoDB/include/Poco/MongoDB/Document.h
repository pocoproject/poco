//
// Document.h
//
// Library: MongoDB
// Package: MongoDB
// Module:  Document
//
// Definition of the Document class.
//
// Copyright (c) 2012-2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef MongoDB_Document_INCLUDED
#define MongoDB_Document_INCLUDED


#include "Poco/BinaryReader.h"
#include "Poco/BinaryWriter.h"
#include "Poco/MongoDB/MongoDB.h"
#include "Poco/MongoDB/Element.h"
#include <algorithm>
#include <cstdlib>
#include <unordered_map>
#include <type_traits>


namespace Poco {
namespace MongoDB {

class Array;

class MongoDB_API ElementFindByName
{
public:
	ElementFindByName(const std::string& name):
		_name(name)
	{
	}

	bool operator()(const Element::Ptr& element)
	{
		return !element.isNull() && element->name() == _name;
	}

private:
	std::string _name;
};


class MongoDB_API Document
	/// Represents a MongoDB (BSON) document.
	///
	/// THREAD SAFETY:
	/// This class is NOT thread-safe. Document instances must not be accessed
	/// concurrently from multiple threads without external synchronization.
	/// Concurrent modifications to the element list will cause undefined behavior.
	///
	/// Each thread should use its own Document instances.
{
public:
	using Ptr = SharedPtr<Document>;
	using Vector = std::vector<Document::Ptr>;

	Document();
		/// Creates an empty Document.

	virtual ~Document();
		/// Destroys the Document.

	Document& addElement(Element::Ptr element);
		/// Add an element to the document.
		///
		/// The active document is returned to allow chaining of the add methods.

	template<typename T>
	Document& add(const std::string& name, T value)
		/// Creates an element with the given name and value and
		/// adds it to the document.
		///
		/// The active document is returned to allow chaining of the add methods.
	{
		return addElement(new ConcreteElement<T>(name, value));
	}

	template<typename T>
	typename std::enable_if<!std::is_same<typename std::decay<T>::type, const char*>::value, Document&>::type
	add(std::string&& name, T value)
		/// Creates an element with the given name (moved) and value and
		/// adds it to the document. Move semantics for efficiency.
		/// Disabled for const char* to avoid ambiguity.
		///
		/// The active document is returned to allow chaining of the add methods.
	{
		return addElement(new ConcreteElement<T>(std::move(name), value));
	}

	Document& add(const std::string& name, const char* value)
		/// Creates an element with the given name and value and
		/// adds it to the document.
		///
		/// The active document is returned to allow chaining of the add methods.
	{
		return addElement(new ConcreteElement<std::string>(name, std::string(value)));
	}

	Document& addNewDocument(const std::string& name);
		/// Create a new document and add it to this document.
		/// Unlike the other add methods, this method returns
		/// a reference to the new document.

	Array& addNewArray(const std::string& name);
		/// Create a new array and add it to this document.
		/// Method returns a reference to the new array.

	void clear() noexcept;
		/// Removes all elements from the document.

	void elementNames(std::vector<std::string>& keys) const;
		/// Puts all element names into std::vector.

	[[nodiscard]] bool empty() const noexcept;
		/// Returns true if the document doesn't contain any documents.

	[[nodiscard]] bool exists(const std::string& name) const noexcept;
		/// Returns true if the document has an element with the given name.

	template<typename T>
	const T& get(const std::string& name) const
		/// Returns the element with the given name and tries to convert
		/// it to the template type. When the element is not found, a
		/// NotFoundException will be thrown. When the element can't be
		/// converted a BadCastException will be thrown.
	{
		Element::Ptr element = get(name);
		if (element.isNull())
		{
			throw NotFoundException(name);
		}
		else
		{
			if (ElementTraits<T>::TypeId == element->type())
			{
				auto* concrete = dynamic_cast<ConcreteElement<T>* >(element.get());
				if (concrete != nullptr)
				{
					return concrete->value();
				}
			}
			throw BadCastException("Invalid type mismatch!");
		}
	}

	template<typename T>
	const T& get(const std::string& name, const T& def) const
		/// Returns the element with the given name and tries to convert
		/// it to the template type. When the element is not found, or
		/// has the wrong type, the def argument will be returned.
	{
		Element::Ptr element = get(name);
		if (element.isNull())
		{
			return def;
		}

		if (ElementTraits<T>::TypeId == element->type())
		{
			auto* concrete = dynamic_cast<ConcreteElement<T>* >(element.get());
			if (concrete != nullptr)
			{
				return concrete->value();
			}
		}

		return def;
	}

	[[nodiscard]] Element::Ptr get(const std::string& name) const;
		/// Returns the element with the given name.
		/// An empty element will be returned when the element is not found.

	[[nodiscard]] Int64 getInteger(const std::string& name) const;
		/// Returns an integer. Useful when MongoDB returns Int32, Int64
		/// or double for a number (count for example). This method will always
		/// return an Int64. When the element is not found, a
		/// Poco::NotFoundException will be thrown.

	bool remove(const std::string& name);
		/// Removes an element from the document.

	template<typename T>
	bool isType(const std::string& name) const
		/// Returns true when the type of the element equals the TypeId of ElementTrait.
	{
		Element::Ptr element = get(name);
		if (element.isNull())
		{
			return false;
		}

		return ElementTraits<T>::TypeId == element->type();
	}

	void read(BinaryReader& reader);
		/// Reads a document from the reader

	[[nodiscard]] std::size_t size() const noexcept;
		/// Returns the number of elements in the document.

	[[nodiscard]] virtual std::string toString(int indent = 0) const;
		/// Returns a String representation of the document.

	void write(BinaryWriter& writer);
		/// Writes a document to the reader

protected:
	const ElementSet& elements() const noexcept;
		/// Returns const reference to elements for read-only access by derived classes.
		/// Direct modification is not allowed to maintain synchronization with hash map.

private:
	ElementSet _elements;
	std::unordered_map<std::string, Element::Ptr> _elementMap;
		/// Hash map for O(1) element lookups by name.
		/// Maintained in sync with _elements for fast access.
		/// These are private to ensure derived classes cannot break synchronization.
};


//
// inlines
//
inline Document& Document::addElement(Element::Ptr element)
{
	_elements.push_back(element);
	_elementMap[element->name()] = element;  // O(1) insert for fast lookups
	return *this;
}


inline Document& Document::addNewDocument(const std::string& name)
{
	Document::Ptr newDoc = new Document();
	add(name, newDoc);
	return *newDoc;
}


inline void Document::clear() noexcept
{
	_elements.clear();
	_elementMap.clear();
}


inline bool Document::empty() const noexcept
{
	return _elements.empty();
}


inline void Document::elementNames(std::vector<std::string>& keys) const
{
	keys.reserve(keys.size() + _elements.size());  // Pre-allocate to avoid reallocations
	for (const auto & _element : _elements)
	{
		keys.push_back(_element->name());
	}
}


inline bool Document::exists(const std::string& name) const noexcept
{
	// O(1) lookup using hash map instead of O(n) linear search
	return _elementMap.find(name) != _elementMap.end();
}


inline bool Document::remove(const std::string& name)
{
	// Remove from hash map first (O(1))
	auto mapIt = _elementMap.find(name);
	if (mapIt == _elementMap.end())
		return false;

	_elementMap.erase(mapIt);

	// Then remove from vector (O(n) but unavoidable for order preservation)
	auto it = std::find_if(_elements.begin(), _elements.end(), ElementFindByName(name));
	if (it != _elements.end())
		_elements.erase(it);

	return true;
}


inline std::size_t Document::size() const noexcept
{
	return _elements.size();
}


inline const ElementSet& Document::elements() const noexcept
{
	return _elements;
}


// BSON Embedded Document
// spec: document
template<>
struct ElementTraits<Document::Ptr>
{
	enum { TypeId = 0x03 };

	static std::string toString(const Document::Ptr& value, int indent = 0)
	{
		return value.isNull() ? "null" : value->toString(indent);
	}
};


template<>
inline void BSONReader::read<Document::Ptr>(Document::Ptr& to)
{
	to->read(_reader);
}


template<>
inline void BSONWriter::write<Document::Ptr>(Document::Ptr& from)
{
	from->write(_writer);
}


} } // namespace Poco::MongoDB


#endif // MongoDB_Document_INCLUDED
