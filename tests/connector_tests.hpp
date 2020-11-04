#ifndef GAS_TESTS_CONNECTOR_TESTS_HPP_
#define GAS_TESTS_CONNECTOR_TESTS_HPP_

#include <gtest/gtest.h>

#include "core/tables.hpp"
#include "utility/connector.hpp"
#include "utility/settings.hpp"

using namespace gas;

TEST(Connector, set_settings) {
  auto settings = Settings{}
                      .host("localhost")
                      .port(5432)
                      .db_name("gas")
                      .username("gas_admin")
                      .password("password");

  Connector connector(settings);
  EXPECT_EQ(settings, connector.get_settings());
}

TEST(Connector, minimal_start) {
  auto settings = Settings{}
                      .host("localhost")
                      .port(5432)
                      .db_name("gas")
                      .username("gas_admin")
                      .password("password");

  Connector connector(settings);
  ASSERT_TRUE(connector.connect());
  ASSERT_TRUE(connector.connected());
}

TEST(Connector, empty_query_fail) {
  auto settings = Settings{}
                      .host("localhost")
                      .port(5432)
                      .db_name("gas")
                      .username("gas_admin")
                      .password("password");

  Connector connector(settings);
  ASSERT_TRUE(connector.connect());
  ASSERT_TRUE(connector.connected());

  auto res = connector.exec(Developer::developer_query(""));
  EXPECT_EQ(static_cast<bool>(res), false);
}

TEST(Connector, error_query_fail) {
  auto settings = Settings{}
                      .host("localhost")
                      .port(5432)
                      .db_name("gas")
                      .username("gas_admin")
                      .password("password");

  Connector connector(settings);
  ASSERT_EQ(connector.connect(), true);
  ASSERT_EQ(connector.connected(), true);

  auto res = connector.exec(Developer::developer_query("qwe ewq ewq eqw"));
  EXPECT_EQ(static_cast<bool>(res), false);
}

TEST(Connector, select_pass) {
  auto settings = Settings{}
                      .host("localhost")
                      .port(5432)
                      .db_name("gas")
                      .username("gas_admin")
                      .password("password");

  Connector connector(settings);
  ASSERT_TRUE(connector.connect());
  ASSERT_TRUE(connector.connected());

  auto query = Developer::get_all();
  auto res = connector.exec(query);
  EXPECT_EQ(static_cast<bool>(res), true);
  EXPECT_GT((*res).size(), 0);
  auto values = res->cast<Developer>();
  EXPECT_GT(values.size(), 0);
}

TEST(Connector, insert_pass) {
  auto settings = Settings{}
                      .host("localhost")
                      .port(5432)
                      .db_name("gas")
                      .username("gas_admin")
                      .password("password");

  Connector connector(settings);
  ASSERT_TRUE(connector.connect());
  ASSERT_TRUE(connector.connected());

  auto query = Developer::insert("test", "test", "test");
  auto res = connector.exec(query);
  EXPECT_EQ(static_cast<bool>(res), true);
  EXPECT_EQ((*res).size(), 0);
  auto values = res->cast<Developer>();
  EXPECT_EQ(values.size(), 0);
}

TEST(Connector, delete_pass) {
  auto settings = Settings{}
                      .host("localhost")
                      .port(5432)
                      .db_name("gas")
                      .username("gas_admin")
                      .password("password");

  Connector connector(settings);
  ASSERT_TRUE(connector.connect());
  ASSERT_TRUE(connector.connected());

  auto query = Developer::remove("test");
  auto res = connector.exec(query);
  EXPECT_EQ(static_cast<bool>(res), true);
  EXPECT_EQ((*res).size(), 0);
  auto values = res->cast<Developer>();
  EXPECT_EQ(values.size(), 0);
}

TEST(Connector, add_notifiers) {
  auto settings = Settings{}
                      .host("localhost")
                      .port(5432)
                      .db_name("gas")
                      .username("gas_admin")
                      .password("password");

  Connector connector(settings);
  ASSERT_TRUE(connector.connect());
  ASSERT_TRUE(connector.connected());

  auto test_notifier = [](const std::string& payload) {};

  connector.add_notifier("channel1", test_notifier);
  connector.add_notifier("channel1", test_notifier);
  connector.add_notifier("channel2", test_notifier);
  auto notifiers = connector.get_notifiers();
  EXPECT_NE(
      std::find_if(std::begin(notifiers), std::end(notifiers),
                   [](const auto& item) { return item.channel == "channel1"; }),
      std::end(notifiers));
  EXPECT_EQ(std::count_if(
                std::begin(notifiers), std::end(notifiers),
                [](const auto& item) { return item.channel == "channel1"; }),
            2);
  EXPECT_NE(
      std::find_if(std::begin(notifiers), std::end(notifiers),
                   [](const auto& item) { return item.channel == "channel2"; }),
      std::end(notifiers));
  EXPECT_EQ(std::count_if(
                std::begin(notifiers), std::end(notifiers),
                [](const auto& item) { return item.channel == "channel2"; }),
            1);
}

TEST(Connector, delete_notifiers) {
  auto settings = Settings{}
                      .host("localhost")
                      .port(5432)
                      .db_name("gas")
                      .username("gas_admin")
                      .password("password");

  Connector connector(settings);
  ASSERT_TRUE(connector.connect());
  ASSERT_TRUE(connector.connected());

  auto test_notifier = [](const std::string& payload) {};

  connector.add_notifier("channel1", test_notifier);
  connector.add_notifier("channel1", test_notifier);
  connector.add_notifier("channel2", test_notifier);

  connector.delete_notifiers("channel1");

  auto notifiers = connector.get_notifiers();
  EXPECT_EQ(
      std::find_if(std::begin(notifiers), std::end(notifiers),
                   [](const auto& item) { return item.channel == "channel1"; }),
      std::end(notifiers));
  EXPECT_NE(
      std::find_if(std::begin(notifiers), std::end(notifiers),
                   [](const auto& item) { return item.channel == "channel2"; }),
      std::end(notifiers));
  EXPECT_EQ(std::count_if(
                std::begin(notifiers), std::end(notifiers),
                [](const auto& item) { return item.channel == "channel2"; }),
            1);
}

TEST(Connector, enable_notifiers) {
  auto settings = Settings{}
                      .host("localhost")
                      .port(5432)
                      .db_name("gas")
                      .username("gas_admin")
                      .password("password");

  Connector connector(settings);
  ASSERT_TRUE(connector.connect());
  ASSERT_TRUE(connector.connected());

  auto test_notifier = [](const std::string& payload) {};

  std::cout << "adding notifiers\n";

  connector.add_notifier("channel1", test_notifier);
  connector.add_notifier("channel1", test_notifier);
  connector.add_notifier("channel2", test_notifier);

  std::cout << "testing ...\n";

  EXPECT_EQ(connector.enable_notifications(), true);
  EXPECT_EQ(connector.notifications_enabled(), true);
}

TEST(Connector, disable_notifications) {
  auto settings = Settings{}
                      .host("localhost")
                      .port(5432)
                      .db_name("gas")
                      .username("gas_admin")
                      .password("password");

  Connector connector(settings);
  ASSERT_EQ(connector.connect(), true);
  ASSERT_EQ(connector.connected(), true);

  auto test_notifier = [](const std::string& payload) {};

  std::cout << "adding notifiers\n";

  connector.add_notifier("channel1", test_notifier);
  connector.add_notifier("channel1", test_notifier);
  connector.add_notifier("channel2", test_notifier);

  std::cout << "testing ...\n";

  EXPECT_EQ(connector.enable_notifications(), true);
  EXPECT_EQ(connector.notifications_enabled(), true);
  EXPECT_EQ(connector.disable_notifications(), true);
  EXPECT_EQ(connector.notifications_enabled(), false);
}

TEST(Connector, test_notifications) {
  auto settings = Settings{}
                      .host("localhost")
                      .port(5432)
                      .db_name("gas")
                      .username("gas_admin")
                      .password("password");

  Connector connector(settings);
  ASSERT_EQ(connector.connect(), true);
  ASSERT_EQ(connector.connected(), true);

  int count{0};
  auto test_notifier = [&count](const std::string& payload) { ++count; };

  connector.add_notifier("gas_channel", test_notifier);

  EXPECT_EQ(connector.enable_notifications(), true);
  EXPECT_EQ(connector.notifications_enabled(), true);

  auto query = Developer::insert("test", "test", "test");
  for (int i{0}; i < 3; ++i) {
    auto res = connector.exec(query);
    EXPECT_EQ(static_cast<bool>(res), true);
    EXPECT_EQ((*res).size(), 0);
    auto values = res->cast<Developer>();
    EXPECT_EQ(values.size(), 0);
  }
  EXPECT_EQ(count, 3);
}

#endif  // GAS_TESTS_CONNECTOR_TESTS_HPP_
