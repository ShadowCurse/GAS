#ifndef GAMEASSETSDB_SRC_DATABASE_DB_CONNECTOR_HPP_
#define GAMEASSETSDB_SRC_DATABASE_DB_CONNECTOR_HPP_

#include <string>
#include <sstream>
#include <utility>
#include <pqxx/pqxx>
#include <iostream>

namespace gas {

class settings {
 public:
  settings() = default;

  auto host(std::string host) -> settings & {
    host_ = std::move(host);
    return *this;
  }
  auto port(size_t port) -> settings & {
    port_ = port;
    return *this;
  }
  auto db_name(std::string db_name) -> settings & {
    db_name_ = std::move(db_name);
    return *this;
  }
  auto username(std::string username) -> settings & {
    username_ = std::move(username);
    return *this;
  }
  auto password(std::string password) -> settings & {
    password_ = std::move(password);
    return *this;
  }

  auto to_string() const -> std::string {
    std::stringstream ss{};
    ss << " host=" << host_
       << " port=" << std::to_string(port_)
       << " dbname=" << db_name_
       << " user=" << username_
       << " password=" << password_;
    return ss.str();
  }

 private:
  std::string host_{};
  size_t port_{};
  std::string db_name_{};
  std::string username_{};
  std::string password_{};
};

class connector {
 public:
  connector() = default;
  bool connect(const settings &settings) {
    settings_ = settings;
    try {
      connection_ = std::make_unique<pqxx::connection>(settings.to_string());
    }
    catch (std::exception const &e) {
      return false;
    }
    return true;
  }

  auto exec(const std::string &sql) -> std::optional<pqxx::result> {
    pqxx::result result;
    try {
      pqxx::work work(*connection_);
      result = work.exec(sql);
    } catch (std::exception const &e) {
      return std::nullopt;
    }
    return result;
  }

 private:
  settings settings_{};
  std::unique_ptr<pqxx::connection> connection_;
};

} // namespace ga

#endif //GAMEASSETSDB_SRC_DATABASE_DB_CONNECTOR_HPP_
