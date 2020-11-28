#include <gtest/gtest.h>

#include "connector_tests.hpp"
#include "cache_tests.hpp"
#include "storage_tests.hpp"

auto main(int argc, char** argv) -> int {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}