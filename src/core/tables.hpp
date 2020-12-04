#ifndef GAS_SRC_CORE_TABLES_HPP_
#define GAS_SRC_CORE_TABLES_HPP_

#include <fmt/format.h>

#include <tuple>

#include "query.hpp"

namespace gas {

using no_return_query = Query<>;

struct Log {
  using log_query = Query<int, std::string, std::string>;

  explicit Log(const std::tuple<int, std::string, std::string> &args) {
    std::tie(id, date, description) = args;
  }

  [[nodiscard]] static auto select_all() {
    return log_query("select id, date, description from logs;");
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

  [[nodiscard]] static auto select_all() {
    return user_query("select id, username, email, description from users;");
  }
  [[nodiscard]] static auto select_by_id(int id) {
    return user_query(fmt::format(
        "select id, username, email, description from users where id = '{}';",
        id));
  }
  [[nodiscard]] static auto insert(std::string_view username,
                                   std::string_view password,
                                   std::string_view email,
                                   std::string_view description) {
    return no_return_query(fmt::format(
        "insert into users(username, password, email, description) values "
        "('{}', crypt('{}', gen_salt('md5')), '{}', '{}');",
        username, password, email, description));
  }
  [[nodiscard]] static auto search(std::string_view username,
                                   std::string_view password) {
    return user_query(
        fmt::format("select (password = crypt('{}', password)) AS pwd_match "
                    "from users where username = '{}'",
                    password, username));
  }
  [[nodiscard]] static auto remove_by_id(int id) {
    return no_return_query(
        fmt::format("delete from users where id = '{}';", id));
  }
  [[nodiscard]] static auto remove_by_username(std::string_view username) {
    return no_return_query(
        fmt::format("delete from users where username = '{}';", username));
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

  [[nodiscard]] static auto select_all() {
    return resourcetype_query("select id, name from resourcetype;");
  }

  [[nodiscard]] auto insert_query() const {
    return no_return_query(
        fmt::format("insert into resourcetype(name) values ('{}');", name));
  }
  [[nodiscard]] auto update_query() const {
    return no_return_query(fmt::format(
        "update resourcetype set name = '{}' where id = '{}';", name, id));
  }
  [[nodiscard]] auto remove_query() const {
    return no_return_query(
        fmt::format("delete from resourcetype where id = '{}';", id));
  }

  int id{};
  std::string name;
};

struct Resource {
  using resource_query =
      Query<int, std::string, std::string, uint64_t, int, uint>;

  explicit Resource() = default;
  explicit Resource(const std::tuple<int, std::string, std::string, uint64_t,
                                     int, uint> &args) {
    std::tie(id, name, description, size, type, data) = args;
  }
  [[nodiscard]] static auto select_all() {
    return resource_query(
        "select id, name, description, size, type, data from resource;");
  }

  [[nodiscard]] auto insert_query() const {
    return no_return_query(
        fmt::format("insert into resource(name, description, size, "
                    "type, data) values "
                    "('{}', '{}', '{}', '{}', '{}');",
                    name, description, size, type, data));
  }
  [[nodiscard]] auto update_query() const {
    return no_return_query(
        fmt::format("update resource set name = '{}',"
                    "description = '{}', size = '{}', type = "
                    "'{}', data = '{}' where id = '{}';",
                    name, description, size, type, data, id));
  }
  [[nodiscard]] auto remove_query() const {
    return no_return_query(
        fmt::format("delete from resource where id = '{}';", id));
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
  [[nodiscard]] static auto select_all() {
    return dependency_query(
        "select id, requesting_resource, required_resource from dependency;");
  }
  [[nodiscard]] auto insert_query() const {
    return no_return_query(
        fmt::format("insert into dependency(requesting_resource, "
                    "required_resource) values ('{}', '{}');",
                    requesting_resource, required_resource));
  }

  int id{};
  int requesting_resource{};
  int required_resource{};
};

struct Commit {
  using commit_query = Query<int, int, int, std::string, std::string>;

  explicit Commit(
      const std::tuple<int, int, int, std::string, std::string> &args) {
    std::tie(id, user, resource, date, message) = args;
  }
  [[nodiscard]] static auto select_all() {
    return commit_query(
        "select id, \"user\", resource, date, message from commits;");
  }

  int id{};
  int user{};
  int resource{};
  std::string date;
  std::string message;
};

}  // namespace gas

#endif  // GAS_SRC_CORE_TABLES_HPP_
