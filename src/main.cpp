#include <iostream>

#include "core/tables.hpp"
#include "utility/connector.hpp"

using namespace gas;

auto callback(const std::string& payload) { std::cout << payload << '\n'; }

auto main() -> int {
  std::cout << "Start:\n";
  auto query = Developer::get_all();
  auto settings = gas::Settings{}
                      .host("localhost")
                      .port(5432)
                      .db_name("gas")
                      .username("gas_admin")
                      .password("password");
  Connector connector(settings);
  if (connector.connect()) {
    connector.add_notifier("gas_channel", callback);
    connector.enable_notifications();
  } else {
    std::cout << "could not connect to db with settings: " +
                     settings.to_string()
              << '\n';
    return -1;
  }
  connector.disable_notifications();
//  if (auto res = connector.exec(query)) {
//    std::cout << "Found " << (*res).size() << ":\n";
//    auto values = res->cast<Developer>();
//    for (const auto& v : values) std::cout << "name = " << v.name << '\n';
//  } else {
//    std::cout << "Query was not processed\n";
//  }
  std::cout << "End\n";
  return 0;
}
