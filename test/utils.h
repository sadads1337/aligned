#pragma once

#include <gtest/gtest.h>

#include <aligned/aligned_allocator>

using namespace aligned;

using testing_types = ::testing::Types<
    std::int8_t, std::int16_t, std::int32_t, std::int64_t, std::uint8_t,
    std::uint16_t, std::uint32_t, std::uint64_t, const std::int8_t,
    const std::int16_t, const std::int32_t, const std::int64_t,
    const std::uint8_t, const std::uint16_t, const std::uint32_t,
    const std::uint64_t, float, double, const float, const double>;

using testing_types_without_const =
    ::testing::Types<std::int8_t, std::int16_t, std::int32_t, std::int64_t,
                     std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t,
                     float, double>;

template <typename T>
[[nodiscard]] inline auto is_aligned(const T* const ptr,
                                     const std::size_t align) noexcept {
  return !(reinterpret_cast<std::uintptr_t>(ptr) % align);
}

template <alignment Align>
inline auto check_align() {
  constexpr auto align = static_cast<std::size_t>(Align);
  switch (Align) {
    case alignment::normal:
      ASSERT_EQ(align, static_cast<std::size_t>(sizeof(void*)));
      break;
    case alignment::sse:
      ASSERT_EQ(align, static_cast<std::size_t>(16u));
      break;
    case alignment::avx:
      ASSERT_EQ(align, static_cast<std::size_t>(32u));
      break;
    default:
      assert(false);
  }
}
