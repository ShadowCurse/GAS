#ifndef GAS_TESTS_STORAGE_TESTS_HPP_
#define GAS_TESTS_STORAGE_TESTS_HPP_

#include <gtest/gtest.h>

#include "core/storage.hpp"

using namespace gas;

TEST(StorageUnit, connect) {
  auto settings = Settings{};
  settings.host = "localhost";
  settings.port = 5432;
  settings.db_name = "gas";
  settings.username = "gas_admin";
  settings.password = "password";

  StorageUnit su(0, settings);
  ASSERT_TRUE(su.connect());
  ASSERT_TRUE(su.connected());
}

TEST(StorageUnit, disconnect) {
  auto settings = Settings{};
  settings.host = "localhost";
  settings.port = 5432;
  settings.db_name = "gas";
  settings.username = "gas_admin";
  settings.password = "password";

  StorageUnit su(0, settings);
  ASSERT_TRUE(su.connect());
  ASSERT_TRUE(su.connected());
  su.disconnect();
  ASSERT_FALSE(su.connected());
}

TEST(StorageUnit, update) {
  auto settings = Settings{};
  settings.host = "localhost";
  settings.port = 5432;
  settings.db_name = "gas";
  settings.username = "gas_admin";
  settings.password = "password";

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
  auto settings = Settings{};
  settings.host = "localhost";
  settings.port = 5432;
  settings.db_name = "gas";
  settings.username = "gas_admin";
  settings.password = "password";
  StorageUnit su(0, settings);
  ASSERT_TRUE(su.connect());
  ASSERT_TRUE(su.connected());
  EXPECT_TRUE(su.update_all());
  su.disconnect();
  ASSERT_FALSE(su.connected());
}

TEST(StorageUnit, view) {
  auto settings = Settings{};
  settings.host = "localhost";
  settings.port = 5432;
  settings.db_name = "gas";
  settings.username = "gas_admin";
  settings.password = "password";

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
  auto settings = Settings{};
  settings.host = "localhost";
  settings.port = 5432;
  settings.db_name = "gas";
  settings.username = "gas_admin";
  settings.password = "password";

  StorageUnit su(0, settings);
  ASSERT_TRUE(su.connect());
  ASSERT_TRUE(su.connected());
  EXPECT_TRUE(su.update_all());
  auto user1 = su.search<User>(
      [](const auto& user) { return user.username == "user1"; });
  EXPECT_TRUE(user1);
  EXPECT_EQ((*user1).username, "user1");
  EXPECT_EQ((*user1).email, "email1");
  EXPECT_EQ((*user1).description, "description1");
  su.disconnect();
  ASSERT_FALSE(su.connected());
}

TEST(StorageUnit, insert_item) {
  auto settings = Settings{};
  settings.host = "localhost";
  settings.port = 5432;
  settings.db_name = "gas";
  settings.username = "gas_admin";
  settings.password = "password";

  StorageUnit su(0, settings);
  ASSERT_TRUE(su.connect());
  ASSERT_TRUE(su.connected());
  EXPECT_TRUE(su.update_all());
  ResourceType rt({0, "test_resource_type"});
  EXPECT_TRUE(su.insert(rt));
  auto rt_s = su.search<ResourceType>(
      [](const auto& rt) { return rt.name == "test_resource_type"; });
  ASSERT_TRUE(rt_s);
  EXPECT_EQ((*rt_s).name, "test_resource_type");
  su.disconnect();
  ASSERT_FALSE(su.connected());
}

TEST(StorageUnit, update_item) {
  auto settings = Settings{};
  settings.host = "localhost";
  settings.port = 5432;
  settings.db_name = "gas";
  settings.username = "gas_admin";
  settings.password = "password";

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
  auto rt_s3 = su.search<ResourceType>([](const auto& type) {
    return type.name == "test_resource_type_updated";
  });
  EXPECT_TRUE(rt_s3);
  su.disconnect();
  ASSERT_FALSE(su.connected());
}

TEST(StorageUnit, remove_item) {
  auto settings = Settings{};
  settings.host = "localhost";
  settings.port = 5432;
  settings.db_name = "gas";
  settings.username = "gas_admin";
  settings.password = "password";

  StorageUnit su(0, settings);
  ASSERT_TRUE(su.connect());
  ASSERT_TRUE(su.connected());
  EXPECT_TRUE(su.update_all());
  auto rt_s = su.search<ResourceType>(
      [](const auto& rt) { return rt.name == "test_resource_type_updated"; });
  EXPECT_TRUE(su.remove(*rt_s));
  auto rt_s2 = su.search<ResourceType>(
      [](const auto& rt) { return rt.name == "test_resource_type_updated"; });
  EXPECT_FALSE(rt_s2);
  su.disconnect();
  ASSERT_FALSE(su.connected());
}

TEST(StorageUnit, create_resource) {
  auto settings = Settings{};
  settings.host = "localhost";
  settings.port = 5432;
  settings.db_name = "gas";
  settings.username = "gas_admin";
  settings.password = "password";

  StorageUnit su(0, settings);
  ASSERT_TRUE(su.connect());
  ASSERT_TRUE(su.connected());
  EXPECT_TRUE(su.update_all());
  auto rt_s = su.search<Resource>(
      [](const auto& rt) { return rt.name == "test_resource"; });
  EXPECT_FALSE(rt_s);
  Resource rs{{0, "test_resource", "", 0, 2, 0}};
  EXPECT_TRUE(su.create_resource(rs, "../../README.md"));
  auto rt_s2 = su.search<Resource>(
      [](const auto& rt) { return rt.name == "test_resource"; });
  EXPECT_TRUE(rt_s2);
  su.disconnect();
  ASSERT_FALSE(su.connected());
}

TEST(StorageUnit, create_resource_fail) {
  auto settings = Settings{};
  settings.host = "localhost";
  settings.port = 5432;
  settings.db_name = "gas";
  settings.username = "gas_admin";
  settings.password = "password";

  StorageUnit su(0, settings);
  ASSERT_TRUE(su.connect());
  ASSERT_TRUE(su.connected());
  EXPECT_TRUE(su.update_all());
  Resource rs{{0, "test_resource", "", 0, 2, 0}};
  EXPECT_FALSE(su.create_resource(rs, "qweqwe"));
  su.disconnect();
  ASSERT_FALSE(su.connected());
}

TEST(StorageUnit, update_resource) {
  auto settings = Settings{};
  settings.host = "localhost";
  settings.port = 5432;
  settings.db_name = "gas";
  settings.username = "gas_admin";
  settings.password = "password";

  StorageUnit su(0, settings);
  ASSERT_TRUE(su.connect());
  ASSERT_TRUE(su.connected());
  EXPECT_TRUE(su.update_all());
  auto rt_s = su.search<Resource>(
      [](const auto& rt) { return rt.name == "test_resource"; });
  EXPECT_TRUE(rt_s);
  EXPECT_TRUE(su.update_resource(*rt_s, "../../sql/test_fill.sql"));
  su.disconnect();
  ASSERT_FALSE(su.connected());
}

TEST(StorageUnit, update_resource_fail) {
  auto settings = Settings{};
  settings.host = "localhost";
  settings.port = 5432;
  settings.db_name = "gas";
  settings.username = "gas_admin";
  settings.password = "password";

  StorageUnit su(0, settings);
  ASSERT_TRUE(su.connect());
  ASSERT_TRUE(su.connected());
  EXPECT_TRUE(su.update_all());
  auto rt_s = su.search<Resource>(
      [](const auto& rt) { return rt.name == "test_resource"; });
  EXPECT_TRUE(rt_s);
  EXPECT_FALSE(su.update_resource(*rt_s, "qwer"));
  su.disconnect();
  ASSERT_FALSE(su.connected());
}

TEST(StorageUnit, download_resource) {
  auto settings = Settings{};
  settings.host = "localhost";
  settings.port = 5432;
  settings.db_name = "gas";
  settings.username = "gas_admin";
  settings.password = "password";

  StorageUnit su(0, settings);
  ASSERT_TRUE(su.connect());
  ASSERT_TRUE(su.connected());
  EXPECT_TRUE(su.update_all());
  auto rt_s = su.search<Resource>(
      [](const auto& rt) { return rt.name == "test_resource"; });
  EXPECT_TRUE(rt_s);
  su.download_resource(*rt_s, "test_resource.txt");
  EXPECT_TRUE(std::filesystem::exists("test_resource.txt"));
  su.disconnect();
  ASSERT_FALSE(su.connected());
}

TEST(StorageUnit, remove_resource) {
  auto settings = Settings{};
  settings.host = "localhost";
  settings.port = 5432;
  settings.db_name = "gas";
  settings.username = "gas_admin";
  settings.password = "password";

  StorageUnit su(0, settings);
  ASSERT_TRUE(su.connect());
  ASSERT_TRUE(su.connected());
  EXPECT_TRUE(su.update_all());
  auto rt_s = su.search<Resource>(
      [](const auto& rt) { return rt.name == "test_resource"; });
  EXPECT_TRUE(rt_s);
  EXPECT_TRUE(su.remove_resource(*rt_s));
  auto rt_s2 = su.search<Resource>(
      [](const auto& rt) { return rt.name == "test_resource"; });
  EXPECT_FALSE(rt_s2);
  EXPECT_TRUE(std::filesystem::exists("test_resource.txt"));
  su.disconnect();
  ASSERT_FALSE(su.connected());
}

TEST(Storage, add_storage) {
  auto settings = Settings{};
  settings.host = "localhost";
  settings.port = 5432;
  settings.db_name = "gas";
  settings.username = "gas_admin";
  settings.password = "password";

  Storage st;
  auto sid = st.add_storage(settings);
  auto storage = st.get_storage(sid);
  EXPECT_TRUE(storage);
  EXPECT_FALSE((*storage)->connected());
}

TEST(Storage, connect_storage) {
  auto settings = Settings{};
  settings.host = "localhost";
  settings.port = 5432;
  settings.db_name = "gas";
  settings.username = "gas_admin";
  settings.password = "password";

  Storage st;
  auto sid = st.add_storage(settings);
  auto storage = st.get_storage(sid);
  EXPECT_TRUE(storage);
  EXPECT_FALSE((*storage)->connected());
  EXPECT_TRUE(st.connect_storage(sid));
  EXPECT_TRUE((*storage)->connected());
}

TEST(Storage, disconnect_storage) {
  auto settings = Settings{};
  settings.host = "localhost";
  settings.port = 5432;
  settings.db_name = "gas";
  settings.username = "gas_admin";
  settings.password = "password";

  Storage st;
  auto sid = st.add_storage(settings);
  auto storage = st.get_storage(sid);
  EXPECT_TRUE(storage);
  EXPECT_FALSE((*storage)->connected());
  EXPECT_TRUE(st.connect_storage(sid));
  EXPECT_TRUE((*storage)->connected());
  st.disconnect_storage(sid);
  EXPECT_FALSE((*storage)->connected());
}

TEST(Storage, remove_storage) {
  auto settings = Settings{};
  settings.host = "localhost";
  settings.port = 5432;
  settings.db_name = "gas";
  settings.username = "gas_admin";
  settings.password = "password";

  Storage st;
  auto sid = st.add_storage(settings);
  auto storage = st.get_storage(sid);
  EXPECT_TRUE(storage);
  EXPECT_FALSE((*storage)->connected());
  EXPECT_TRUE(st.connect_storage(sid));
  EXPECT_TRUE((*storage)->connected());
  st.remove_storage(sid);
  EXPECT_FALSE(st.get_storage(sid));
}

TEST(Storage, create_view) {
  auto settings = Settings{};
  settings.host = "localhost";
  settings.port = 5432;
  settings.db_name = "gas";
  settings.username = "gas_admin";
  settings.password = "password";

  Storage st;
  auto sid = st.add_storage(settings);
  auto storage = st.get_storage(sid);
  EXPECT_TRUE(storage);
  EXPECT_FALSE((*storage)->connected());
  EXPECT_TRUE(st.connect_storage(sid));
  EXPECT_TRUE((*storage)->connected());
  auto view = st.create_view<Resource>();
  EXPECT_GT(view.size(), 0);
  int size{0};
  for (const auto& item : view) ++size;
  EXPECT_GT(size, 0);
  st.remove_storage(sid);
}

#endif  // GAS_TESTS_STORAGE_TESTS_HPP_
