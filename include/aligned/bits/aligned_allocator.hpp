#pragma once

#include <cassert>
#include <cstdint>
#include <limits>

#include "aligned_allocator.h"

namespace aligned {
namespace detail {
inline void* allocate_aligned(const std::size_t size, const std::size_t align) {
  assert(align >= sizeof(void*));
  if (size == static_cast<std::size_t>(0u)) {
    return nullptr;
  }
  void* ptr = nullptr;
  const auto rc = posix_memalign(&ptr, align, size);
  return rc ? nullptr : ptr;
}

inline auto deallocate_aligned(void* ptr) { return free(ptr); }
}  // namespace detail
inline namespace cxx17_v1 {

template <typename T, alignment Align>
[[nodiscard]] inline auto aligned_allocator<T, Align>::max_size()
    const noexcept {
  constexpr auto max = (std::numeric_limits<size_type>::max() -
                        size_type{static_cast<size_t>(Align)}) /
                       sizeof(T);
  static_assert(max < std::numeric_limits<size_type>::max());
  return static_cast<size_type>(max);
}

template <typename T, alignment Align>
[[nodiscard]] inline typename aligned_allocator<T, Align>::pointer
aligned_allocator<T, Align>::address(reference x) const noexcept {
  return std::addressof(x);
}

template <typename T, alignment Align>
[[nodiscard]] inline typename aligned_allocator<T, Align>::const_pointer
aligned_allocator<T, Align>::address(
    typename aligned_allocator<T, Align>::const_reference x) const noexcept {
  return std::addressof(x);
}

template <typename T, alignment Align>
[[nodiscard]] inline typename aligned_allocator<T, Align>::pointer
aligned_allocator<T, Align>::allocate(
    const typename aligned_allocator<T, Align>::size_type n) {
  if (max_size() < n) {
    throw std::bad_array_new_length{};
  }
  auto* ptr = detail::allocate_aligned(n * sizeof(value_type), align);
  if (!ptr) {
    throw std::bad_alloc{};
  }
  return reinterpret_cast<pointer>(ptr);
}

template <typename T, alignment Align>
[[nodiscard]] inline typename aligned_allocator<T, Align>::pointer
aligned_allocator<T, Align>::allocate(
    [[maybe_unused]]const typename aligned_allocator<T, Align>::size_type n,
    [[maybe_unused]] const void* hint) {
  //! Deprecated
  assert(false);
}

template <typename T, alignment Align>
inline auto aligned_allocator<T, Align>::deallocate(
    typename aligned_allocator<T, Align>::pointer ptr,
    [[maybe_unused]] const typename aligned_allocator<T, Align>::size_type n) {
  detail::deallocate_aligned(ptr);
}

template <typename T, alignment Align>
template <typename U, typename... Args>
inline auto aligned_allocator<T, Align>::construct(U* ptr, Args&&... args) {
  ::new (reinterpret_cast<void*>(ptr)) U(std::forward<Args>(args)...);
}

template <typename T, alignment Align>
template <class U>
auto aligned_allocator<T, Align>::destroy(U* ptr) {
  ptr->~U();
}

template <typename T, alignment TAlign, typename U, alignment UAlign>
inline auto operator==(const aligned_allocator<T, TAlign>&,
                       const aligned_allocator<U, UAlign>&) noexcept {
  return TAlign == UAlign;
}

template <typename T, alignment TAlign, typename U, alignment UAlign>
inline auto operator!=(const aligned_allocator<T, TAlign>&,
                       const aligned_allocator<U, UAlign>&) noexcept {
  return !(TAlign == UAlign);
}

}  // namespace cxx17_v1
}  // namespace aligned