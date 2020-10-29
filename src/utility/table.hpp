#ifndef GAS_SRC_SQL_TABLES_HPP_
#define GAS_SRC_SQL_TABLES_HPP_

#include <string>
#include <string_view>
#include <concepts>

using namespace std::literals::string_literals;

namespace gas {

/////////////////////
/// Example table ///
/////////////////////
//namespace table_name {
// struct table_name {
//   explicit table_name(const std::tuple<types ...> &args) {
//     std::tie(fields ...) = args;
//   }
//   fields ...
// };
// } // namespace table_name

////////////////////////
/// currently unused ///
////////////////////////

template<typename T>
concept is_table = requires(T t) {{ std::invoke(t) } -> std::same_as<std::string>; }
    && std::is_class_v<typename T::table_class>;

template<typename T>
concept is_column = std::is_class_v<typename T::parent_table>;

template<typename Column, typename Table>
concept is_column_of_table = is_table<Table> && is_column<Column>
    && std::is_same_v<typename Table::table_class, typename Column::parent_table>;

#define MARK_AS_TABLE(table) \
using table_class = table;   \
auto operator()() { return std::string(#table); }

#define TABLE_FIELD(name, type) \
struct name { \
  using parent_table = table_class; \
  using field_type = type; \
  auto operator()() { return std::string(#name); }; \
};

} // namespace gas

#endif //GAS_SRC_SQL_TABLES_HPP_
