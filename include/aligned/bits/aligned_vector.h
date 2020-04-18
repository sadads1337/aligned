#pragma once

#include <vector>

#include "aligned_allocator.h"

namespace aligned {

template <typename T, alignment Align = alignment::normal>
using aligned_vector = std::vector<T, aligned_allocator<T, Align>>;

}  // namespace aligned