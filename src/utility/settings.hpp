#ifndef GAS_SRC_UTILITY_SETTINGS_HPP_
#define GAS_SRC_UTILITY_SETTINGS_HPP_

#include <string>

#include "fmt/format.h"

namespace gas {

class Settings {
 public:
  Settings() = default;
  auto operator==(const Settings& other) const -> bool = default;

  [[nodiscard]] auto host(std::string host) -> Settings& {
    host_ = std::move(host);
    return *this;
  }
  [[nodiscard]] auto port(size_t port) -> Settings& {
    port_ = port;
    return *this;
  }
  [[nodiscard]] auto db_name(std::string db_name) -> Settings& {
    db_name_ = std::move(db_name);
    return *this;
  }
  [[nodiscard]] auto username(std::string username) -> Settings& {
    username_ = std::move(username);
    return *this;
  }
  [[nodiscard]] auto password(std::string password) -> Settings& {
    password_ = std::move(password);
    return *this;
  }

  [[nodiscard]] auto to_string() const -> std::string {
    return fmt::format("host={} port={} dbname={} user={} password={}", host_,
                       std::to_string(port_), db_name_, username_, password_);
  }

 private:
  std::string host_;
  size_t port_{};
  std::string db_name_;
  std::string username_;
  std::string password_;
};

}  // namespace gas

#endif  // GAS_SRC_UTILITY_SETTINGS_HPP_
