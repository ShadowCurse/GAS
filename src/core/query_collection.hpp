#ifndef GAS_SRC_CORE_QUERIES_HPP_
#define GAS_SRC_CORE_QUERIES_HPP_

//#include <array>
//#include <utility>
//#include <stdexcept>

//using namespace std::string_view_literals;
//
//template<typename Key, typename Value, std::size_t Size>
//struct ConstExprMap {
//  std::array<std::pair<Key, Value>, Size> data;
//  [[nodiscard]] constexpr Value operator[](const Key &key) const {
//    if (const auto result =
//          std::find_if(std::begin(data), std::end(data), [&key](const auto &value) { return value.first = key; });
//        result != std::end(data)) {
//      return result->second;
//    } else {
//      throw std::range_error("Value not found");
//    }
//  }
//};
//
//static constexpr ConstExprMap<std::string_view, std::string_view, 10> query_collection =
//    {std::pair{"add_project"sv, ""sv},
//     {"delete_project", ""}};

#include <fmt/format.h>
#include "tables.hpp"
#include "utility/query.hpp"

namespace gas {

namespace role {
using role_query = query<int, std::string>;
auto get_all() {
  return role_query("select * from Roles");
}
} // namespace role

namespace project {
using project_info_query = query<int, std::string, std::string>;
} // namespace project

namespace dev {
using dev_info_query = query<int, std::string, int, int>;
} // namespace dev

namespace commit {
using commit_info_query = query<int, int, int, std::string, std::string>;
} // namespace commit

namespace resource {
using resource_info_query = query<int, std::string, std::string, int, int, int>;
} // namespace resource

namespace dependency {
using dependency_info_query = query<int, int, int>;
} // namespace dependency

namespace resource_type {
using resource_type_info_query = query<int, std::string>;
} // namespace resource_type

namespace dev {
  auto get_all() {
    return dev_info_query("select * from DeveloperInfo;");
  }
  auto get_named(const std::string &name) {
    return dev_info_query(fmt::format("select * from DeveloperInfo where name = '{}'", name));
  }
} // namespace dev

} // namespace gas

#endif //GAS_SRC_CORE_QUERIES_HPP_
