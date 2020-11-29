//#include <QApplication>
//
//#include "gas_core.hpp"
//
//int main(int argc, char *argv[])
//{
//    QApplication a(argc, argv);
//    GasCore core;
//    core.show_gui();
//    return a.exec();
//}

#include "core/storage.hpp"
#include "core/tables.hpp"

using namespace gas;

auto main() -> int {
  auto settings = Settings{}
                      .host("localhost")
                      .port(5432)
                      .db_name("gas")
                      .username("gas_admin")
                      .password("password");
  StorageUnit su(0, settings);
  assert(su.connect());
  assert(su.connected());
  assert(su.update_all());

  ResourceType rt({5, "test_resource_type"});
  assert(su.insert(rt));
  auto rt_s = su.search<ResourceType>(
      [](const auto& rt) { return rt.name == "test_resource_type"; });
//  assert(rt_s);

  ResourceType rt2({(*rt_s).id, "test_resource_type_updated"});
  assert(su.update(rt2));
  auto rt_s2 = su.search<ResourceType>(
      [](const auto& type) { return type.name == "test_resource_type"; });
//  assert(!rt_s2);
  auto rt_s3 = su.search<ResourceType>(
      [](const auto& type) { return type.name == "test_resource_type_updated"; });
//  assert(rt_s3);
  assert(su.remove(*rt_s));
  auto rt_s4 = su.search<ResourceType>(
      [](const auto& rt) { return rt.name == "test_resource_type_updated"; });
//  assert(!rt_s4);
  su.disconnect();
  assert(!su.connected());
}