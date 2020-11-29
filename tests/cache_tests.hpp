#ifndef GAS_TESTS_CACHE_TESTS_HPP_
#define GAS_TESTS_CACHE_TESTS_HPP_

#include <gtest/gtest.h>

#include "core/cache.hpp"

using namespace gas;

using StorageCache =
StorageCache_T<int, double, char>;

TEST(Cache, put){
    StorageCache cache;
    cache.update<int>({1, 2, 3});
    cache.update<double>({1.0, 2.0, 3.0});
    cache.update<char>({'a', 'b', 'c'});
};

TEST(Cache, get_view){
  StorageCache cache;
  cache.update<int>({1, 2, 3});
  cache.update<double>({1.0, 2.0, 3.0});
  cache.update<char>({'a', 'b', 'c'});

  auto view_int = cache.get_cache_view<int>();
  ASSERT_EQ(view_int.size(), 3);
  ASSERT_EQ(*view_int.begin(), 1);
  ASSERT_EQ(*view_int.begin() + 1, 2);
  ASSERT_EQ(*view_int.begin() + 2, 3);

  auto view_double = cache.get_cache_view<double>();
  ASSERT_EQ(view_double.size(), 3);
  ASSERT_EQ(*view_double.begin(), 1.0);
  ASSERT_EQ(*view_double.begin() + 1, 2.0);
  ASSERT_EQ(*view_double.begin() + 2, 3.0);

  auto view_char = cache.get_cache_view<char>();
  ASSERT_EQ(view_char.size(), 3);
  ASSERT_EQ(*view_char.begin(), 'a');
  ASSERT_EQ(*view_char.begin() + 1, 'b');
  ASSERT_EQ(*view_char.begin() + 2, 'c');
};

TEST(Cache, search) {
  StorageCache cache;
  cache.update<int>({1, 2, 3});
  cache.update<double>({1.0, 2.0, 3.0});
  cache.update<char>({'a', 'b', 'c'});

  auto s_1 = cache.search<int>([](const auto& i) {return i == 1;});
  ASSERT_TRUE(s_1);
  ASSERT_EQ(*s_1, 1);

  auto d_2 = cache.search<double>([](const auto& i) {return i == 2.0;});
  ASSERT_TRUE(d_2);
  ASSERT_EQ(*d_2, 2.0);

  auto c_3 = cache.search<char>([](const auto& i) {return i == 'c';});
  ASSERT_TRUE(c_3);
  ASSERT_EQ(*c_3, 'c');
}

#endif  // GAS_TESTS_CACHE_TESTS_HPP_
