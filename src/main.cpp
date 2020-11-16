#include <iostream>

#include "core/core.hpp"

using namespace gas;

auto main() -> int {
  std::cout << "Start:\n";
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

  auto res = Resource{{13, "actions", "descr", 131, 1, 0}};

  core.create_resource(storage1, res, "actions.sql");
  core.download_resource(storage1, res, "new_actions.sql");

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  core.remove_resource(storage1, *view_res.begin());

  core.update_storage();

  std::cout << "Total res size: " << view_res.size() << '\n';
  for (const auto& item: view_res) {
    std::cout << item.data << '\n';
  }

//  std::getchar();

//  std::cout << "Total devs size: " << view_dev.size() << '\n';

  core.disconnect_storage(storage1);

  std::cout << "End\n";


//  auto settings = Settings{}
//      .host("localhost")
//      .port(5432)
//      .db_name("gas")
//      .username("gas_admin")
//      .password("password");
//
//  Connector connector(settings);
//  std::cout << std::boolalpha << connector.connect() << '\n';
//
//  auto oid = connector.upload_large_object("actions.sql");
//  connector.download_large_object(oid, "new_actions.sql");
//  std::cout << "oid: " << oid << '\n';

  return 0;
}
