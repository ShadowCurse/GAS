#ifndef GAS_SRC_CORE_TABLES_HPP_
#define GAS_SRC_CORE_TABLES_HPP_

#include <fmt/format.h>

#include <tuple>

#include "utility/query.hpp"

namespace gas {

struct Developer {
  using developer_query = Query<int, std::string, std::string, std::string>;

  explicit Developer(const std::tuple<int, std::string, std::string, std::string> &args) {
    std::tie(id, name, email, description) = args;
  }

  [[nodiscard]] static auto get_all() {
    return developer_query("select * from developer;");
  }
  [[nodiscard]] static auto get_named(const std::string &name) {
    return developer_query(
        fmt::format("select * from developer where name = '{}'", name));
  }

  int id{};
  std::string name;
  std::string email;
  std::string description;
};

struct ResourceType {
  using resourcetype_query = Query<int, std::string>;

  explicit ResourceType(const std::tuple<int, std::string> &args) {
    std::tie(id, name) = args;
  }

  int id{};
  std::string name;
};

struct Resource {
  using resource_query =
  Query<int, std::string, std::string, int, int, int>;

  explicit Resource(
      const std::tuple<int, std::string, std::string, int, int, int> &args) {
    std::tie(id, name, description, size, checksum, type) = args;
  }

  int id{};
  std::string name;
  std::string description;
  int size{};
  int checksum{};
  int type{};
};

struct Dependency {
  using dependency_query = Query<int, int, int>;

  explicit Dependency(const std::tuple<int, int, int> &args) {
    std::tie(id, requesting_resource, required_resource) = args;
  }

  int id{};
  int requesting_resource{};
  int required_resource{};
};

struct Commit {
  using commit_query = Query<int, int, int, std::string, std::string>;

  explicit Commit(
      const std::tuple<int, int, int, std::string, std::string> &args) {
    std::tie(id, dev, resource, date, message) = args;
  }

  int id{};
  int dev{};
  int resource{};
  std::string date;
  std::string message;
};

}  // namespace gas

#endif  // GAS_SRC_CORE_TABLES_HPP_
