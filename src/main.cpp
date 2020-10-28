#include <iostream>
#include "database/db_connector.hpp"
#include "sql/query.hpp"
#include <tuple>

using namespace gas;

struct dev_info {
  explicit dev_info(const std::tuple<int, std::string, int, int>& args) {
    std::tie(id, name, age, role) = args;
  }
  int id{};
  std::string name;
  int age{};
  int role{};
};

auto main() -> int {

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
      auto q = query<int ,std::string, int, int>(std::string("select * from DeveloperInfo;"));
      if (auto res = connector.exec(q)) {
        std::cout << "Found " << (*res).size() << " meshes:\n";
        auto values = res->cast<dev_info>();
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
