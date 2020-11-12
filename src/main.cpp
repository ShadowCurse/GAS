#include <iostream>

#include "core/core.hpp"

using namespace gas;

auto callback(const std::string& payload) { std::cout << payload << '\n'; }

auto main() -> int {
  std::cout << "Start:\n";
//  auto query = Developer::get_all();
  auto settings = gas::Settings{}
                      .host("localhost")
                      .port(5432)
                      .db_name("gas")
                      .username("gas_admin")
                      .password("password");

  Core core;
  auto storage1 = core.add_storage(settings);
  auto connection_result = core.connect_storage(storage1);

  std::cout << "Connection result: " << std::boolalpha << connection_result << '\n';

  auto view_dev = core.add_view<Developer>();
  auto view_res = core.add_view<Resource>();

  core.update_storage();

  for (const auto& item: view_dev) {
    std::cout << item.name << '\n';
  }
  for (const auto& item: view_res) {
    std::cout << item.name << '\n';
  }

  core.disconnect_storage(storage1);

  std::cout << "End\n";
  return 0;
}
