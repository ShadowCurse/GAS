#ifndef GAS_TESTS_STORAGE_TESTS_HPP_
#define GAS_TESTS_STORAGE_TESTS_HPP_

#include <gtest/gtest.h>

#include "core/storage.hpp"

using namespace gas;

TEST(StorageUnit, connect) {
  auto settings = Settings{}
                      .host("localhost")
                      .port(5432)
                      .db_name("gas")
                      .username("gas_admin")
                      .password("password");
  StorageUnit su(0, settings);
  ASSERT_TRUE(su.connect());
  ASSERT_TRUE(su.connected());
}

TEST(StorageUnit, disconnect) {
  auto settings = Settings{}
                      .host("localhost")
                      .port(5432)
                      .db_name("gas")
                      .username("gas_admin")
                      .password("password");
  StorageUnit su(0, settings);
  ASSERT_TRUE(su.connect());
  ASSERT_TRUE(su.connected());
  su.disconnect();
  ASSERT_FALSE(su.connected());
}

TEST(StorageUnit, update) {
  auto settings = Settings{}
                      .host("localhost")
                      .port(5432)
                      .db_name("gas")
                      .username("gas_admin")
                      .password("password");
  StorageUnit su(0, settings);
  ASSERT_TRUE(su.connect());
  ASSERT_TRUE(su.connected());
  EXPECT_TRUE(su.update<Commit>());
  EXPECT_TRUE(su.update<Dependency>());
  EXPECT_TRUE(su.update<Log>());
  EXPECT_TRUE(su.update<Resource>());
  EXPECT_TRUE(su.update<ResourceType>());
  EXPECT_TRUE(su.update<User>());
  su.disconnect();
  ASSERT_FALSE(su.connected());
}

TEST(StorageUnit, update_all) {
  auto settings = Settings{}
                      .host("localhost")
                      .port(5432)
                      .db_name("gas")
                      .username("gas_admin")
                      .password("password");
  StorageUnit su(0, settings);
  ASSERT_TRUE(su.connect());
  ASSERT_TRUE(su.connected());
  EXPECT_TRUE(su.update_all());
  su.disconnect();
  ASSERT_FALSE(su.connected());
}

TEST(StorageUnit, view) {
  auto settings = Settings{}
                      .host("localhost")
                      .port(5432)
                      .db_name("gas")
                      .username("gas_admin")
                      .password("password");
  StorageUnit su(0, settings);
  ASSERT_TRUE(su.connect());
  ASSERT_TRUE(su.connected());
  EXPECT_TRUE(su.update_all());
  auto view = su.create_view<User>();
  EXPECT_GT(view.size(), 0);
  uint32_t s{0};
  for (const auto& u : view) ++s;
  EXPECT_GT(s, 0);
  EXPECT_EQ(view.size(), s);
  su.disconnect();
  ASSERT_FALSE(su.connected());
}

TEST(StorageUnit, search) {
  auto settings = Settings{}
                      .host("localhost")
                      .port(5432)
                      .db_name("gas")
                      .username("gas_admin")
                      .password("password");
  StorageUnit su(0, settings);
  ASSERT_TRUE(su.connect());
  ASSERT_TRUE(su.connected());
  EXPECT_TRUE(su.update_all());
  auto user1 = su.search<User>(
      [](const auto& user) { return user.username == "name1"; });
  EXPECT_TRUE(user1);
  EXPECT_EQ((*user1).username, "name1");
  EXPECT_EQ((*user1).email, "email1");
  EXPECT_EQ((*user1).description, "description1");
  su.disconnect();
  ASSERT_FALSE(su.connected());
}

TEST(StorageUnit, insert_item) {
  auto settings = Settings{}
                      .host("localhost")
                      .port(5432)
                      .db_name("gas")
                      .username("gas_admin")
                      .password("password");
  StorageUnit su(0, settings);
  ASSERT_TRUE(su.connect());
  ASSERT_TRUE(su.connected());
  EXPECT_TRUE(su.update_all());
  ResourceType rt({5, "test_resource_type"});
  EXPECT_TRUE(su.insert(rt));
  auto rt_s = su.search<ResourceType>(
      [](const auto& rt) { return rt.name == "test_resource_type"; });
  EXPECT_TRUE(rt_s);
  EXPECT_EQ((*rt_s).name, "test_resource_type");
  su.disconnect();
  ASSERT_FALSE(su.connected());
}

TEST(StorageUnit, update_item) {
  auto settings = Settings{}
                      .host("localhost")
                      .port(5432)
                      .db_name("gas")
                      .username("gas_admin")
                      .password("password");
  StorageUnit su(0, settings);
  ASSERT_TRUE(su.connect());
  ASSERT_TRUE(su.connected());
  EXPECT_TRUE(su.update_all());
  auto rt_s = su.search<ResourceType>(
      [](const auto& type) { return type.name == "test_resource_type"; });
  EXPECT_TRUE(rt_s);
  ResourceType rt({(*rt_s).id, "test_resource_type_updated"});
  EXPECT_TRUE(su.update(rt));
  auto rt_s2 = su.search<ResourceType>(
      [](const auto& type) { return type.name == "test_resource_type"; });
  EXPECT_FALSE(rt_s2);
  auto rt_s3 = su.search<ResourceType>(
      [](const auto& type) { return type.name == "test_resource_type_updated"; });
  EXPECT_TRUE(rt_s3);
  su.disconnect();
  ASSERT_FALSE(su.connected());
}

TEST(StorageUnit, remove_item) {
  auto settings = Settings{}
                      .host("localhost")
                      .port(5432)
                      .db_name("gas")
                      .username("gas_admin")
                      .password("password");
  StorageUnit su(0, settings);
  ASSERT_TRUE(su.connect());
  ASSERT_TRUE(su.connected());
  EXPECT_TRUE(su.update_all());
  std::cout << "update_all\n";
  auto rt_s = su.search<ResourceType>(
      [](const auto& rt) { return rt.name == "test_resource_type_updated"; });
  EXPECT_TRUE(rt_s);
  std::cout << "rt_s\n";
  EXPECT_TRUE(su.remove(*rt_s));
  auto rt_s2 = su.search<ResourceType>(
      [](const auto& rt) { return rt.name == "test_resource_type_updated"; });
  EXPECT_FALSE(rt_s2);
  std::cout << "rt_s2\n";
  su.disconnect();
  ASSERT_FALSE(su.connected());
}

#endif  // GAS_TESTS_STORAGE_TESTS_HPP_
