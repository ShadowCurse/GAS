#ifndef GAS_SRC_CORE_TABLES_HPP_
#define GAS_SRC_CORE_TABLES_HPP_

#include <tuple>

namespace gas {

namespace role {
struct role_info {
  explicit role_info(const std::tuple<int, std::string> &args) {
    std::tie(id, name) = args;
  }
  int id{};
  std::string name;
};
} // namespace role

namespace project {
struct project_info {
  explicit project_info(const std::tuple<int, std::string, std::string> &args) {
    std::tie(id, name, description) = args;
  }
  int id{};
  std::string name;
  std::string description;
};
} // namespace project

namespace dev {
struct dev_info {
  explicit dev_info(const std::tuple<int, std::string, int, int> &args) {
    std::tie(id, name, age, role) = args;
  }
  int id{};
  std::string name;
  int age{};
//  int project{};
  int role{};
};
} // namespace dev

namespace commit {
struct commit_info {
  explicit commit_info(const std::tuple<int, int, int, std::string, std::string> &args) {
    std::tie(id, dev, resource, date, message) = args;
  }
  int id{};
  int dev{};
  int resource{};
  std::string date;
  std::string message;
};
} // namespace commit

namespace resource {
struct resource_info {
  explicit resource_info(const std::tuple<int, std::string, std::string, int, int, int> &args) {
    std::tie(id, name, description, size, checksum, type) = args;
  }
  int id{};
  std::string name;
  std::string description;
  int size{};
  int checksum{};
  int type{};
};
} // namespace resource

namespace dependency {
struct dependency_info {
  explicit dependency_info(const std::tuple<int, int, int> &args) {
    std::tie(id, requesting_resource, required_resource) = args;
  }
  int id{};
  int requesting_resource{};
  int required_resource{};
};
} // namespace dependency

namespace resource_type {
struct resource_type_info {
  explicit resource_type_info(const std::tuple<int, std::string> &args) {
    std::tie(id, name) = args;
  }
  int id{};
  std::string name;
};
} // namespace resource_type

} // namespace gas

#endif //GAS_SRC_CORE_TABLES_HPP_
