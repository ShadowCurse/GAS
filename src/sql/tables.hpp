#ifndef GAMEASSETSDB_SRC_SQL_TABLES_HPP_
#define GAMEASSETSDB_SRC_SQL_TABLES_HPP_

#include <string_view>

using namespace std::literals::string_literals;

namespace gas {

auto all() {
  return "*"s;
};

#define MARK_AS_TABLE(table) \
using table_class = table; \
auto operator()() { return std::string(#table); }

#define TABLE_FIELD(name, type) \
struct name { \
  using parent_table = table_class; \
  using field_type = type; \
  auto operator()() { return std::string(#name); }; \
};

} // namespace ga

#endif //GAMEASSETSDB_SRC_SQL_TABLES_HPP_
