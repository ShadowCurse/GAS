#include <iostream>
#include "utility/db_connector.hpp"
#include "core/tables.hpp"
#include "core/query_collection.hpp"

using namespace gas;

auto main() -> int {

  auto query = dev::get_named("Dev1");

  auto settings =
      gas::Settings{}
          .host("localhost")
          .port(5432)
          .db_name("game_assets")
          .username("username")
          .password("password");
  Connector connector;
  try {
    if (connector.connect(settings)) {
      if (auto res = connector.exec(query)) {
        std::cout << "Found " << (*res).size() << ":\n";
        auto values = res->cast<dev::dev_info>();
        for (const auto& v: values)
          std::cout << "name = " << v.name << '\n';
      } else {
        std::cout << "Nothing was found\n";
      }
    } else {
      std::cout << "could not connect to db with settings: " + settings.to_string() << '\n';
    }
  }
  catch (std::exception const &e) {
    std::cerr << e.what() << '\n';
    return 1;
  }
  return 0;

}
