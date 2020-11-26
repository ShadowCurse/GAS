#ifndef GAS_SRC_CORE_TABLES_HPP_
#define GAS_SRC_CORE_TABLES_HPP_

#include <fmt/format.h>

#include <tuple>

#include "query.hpp"

namespace gas {

struct Log {
  using log_query = Query<int, std::string, std::string>;

  explicit Log(const std::tuple<int, std::string, std::string> &args) {
    std::tie(id, date, description) = args;
  }

  [[nodiscard]] static auto get_all() {
    return log_query("select * from logs;");
  }

  int id{};
  std::string date;
  std::string description;
};

struct User {
  using user_query = Query<int, std::string, std::string, std::string>;

  explicit User(
      const std::tuple<int, std::string, std::string, std::string> &args) {
    std::tie(id, username, email, description) = args;
  }

  [[nodiscard]] static auto get_all() {
    return user_query("select * from developer;");
  }
  [[nodiscard]] static auto get_named(std::string_view name) {
    return user_query(
        fmt::format("select * from developer where name = '{}'", name));
  }
  [[nodiscard]] static auto insert(std::string_view name,
                                   std::string_view email,
                                   std::string_view description) {
    return user_query(
        fmt::format("insert into developer(name, email, description) values "
                    "('{}', '{}', '{}');",
                    name, email, description));
  }
  [[nodiscard]] static auto remove(std::string_view name) {
    return user_query(
        fmt::format("delete from developer where name = '{}';", name));
  }

  int id{};
  std::string username;
  std::string email;
  std::string description;
};

struct ResourceType {
  using resourcetype_query = Query<int, std::string>;

  explicit ResourceType(const std::tuple<int, std::string> &args) {
    std::tie(id, name) = args;
  }

  [[nodiscard]] static auto get_all() {
    return resourcetype_query("select * from resourecetype;");
  }

  int id{};
  std::string name;
};

struct Resource {
  using resource_query =
      Query<int, std::string, std::string, uint64_t , int, uint>;

  explicit Resource(const std::tuple<int, std::string, std::string, uint64_t , int, uint> &args) {
    std::tie(id, name, description, size, type, data) = args;
  }
  [[nodiscard]] static auto get_all() {
    return resource_query("select * from resource;");
  }

  [[nodiscard]] auto insert_query() const {
    return resource_query(
        fmt::format("insert into resource(name, description, size, "
                    "type, data) values "
                    "('{}', '{}', '{}', '{}', '{}');",
                    name, description, size, type, data));
  }
  [[nodiscard]] auto update_query() const {
    return resource_query(
        fmt::format("update resource set name = '{}',"
                    "description = '{}', size = '{}', type = "
                    "'{}', data = '{}' where id = '{}'",
                    name, description, size, type, data, id));
  }
  [[nodiscard]] auto remove_query() const {
    return resource_query(
        fmt::format("delete from resource where id = '{}'", id));
  }

  int id{};
  std::string name;
  std::string description;
  uint64_t size{};
  int type{};
  uint data{};
};

struct Dependency {
  using dependency_query = Query<int, int, int>;

  explicit Dependency(const std::tuple<int, int, int> &args) {
    std::tie(id, requesting_resource, required_resource) = args;
  }
  [[nodiscard]] static auto get_all() {
    return dependency_query("select * from dependecy;");
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
  [[nodiscard]] static auto get_all() {
    return commit_query("select * from commits;");
  }

  int id{};
  int dev{};
  int resource{};
  std::string date;
  std::string message;
};

}  // namespace gas

#endif  // GAS_SRC_CORE_TABLES_HPP_
