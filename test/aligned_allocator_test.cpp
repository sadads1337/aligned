#include <gtest/gtest.h>

#include <aligned/aligned_allocator>

#include "utils.h"

using namespace aligned;

namespace {

template <typename T>
class aligned_allocator_test : public ::testing::Test {};

template <typename T, alignment Align>
inline auto test_address_aligment(aligned_allocator<T, Align>& allocator) {
  check_align<Align>();
  constexpr auto align = static_cast<std::size_t>(Align);

  constexpr auto size = static_cast<std::size_t>(1u);
  auto* const ptr = allocator.allocate(size);
  ASSERT_TRUE(ptr);
  EXPECT_TRUE(is_aligned(ptr, align));

  allocator.deallocate(ptr, size);
}

template <typename T, alignment Align>
inline auto test_max_size(aligned_allocator<T, Align>& allocator) {
  check_align<Align>();
  constexpr auto align = static_cast<std::size_t>(Align);

  const auto max_size = allocator.max_size();
  const auto expected_max_size =
      (std::numeric_limits<std::size_t>::max() - align) / sizeof(T);
  EXPECT_EQ(max_size, expected_max_size);
}

template <typename T, alignment Align>
inline auto test_address(aligned_allocator<T, Align>& allocator) {
  check_align<Align>();
  constexpr auto align = static_cast<std::size_t>(Align);

  constexpr auto size = static_cast<std::size_t>(1u);
  auto* const ptr = allocator.allocate(size);
  ASSERT_TRUE(ptr);
  ASSERT_TRUE(is_aligned(ptr, align));
  const auto& const_ref = *ptr;
  const auto* const expected_addr = std::addressof(const_ref);
  const auto* const addr = allocator.address(const_ref);
  EXPECT_EQ(addr, expected_addr);

  allocator.deallocate(ptr, size);
}

}  // namespace

TEST(aligned_allocator_test, check_normal_aligment) {
  constexpr auto align = static_cast<std::size_t>(alignment::normal);
  EXPECT_EQ(align, static_cast<std::size_t>(sizeof(void*)));
}

TEST(aligned_allocator_test, check_sse_aligment) {
  constexpr auto align = static_cast<std::size_t>(alignment::sse);
  EXPECT_EQ(align, static_cast<std::size_t>(16u));
}

TEST(aligned_allocator_test, check_avx_aligment) {
  constexpr auto align = static_cast<std::size_t>(alignment::avx);
  EXPECT_EQ(align, static_cast<std::size_t>(32u));
}

TEST(aligned_allocator_test, check_eq_operator) {
  {
    aligned_allocator<int> allocator;
    aligned_allocator<float> other_allocator;
    EXPECT_EQ(allocator, other_allocator);
  }
  {
    aligned_allocator<int> allocator;
    aligned_allocator<int> other_allocator;
    EXPECT_EQ(allocator, other_allocator);
  }
}

TEST(aligned_allocator_test, check_neq_operator) {
  {
    aligned_allocator<int, alignment::normal> allocator;
    aligned_allocator<float, alignment::avx> other_allocator;
    EXPECT_NE(allocator, other_allocator);
  }
  {
    aligned_allocator<int, alignment::normal> allocator;
    aligned_allocator<int, alignment::avx> other_allocator;
    EXPECT_NE(allocator, other_allocator);
  }
}

TYPED_TEST_CASE(aligned_allocator_test, testing_types);

TYPED_TEST(aligned_allocator_test, check_default_constructor_nothrow) {
  EXPECT_NO_THROW((aligned_allocator<TypeParam>{}));
}

TYPED_TEST(aligned_allocator_test, check_copy_constructor_nothrow) {
  {
    auto allocator = aligned_allocator<TypeParam>{};
    EXPECT_NO_THROW((allocator = aligned_allocator<TypeParam>{}));
  }
  {
    auto allocator = aligned_allocator<TypeParam>{};
    EXPECT_NO_THROW((allocator = aligned_allocator<int>{}));
  }
}

TYPED_TEST(aligned_allocator_test, check_normal_allocation_throw) {
  aligned_allocator<TypeParam, alignment::normal> allocator;
  TypeParam* ptr = nullptr;
  EXPECT_ANY_THROW(
      (ptr = allocator.allocate(std::numeric_limits<std::size_t>::max())));
}

TYPED_TEST(aligned_allocator_test, check_sse_allocation_throw) {
  aligned_allocator<TypeParam, alignment::sse> allocator;
  TypeParam* ptr = nullptr;
  EXPECT_ANY_THROW(
      (ptr = allocator.allocate(std::numeric_limits<std::size_t>::max())));
}

TYPED_TEST(aligned_allocator_test, check_avx_allocation_throw) {
  aligned_allocator<TypeParam, alignment::avx> allocator;
  TypeParam* ptr = nullptr;
  EXPECT_ANY_THROW(
      (ptr = allocator.allocate(std::numeric_limits<std::size_t>::max())));
}

TYPED_TEST(aligned_allocator_test, check_normal_address_aligment) {
  aligned_allocator<TypeParam, alignment::normal> allocator;
  test_address_aligment(allocator);
}

TYPED_TEST(aligned_allocator_test, check_sse_address_aligment) {
  aligned_allocator<TypeParam, alignment::sse> allocator;
  test_address_aligment(allocator);
}

TYPED_TEST(aligned_allocator_test, check_avx_address_aligment) {
  aligned_allocator<TypeParam, alignment::avx> allocator;
  test_address_aligment(allocator);
}

TYPED_TEST(aligned_allocator_test, check_normal_max_size) {
  aligned_allocator<TypeParam, alignment::normal> allocator;
  test_max_size(allocator);
}

TYPED_TEST(aligned_allocator_test, check_sse_max_size) {
  aligned_allocator<TypeParam, alignment::sse> allocator;
  test_max_size(allocator);
}

TYPED_TEST(aligned_allocator_test, check_avx_max_size) {
  aligned_allocator<TypeParam, alignment::avx> allocator;
  test_max_size(allocator);
}

TYPED_TEST(aligned_allocator_test, check_normal_address) {
  aligned_allocator<TypeParam, alignment::normal> allocator;
  test_address(allocator);
}

TYPED_TEST(aligned_allocator_test, check_sse_address) {
  aligned_allocator<TypeParam, alignment::sse> allocator;
  test_address(allocator);
}

TYPED_TEST(aligned_allocator_test, check_avx_address) {
  aligned_allocator<TypeParam, alignment::avx> allocator;
  test_address(allocator);
}
