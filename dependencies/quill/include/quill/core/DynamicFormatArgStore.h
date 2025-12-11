/**
 * @page copyright
 * Copyright(c) 2020-present, Odysseas Georgoudis & quill contributors.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#include "quill/bundled/fmt/base.h"
#include "quill/core/Attributes.h"

QUILL_BEGIN_NAMESPACE

namespace detail
{
class DynamicArgList
{
  struct Node
  {
    virtual ~Node() = default;
    std::unique_ptr<Node> next;
  };

  template <typename T>
  struct TypedNode : Node
  {
    template <typename Arg>
    explicit TypedNode(Arg&& arg) : value(_construct(static_cast<Arg&&>(arg)))
    {
    }

  private:
    template <typename Arg>
    static T _construct(Arg&& arg)
    {
      if constexpr (std::is_move_constructible<T>::value)
      {
        return T(static_cast<Arg&&>(arg));
      }
      else
      {
        // For copy-only types: arg is a named rvalue reference, which is an lvalue expression.
        // This lvalue can bind to const T& (the copy constructor parameter), resulting in a copy.
        return T(arg);
      }
    }

  public:
    T value;
  };

  std::unique_ptr<Node> _head;

public:
  template <typename T, typename Arg>
  T const& push(Arg&& arg)
  {
    auto new_node = std::unique_ptr<TypedNode<T>>(new TypedNode<T>(static_cast<Arg&&>(arg)));
    T& value = new_node->value;
    new_node->next = static_cast<std::unique_ptr<Node>&&>(_head);
    _head = static_cast<std::unique_ptr<TypedNode<T>>&&>(new_node);
    return value;
  }
};
} // namespace detail

/**
 * Similar to fmt::dynamic_arg_store but better suited to our needs
 * e.g does not include <functional> and requires less space
 */
class DynamicFormatArgStore
{
private:
  // Storage of basic_format_arg must be contiguous.
  std::vector<fmtquill::basic_format_arg<fmtquill::format_context>> _data;

  // Storage of arguments not fitting into basic_format_arg must grow
  // without relocation because items in data_ refer to it.
  detail::DynamicArgList _dynamic_arg_list;
  bool _has_string_related_type{false};

  template <typename T>
  void emplace_arg(T&& arg)
  {
    _data.emplace_back(static_cast<T&&>(arg));
  }

public:
  DynamicFormatArgStore() = default;

  QUILL_NODISCARD int size() const { return static_cast<int>(_data.size()); }

  QUILL_NODISCARD fmtquill::basic_format_arg<fmtquill::format_context> const* data() const
  {
    return _data.data();
  }

  /**
    Adds an argument for later passing to a formatting function.

    **Example**::

      fmtquill::dynamic_format_arg_store<fmtquill::format_fmtquill::format_context> store;
      store.push_back(42);
      store.push_back("abc");
      store.push_back(1.5f);
      std::string result = fmtquill::vformat("{} and {} and {}", store);
  */
  template <typename T>
  void push_back(T&& arg)
  {
    using char_type = typename fmtquill::format_context::char_type;
    using bare_type = std::remove_cv_t<std::remove_reference_t<T>>;
    constexpr auto mapped_type = fmtquill::detail::mapped_type_constant<bare_type, char_type>::value;
    using stored_type =
      std::conditional_t<std::is_convertible_v<bare_type, std::string>, std::string, bare_type>;

    if constexpr (!(std::is_same_v<bare_type, std::string_view> || std::is_same_v<bare_type, fmtquill::string_view> ||
                    (mapped_type != fmtquill::detail::type::cstring_type &&
                     mapped_type != fmtquill::detail::type::string_type &&
                     mapped_type != fmtquill::detail::type::custom_type)))
    {
      emplace_arg(_dynamic_arg_list.push<stored_type>(static_cast<T&&>(arg)));
    }
    else
    {
      emplace_arg(static_cast<T&&>(arg));
    }

    if constexpr (std::is_same_v<bare_type, std::string_view> || std::is_same_v<bare_type, fmtquill::string_view> ||
                  (mapped_type == fmtquill::detail::type::cstring_type) ||
                  (mapped_type == fmtquill::detail::type::string_type) ||
                  (mapped_type == fmtquill::detail::type::custom_type) ||
                  (mapped_type == fmtquill::detail::type::char_type))
    {
      _has_string_related_type = true;
    }
  }

  /** Erase all elements from the store */
  void clear()
  {
    _data.clear();
    _dynamic_arg_list = detail::DynamicArgList{};
    _has_string_related_type = false;
  }

  QUILL_NODISCARD bool has_string_related_type() const noexcept { return _has_string_related_type; }
};

QUILL_END_NAMESPACE