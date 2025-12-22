/**
 * @page copyright
 * Copyright(c) 2020-present, Odysseas Georgoudis & quill contributors.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#pragma once

#include "quill/core/Attributes.h"
#include "quill/core/Codec.h"
#include "quill/core/DynamicFormatArgStore.h"
#include "quill/core/InlinedVector.h"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>
#include <type_traits>

QUILL_BEGIN_NAMESPACE

/**
 * @brief A non-owning view over binary data with type tagging support
 *
 * This class provides a lightweight wrapper around a pointer to binary data and its size.
 * The template parameter T is used as a tag to give semantic meaning to the binary data
 */
template <typename T>
class BinaryData
{
public:
  using ValueType = T;

  /**
   * @brief Template constructor that accepts any pointer type that can be safely
   * reinterpreted as binary data
   *
   * @param data Pointer to the binary data
   * @param size Size of the binary data in bytes
   *
   * @note If size exceeds uint32_t max value, it will be capped
   */
  template <typename U>
  BinaryData(U const* data, size_t size)
    : _data{reinterpret_cast<std::byte const*>(data)},
      _size{size > std::numeric_limits<uint32_t>::max() ? std::numeric_limits<uint32_t>::max()
                                                        : static_cast<uint32_t>(size)}
  {
    static_assert(sizeof(U) == 1, "BinaryData only accepts byte-sized element types");
  }

  /**
   * @brief Get the raw binary data pointer
   * @return Pointer to the binary data
   */
  QUILL_NODISCARD std::byte const* data() const noexcept { return _data; }

  /**
   * @brief Get the size of the binary data
   * @return Size in bytes
   */
  QUILL_NODISCARD uint32_t size() const noexcept { return _size; }

private:
  template <typename U>
  friend struct BinaryDataDeferredFormatCodec;

  std::byte const* _data{nullptr};
  uint32_t _size{0};
};

/**
 * @brief Codec for efficient binary data logging with deferred formatting
 *
 * This codec allows efficient logging of binary data by:
 * 1. Copying the raw binary data to the log queue on the hot path (critical section)
 * 2. Deferring the actual formatting to the backend thread (non-critical section)
 *
 * Since the library always writes to human-readable log files, binary data must be formatted
 * appropriately in the formatter implementation :
 *
 * 1. Parse the binary data into a meaningful text representation
 *    (e.g., decode a protocol message into its fields)
 *
 * 2. Convert the binary data to a hexadecimal representation using
 *    quill::utility::to_hex() for readability
 *
 * Example:
 * ```cpp
 * // 1. Create a tag struct for your binary protocol
 * struct TradingProtocol { };
 *
 * // 2. Create a type alias for your binary data
 * using TradingProtocolData = quill::BinaryData<TradingProtocol>;
 *
 * // 3. Implement a formatter for your type
 * template <>
 * struct fmtquill::formatter<TradingProtocolData>
 * {
 *   constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
 *
 *   auto format(::TradingProtocolData const& binary_data, format_context& ctx) const
 *   {
 *     // Format binary data as needed, e.g. parse protocol messages or convert to hex
 *     return fmtquill::format_to(ctx.out(), "{}",
 *            quill::utility::to_hex(binary_data.data(), binary_data.size()));
 *   }
 * };
 *
 * // 4. Specialize the codec for your type
 * template <>
 * struct quill::Codec<TradingProtocolData> : quill::BinaryDataDeferredFormatCodec<TradingProtocolData>
 * {
 * };
 *
 * // Now you can log binary data efficiently
 * LOG_INFO(logger, "Binary message: {}", TradingProtocolData{data_ptr, data_size});
 * ```
 */

template <typename T>
struct BinaryDataDeferredFormatCodec
{
  static_assert(std::is_same_v<BinaryData<typename T::ValueType>, T>,
                "BinaryDataDeferredFormatCodec can only be used with BinaryData");

  static size_t compute_encoded_size(detail::SizeCacheVector&, T const& arg)
  {
    return sizeof(uint32_t) + arg._size;
  }

  static void encode(std::byte*& buffer, detail::SizeCacheVector const&, uint32_t, T const& arg)
  {
    std::byte* buf_ptr = buffer;

    std::memcpy(buf_ptr, &arg._size, sizeof(uint32_t));
    buf_ptr += sizeof(uint32_t);

    if (QUILL_LIKELY((arg._data && arg._size > 0)))
    {
      std::memcpy(buf_ptr, arg._data, arg._size);
      buf_ptr += arg._size;
    }

    buffer = buf_ptr;
  }

  static T decode_arg(std::byte*& buffer)
  {
    uint32_t size;
    std::memcpy(&size, buffer, sizeof(uint32_t));
    buffer += sizeof(uint32_t);

    T binary_data_ref{buffer, size};
    buffer += size;
    return binary_data_ref;
  }

  static void decode_and_store_arg(std::byte*& buffer, DynamicFormatArgStore* args_store)
  {
    args_store->push_back(decode_arg(buffer));
  }
};

QUILL_END_NAMESPACE