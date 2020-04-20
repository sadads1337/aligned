#include <gtest/gtest.h>

#include <aligned/aligned_vector>

#include "utils.h"

using namespace aligned;

namespace {

template <typename T>
class aligned_vector_test : public ::testing::Test {};

}  // namespace

TYPED_TEST_CASE(aligned_vector_test, testing_types);

TYPED_TEST(aligned_vector_test, construct_normal) {
  aligned_vector<TypeParam, alignment::normal>{};
}

TYPED_TEST(aligned_vector_test, construct_sse) {
  aligned_vector<TypeParam, alignment::sse>{};
}

TYPED_TEST(aligned_vector_test, construct_avx) {
  aligned_vector<TypeParam, alignment::avx>{};
}

TYPED_TEST(aligned_vector_test, push_back_normal) {
  aligned_vector<TypeParam, alignment::normal> vector;
  vector.push_back(TypeParam{});
}

TYPED_TEST(aligned_vector_test, push_back_sse) {
  aligned_vector<TypeParam, alignment::sse> vector;
  vector.push_back(TypeParam{});
}

TYPED_TEST(aligned_vector_test, push_back_avx) {
  aligned_vector<TypeParam, alignment::avx> vector;
  vector.push_back(TypeParam{});
}

TYPED_TEST(aligned_vector_test, emplace_back_normal) {
  aligned_vector<TypeParam, alignment::normal> vector;
  vector.emplace_back(TypeParam{});
}

TYPED_TEST(aligned_vector_test, emplace_back_sse) {
  aligned_vector<TypeParam, alignment::sse> vector;
  vector.emplace_back(TypeParam{});
}

TYPED_TEST(aligned_vector_test, emplace_back_avx) {
  aligned_vector<TypeParam, alignment::avx> vector;
  vector.emplace_back(TypeParam{});
}
