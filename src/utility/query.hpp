#ifndef GAS_SRC_UTILITY_QUERY_HPP_
#define GAS_SRC_UTILITY_QUERY_HPP_

#include <string>

namespace gas {

template <typename... T>
struct Query {
  explicit Query(std::string query) : sql(std::move(query)) {}
  std::string sql;
};

};  // namespace gas

#endif  // GAS_SRC_UTILITY_QUERY_HPP_
