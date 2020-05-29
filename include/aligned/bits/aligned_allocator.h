#pragma once

#include <cstdint>

namespace aligned {

namespace detail {
static void* allocate_aligned(std::size_t size, std::size_t align);

static auto deallocate_aligned(void* ptr);
}  // namespace detail

inline namespace cxx17_v1 {

enum class alignment : std::size_t {
  normal = static_cast<std::size_t>(sizeof(void*)),
  sse = static_cast<std::size_t>(16u),
  avx = static_cast<std::size_t>(32u),
};

template <typename T, alignment Align = alignment::normal>
class aligned_allocator;

template <alignment Align>
class aligned_allocator<void, Align> {
 public:
  using pointer = void*;
  using const_pointer = const void*;
  using value_type = void;

  template <class U>
  struct rebind {
    using other = aligned_allocator<U, Align>;
  };
};

template <typename T, alignment Align>
class aligned_allocator {
 public:
  using pointer = T*;
  using const_pointer = const T*;
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;

  using propagate_on_container_move_assignment = std::true_type;
  using is_always_equal = std::true_type;

  static constexpr auto align = static_cast<size_type>(Align);

  template <class U>
  struct rebind {
    using other = aligned_allocator<U, Align>;
  };

 public:
  aligned_allocator() noexcept {};

  template <typename U>
  aligned_allocator(const aligned_allocator<U, Align>& other) noexcept {};

  [[nodiscard]] auto max_size() const noexcept;

  [[nodiscard]] pointer address(reference x) const noexcept;

  [[nodiscard]] const_pointer address(const_reference x) const noexcept;

  [[nodiscard]] pointer allocate(const size_type n);

  [[nodiscard]] pointer allocate(const size_type n, const void* hint);

  auto deallocate(pointer ptr, [[maybe_unused]] const size_type n);

  template <typename U, typename... Args>
  auto construct(U* ptr, Args&&... args);

  template <class U>
  auto destroy(U* ptr);
};

template <typename T, alignment TAlign, typename U, alignment UAlign>
auto operator==(const aligned_allocator<T, TAlign>&,
                const aligned_allocator<U, UAlign>&) noexcept;

template <typename T, alignment TAlign, typename U, alignment UAlign>
auto operator!=(const aligned_allocator<T, TAlign>&,
                const aligned_allocator<U, UAlign>&) noexcept;

}  // namespace cxx17_v1
}  // namespace aligned

#include "aligned_allocator.hpp"
