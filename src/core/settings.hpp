#ifndef GAS_SRC_UTILITY_SETTINGS_HPP_
#define GAS_SRC_UTILITY_SETTINGS_HPP_

#include <string>

#include "fmt/format.h"

namespace gas {

class Settings {
 public:
  Settings() = default;
  auto operator==(const Settings& other) const -> bool = default;

  [[nodiscard]] auto to_string() const -> std::string {
    return fmt::format("host={} port={} dbname={} user={} password={}", host,
                       std::to_string(port), db_name, username, password);
  }

  std::string host;
  size_t port{};
  std::string db_name;
  std::string username;
  std::string password;
};

}  // namespace gas

#endif  // GAS_SRC_UTILITY_SETTINGS_HPP_
